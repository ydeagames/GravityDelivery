#include "ScenePlay.h"
#include "GameMain.h"
#include "GameObject.h"
#include "GameObjectHit.h"
#include "InputManager.h"
#include "Vector.h"
#include "GameUtils.h"
#include "GameObjects.h"
#include "SceneManager.h"
#include "GameStage.h"
#include <assert.h>


// 定数の定義 ==============================================================

#define screen_start(screen) \
{ \
	HGRP screen_stack = GetDrawScreen(); \
	SetDrawScreen(screen); \
	{
#define screen_end \
	} \
	SetDrawScreen(screen_stack); \
}

#define TUTORIAL_FLAG_TYPE_BEAM			(1<<0)
#define TUTORIAL_FLAG_TYPE_WARP			(1<<1)
#define TUTORIAL_FLAG_TYPE_VEL			(1<<2)
#define TUTORIAL_FLAG_TYPE_BEAM_BOUNCE	(1<<3)



// グローバル変数の定義 ====================================================

static GameObject g_field_ball;
static GameObject g_field_layer_top;
static Vector g_field_layers;
static GameTimer g_field_layer_timer;
static GameObject g_view;
static GameObject g_filter_screen;

static GameObject g_back_button;

static Vec2 g_mouse_last_from;
static Vec2 g_offset_mouse;
static Vec2 g_offset_location;
static int g_edit_mode;

static BOOL g_mouse_on_last;
static BOOL g_mouse_down;

static GameStage g_stage;

static int g_tutorial_state = 0;
static int g_tutorial_flag = 0;



// 関数の宣言 ==============================================================

void InitializePlay(void);  // ゲームの初期化処理
void UpdatePlay(void);      // ゲームの更新処理
void RenderPlay(void);      // ゲームの描画処理
void FinalizePlay(void);    // ゲームの終了処理

static void UpdateStageEdit(const Vec2* mouse);




// 関数の定義 ==============================================================

//----------------------------------------------------------------------
//! @brief ゲームの初期化処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void InitializePlay(void)
{
	g_field_ball = g_field;
	g_field_ball.size = Vec2_Create(3200, 1600);
	g_field_ball.sprite = GameSprite_Create(GameTexture_Create(MakeScreen((int)g_field_ball.size.x, (int)g_field_ball.size.y, TRUE), Vec2_Create(), g_field_ball.size));
	g_field_layer_top = g_field_ball;
	g_field_layer_top.sprite = GameSprite_Create(GameTexture_Create(MakeScreen((int)g_field_ball.size.x, (int)g_field_ball.size.y, TRUE), Vec2_Create(), g_field_ball.size));
	{
		int i;
		g_field_layers = Vector_Create(sizeof(GameObject));
		for (i = 0; i < 3; i++)
		{
			GameObject obj = g_field_ball;
			obj.sprite = GameSprite_Create(GameTexture_Create(MakeScreen((int)g_field_ball.size.x, (int)g_field_ball.size.y, TRUE), Vec2_Create(), g_field_ball.size));
			Vector_AddLast(&g_field_layers, &obj);
		}
	}
	g_field_layer_timer = GameTimer_Create();
	g_filter_screen = g_field;
	g_filter_screen.sprite = GameSprite_Create(GameTexture_Create(MakeScreen((int)g_filter_screen.size.x, (int)g_filter_screen.size.y, TRUE), Vec2_Create(), g_filter_screen.size));

	GameTimer_SetRemaining(&g_field_layer_timer, .5f);
	GameTimer_Resume(&g_field_layer_timer);
	g_view = g_field;

	{
		Vec2 size = Vec2_Create(150, 50);
		g_back_button = GameObject_Create(Vec2_Create(GameObject_GetX(&g_field, LEFT, -size.x / 2 - 50), GameObject_GetY(&g_field, BOTTOM, -size.y / 2 - 50)), Vec2_Create(), size);
		g_back_button.fill = TRUE;
		g_back_button.sprite.color = COLOR_GRAY;
	}

	g_edit_mode = -1;
	g_offset_mouse = Vec2_Create();
	g_mouse_on_last = FALSE;
	g_mouse_down = FALSE;

	g_stage = GameStage_Create();
	GameStage_Load(&g_stage, &g_selected_stageinfo);

	if (0 <= g_stage.bgm && g_stage.bgm < NUM_BGM)
	{
		ChangeVolumeSoundMem(100, g_resources.sound_bgm[g_stage.bgm]);
		PlaySoundMem(g_resources.sound_bgm[g_stage.bgm], DX_PLAYTYPE_LOOP);
	}

	{
		TCHAR title[260];
		sprintf(title, "%s - %s", GAME_TITLE, g_selected_stageinfo.title);
		SetMainWindowText(title);		// ウインドウタイトルの設定
	}
}




//----------------------------------------------------------------------
//! @brief ゲームの更新処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void UpdatePlay(void)
{
	Vec2 offset, mouse, mouse_last;

	// 座標系
	offset = Vec2_Sub(&g_view.pos, &g_field.pos);
	mouse = Vec2_Sub(&g_raw_mouse, &offset);
	mouse_last = Vec2_Sub(&g_raw_mouse_last, &offset);

	// 視差効果計算
	{
		float current_parallax = .2f;
		float parallax = .8f;
		Vec2 mouse_offset = Vec2_Scale(&mouse, -.02f);
		offset = Vec2_Add(&offset, &mouse_offset);
		mouse = Vec2_Sub(&g_raw_mouse, &offset);
	}

	// ドラッグ&ドロップ パン
	{
		if (IsMousePressed(MOUSE_INPUT_1))
		{
			g_offset_mouse = g_raw_mouse;
			g_offset_location = g_view.pos;
			g_mouse_down = TRUE;
		}
		if (IsMouseDown(MOUSE_INPUT_1))
		{
			if (g_mouse_down)
			{
				Vec2 diff = Vec2_Sub(&g_raw_mouse, &g_offset_mouse);
				g_view.pos = Vec2_Add(&g_offset_location, &diff);
			}
		}
		if (IsMouseReleased(MOUSE_INPUT_1))
		{
			g_mouse_down = FALSE;
		}

		GameObject_Field_CollisionVertical(&g_field_ball, &g_view, CONNECTION_BARRIER, EDGESIDE_INNER);
		GameObject_Field_CollisionHorizontal(&g_field_ball, &g_view, CONNECTION_BARRIER, EDGESIDE_INNER);
	}

	// エディット
	UpdateStageEdit(&mouse);

	// 惑星 ON/OFF
	if (IsMouseReleased(MOUSE_INPUT_1))
	{
		Vec2 diff = Vec2_Sub(&g_raw_mouse, &g_offset_mouse);
		if (Vec2_LengthSquared(&diff) < 5 * 5)
		{
			BOOL switched = FALSE;
			foreach_start(&g_stage.planets, GameObject, obj)
			{
				if (GameObject_IsAlive(obj))
				{
					switch (obj->type)
					{
					case TYPE_PLANET:
						GameObject mouseobj1 = GameObject_Create(mouse, Vec2_Create(), Vec2_Create(10, 10));
						mouseobj1.shape = SHAPE_CIRCLE;
						if (GameObject_IsHit(&mouseobj1, obj))
						{
							obj->state = !obj->state;
							//GameSprite_SetFrame(&obj->sprite, obj->state ? 12 : 9);
							GameObject_SetSize(obj, obj->state ? 4.f : 2.f, 8);
							if (g_tutorial_state == 0)
								g_tutorial_state = 1;
							switched = TRUE;
							PlaySoundMem(g_resources.sound_se[2], DX_PLAYTYPE_BACK);
						}
						break;
					default:
						GameObject mouseobj = GameObject_Create(mouse, Vec2_Create(), Vec2_Create(300, 300));
						if (GameObject_IsHit(&mouseobj, obj))
							switch (obj->type)
							{
							case TYPE_BEAM:
								g_tutorial_flag |= TUTORIAL_FLAG_TYPE_BEAM;
								break;
							case TYPE_WARP:
								if (GameObject_IsHit(&mouseobj, obj))
									g_tutorial_flag |= TUTORIAL_FLAG_TYPE_WARP;
								break;
							case TYPE_VEL:
								if (GameObject_IsHit(&mouseobj, obj))
									g_tutorial_flag |= TUTORIAL_FLAG_TYPE_VEL;
								break;
							case TYPE_BEAM_BOUNCE:
								if (GameObject_IsHit(&mouseobj, obj))
									g_tutorial_flag |= TUTORIAL_FLAG_TYPE_BEAM_BOUNCE;
								break;
							}
					}
				}
			} foreach_end;
			if (switched)
			{
				g_stage.score = 0;
				foreach_start(&g_stage.balls, GameObject, obj)
				{
					VectorIterator_Remove(&itr_obj);
				} foreach_end;
			}
		}
		else if (g_tutorial_state == 1)
			g_tutorial_state = 2;
	}

	// 惑星, ボール作用
	foreach_start(&g_stage.balls, GameObject, ball)
	{
		GameObject* nearest = NULL;
		float length2_min = -1;
		BOOL request_ballloop = FALSE;

		foreach_start(&g_stage.planets, GameObject, planet)
		{
			if (GameObject_IsAlive(planet))
			{
				GameObject line = GameObject_CreateLine(Vec2_Sub(&ball->pos, &ball->vel), ball->pos);
				switch (planet->type)
				{
				case TYPE_PLANET:
					// 一番近いものを検索
					if (planet->state)
					{
						float length2 = Vec2_LengthSquaredTo(&planet->pos, &ball->pos);

						//if (20 * 20 < length2 && length2 < 400 * 400)
						{
							if (nearest == NULL || length2 < length2_min)
							{
								nearest = planet;
								length2_min = length2;
							}
						}

						// 衝突したら消す
						if (GameObject_IsHit(planet, ball)) {
							VectorIterator_Remove(&itr_ball);
							request_ballloop = TRUE;
							PlaySoundMem(g_resources.sound_se[8], DX_PLAYTYPE_BACK);
						}
					}

					break;
				case TYPE_BEAM:
					// 衝突したら消す
					if (GameObject_IsHit(planet, &line)) {
						VectorIterator_Remove(&itr_ball);
						request_ballloop = TRUE;
						PlaySoundMem(g_resources.sound_se[8], DX_PLAYTYPE_BACK);
					}
					break;
				case TYPE_WARP:
					// 衝突したらワープ
					if (GameObject_IsHit(planet, &line)) {
						ball->pos = Vec2_Add(&planet->pos, &planet->vel);
						request_ballloop = TRUE;
						ChangeVolumeSoundMem(150, g_resources.sound_se[9]);
						PlaySoundMem(g_resources.sound_se[9], DX_PLAYTYPE_BACK);
					}
					break;
				case TYPE_VEL:
					// 衝突したら速度変更
					if (GameObject_IsHit(planet, &line)) {
						ball->vel = Vec2_Scale(&planet->vel, .1f);
						request_ballloop = TRUE;
						ChangeVolumeSoundMem(150, g_resources.sound_se[10]);
						PlaySoundMem(g_resources.sound_se[10], DX_PLAYTYPE_BACK);
					}
					break;
				case TYPE_BEAM_BOUNCE:
					// 衝突したら消す
					if (GameObject_IsHit(planet, &line)) {
						Vec2 planet_p1 = Vec2_Create(GameObject_GetRawX(planet, LEFT), GameObject_GetRawY(planet, TOP));
						Vec2 planet_p2 = Vec2_Create(GameObject_GetRawX(planet, RIGHT), GameObject_GetRawY(planet, BOTTOM));
						Vec2 planet_line = Vec2_Sub(&planet_p2, &planet_p1);

						Vec2 vecA;
						Vec2 vecB;

						Vec2_Decompose(&ball->vel, &planet_line, &vecA, &vecB);

						vecB = Vec2_Negate(&vecB);

						ball->vel = Vec2_Add(&vecA, &vecB);
						ball->pos = Vec2_Add(&ball->pos, &ball->vel);

						request_ballloop = TRUE;
						PlaySoundMem(g_resources.sound_se[8], DX_PLAYTYPE_BACK);
					}
					break;
				case TYPE_GOAL:
					// 衝突したら加点
					if (GameObject_IsHit(ball, planet))
					{
						VectorIterator_Remove(&itr_ball);
						g_stage.score++;
						ChangeVolumeSoundMem(100, g_resources.sound_se[0]);
						if (g_tutorial_state == 2)
							g_tutorial_state = 3;
						PlaySoundMem(g_resources.sound_se[0], DX_PLAYTYPE_BACK);
						request_ballloop = TRUE;
					}
					break;
				}

				if (request_ballloop)
					break;
			}
		} foreach_end;

		// 一番近いものを寄せる
		if (nearest != NULL)
			ball->vel = Vec2_Add(&ball->vel, &Vec2_Scale(&Vec2_Normalized(&Vec2_Sub(&nearest->pos, &ball->pos)), .1f));
	} foreach_end;

	/*
	// ボール同士の判定
	foreach_start(&g_stage.balls, GameObject, ball1)
	{
		foreach_start(&g_stage.balls, GameObject, ball2)
		{
			if (ball1 != ball2)
				// ボール同士が衝突したら消す
				if (GameObject_IsHit(ball1, ball2))
				{
					VectorIterator_Remove(&itr_ball1);
					break;
				}
		} foreach_end;
	} foreach_end;
	/**/

	// 惑星 更新
	foreach_start(&g_stage.planets, GameObject, planet)
	{
		if (GameObject_IsAlive(planet))
		{
			switch (planet->type)
			{
			case TYPE_PLANET:
				if (planet->state)
					planet->sprite.angle += ToRadians(1);
				break;
			case TYPE_START:
				if (GameTimer_IsFinished(&planet->count))
				{
					GameTimer_SetRemaining(&planet->count, .5f);
					GameTimer_Resume(&planet->count);

					{
						Vec2 vec = Vec2_Scale(&planet->vel, .1f);
						GameObject obj = GameObject_Ball_Create(&planet->pos, &vec);
						Vector_AddLast(&g_stage.balls, &obj);
					}
				}

				break;
			}
		}
	} foreach_end;

	// ボール 更新
	foreach_start(&g_stage.balls, GameObject, ball)
	{
		GameObject_UpdatePosition(ball);

		// 減速
		//ball->vel.x *= 0.998f;
		//ball->vel.y *= 0.998f;

		// ワールドボーダー
		if (GameObject_Field_CollisionHorizontal(&g_field_ball, ball, CONNECTION_NONE, EDGESIDE_OUTER) ||
			GameObject_Field_CollisionVertical(&g_field_ball, ball, CONNECTION_NONE, EDGESIDE_OUTER))
			VectorIterator_Remove(&itr_ball);
	} foreach_end;

	// タイトルへ戻る
	if (IsMousePressed(MOUSE_INPUT_1))
	{
		if (GameObject_IsHitPoint(&g_back_button, &g_raw_mouse))
		{
			RequestScene(SCENE_TITLE, COLOR_GRAY, .5f);
			PlaySoundMem(g_resources.sound_se[5], DX_PLAYTYPE_BACK);
		}
	}
	// タイトルへ戻る onカーソル
	if (GameObject_IsHitPoint(&g_back_button, &g_raw_mouse) && !GameObject_IsHitPoint(&g_back_button, &g_raw_mouse_last))
		PlaySoundMem(g_resources.sound_se[1], DX_PLAYTYPE_BACK);

	// 惑星 onカーソル
	{
		BOOL mouse_on = FALSE;
		foreach_start(&g_stage.planets, GameObject, obj)
		{
			if (GameObject_IsAlive(obj))
			{
				switch (obj->type)
				{
				case TYPE_PLANET:
					GameObject mouseobj = GameObject_Create(mouse, Vec2_Create(), Vec2_Create(10, 10));
					mouseobj.shape = SHAPE_CIRCLE;
					if (GameObject_IsHit(&mouseobj, obj))
						mouse_on = TRUE;
				}
			}
		} foreach_end;
		if (mouse_on && !g_mouse_on_last)
			PlaySoundMem(g_resources.sound_se[1], DX_PLAYTYPE_BACK);
		g_mouse_on_last = mouse_on;
	}

	// クリア判定
	if (g_stage.score >= 10 && !g_stage.edited)
	{
		foreach_start(&g_stage.planets, GameObject, obj)
		{
			if (GameObject_IsAlive(obj))
			{
				switch (obj->type)
				{
				case TYPE_GOAL:
					obj->sprite = GameSprite_Create(GameTexture_Create(g_resources.texture[0], Vec2_Create(), Vec2_Create(26, 26)));
					obj->sprite.num_columns = 8;
					obj->sprite.animation = GameSpriteAnimation_Create(19, 22, 8);
					obj->sprite.animation.loop_flag = FALSE;
					GameObject_SetSize(obj, 2, 32);
					obj->type = TYPE_GOAL_DOOM;
					ChangeVolumeSoundMem(150, g_resources.sound_se[6]);
					PlaySoundMem(g_resources.sound_se[6], DX_PLAYTYPE_BACK);
					break;
				case TYPE_GOAL_DOOM:
					if (GameSpriteAnimation_Update(&obj->sprite) == ANIMATION_FINISHED)
						RequestScene(SCENE_RESULT, COLOR_WHITE, 1.5f);
					break;
				}
			}
		} foreach_end;
	}
}

static void UpdateStageEdit_HandlePlanetControl(const Vec2* mouse, int id, int key, int type, BOOL unique)
{
	if (IsMousePressed(MOUSE_INPUT_3) && IsKeyDown(key))
	{
		g_mouse_last_from = *mouse;
		g_edit_mode = id;
		g_stage.edited = TRUE;
	}
	if (IsMouseReleased(MOUSE_INPUT_3) && g_edit_mode == id)
	{
		Vec2 mouse_last_to = *mouse;
		GameObject obj = GameObject_Planets_Create(type, &g_mouse_last_from, &mouse_last_to);

		if (unique)
			foreach_start(&g_stage.planets, GameObject, planet)
		{
			if (planet->type == type)
				VectorIterator_Remove(&itr_planet);
		} foreach_end;

		Vector_AddLast(&g_stage.planets, &obj);
		g_edit_mode = -1;
	}
}

static void UpdateStageEdit(const Vec2* mouse)
{
	int id = 0;
	UpdateStageEdit_HandlePlanetControl(mouse, id++, KEY_INPUT_S, TYPE_START, TRUE);
	UpdateStageEdit_HandlePlanetControl(mouse, id++, KEY_INPUT_G, TYPE_GOAL, TRUE);
	UpdateStageEdit_HandlePlanetControl(mouse, id++, KEY_INPUT_Q, TYPE_PLANET, FALSE);
	UpdateStageEdit_HandlePlanetControl(mouse, id++, KEY_INPUT_B, TYPE_BEAM, FALSE);
	UpdateStageEdit_HandlePlanetControl(mouse, id++, KEY_INPUT_R, TYPE_BEAM_BOUNCE, FALSE);
	UpdateStageEdit_HandlePlanetControl(mouse, id++, KEY_INPUT_W, TYPE_WARP, FALSE);
	UpdateStageEdit_HandlePlanetControl(mouse, id++, KEY_INPUT_V, TYPE_VEL, FALSE);

	// Remover
	if (IsMousePressed(MOUSE_INPUT_3) && IsKeyDown(KEY_INPUT_DELETE))
	{
		foreach_start(&g_stage.planets, GameObject, obj)
		{
			if (GameObject_IsAlive(obj))
			{
				GameObject mouseobj = GameObject_Create(*mouse, Vec2_Create(), Vec2_Create(10, 10));
				mouseobj.shape = SHAPE_CIRCLE;
				if (GameObject_IsHit(obj, &mouseobj))
					VectorIterator_Remove(&itr_obj);
			}
		} foreach_end;
		g_edit_mode = id;
		g_stage.edited = TRUE;
	}

	// Migration
	if (IsKeyDown(KEY_INPUT_LCONTROL) && IsKeyPressed(KEY_INPUT_M))
	{
		foreach_start(&g_stage.planets, GameObject, planet)
		{
			if (planet->type == TYPE_START)
				planet->vel = Vec2_Add(&planet->vel, &planet->pos);
		} foreach_end;

		DebugConsole_Log(&g_console, "migrated!");
		g_stage.edited = TRUE;
	}

	// Load
	if (IsKeyDown(KEY_INPUT_LCONTROL) && IsKeyPressed(KEY_INPUT_O))
	{
		GameStage_Load(&g_stage, &g_selected_stageinfo);
		DebugConsole_Log(&g_console, "stage loaded!");
	}

	// Save
	if (IsKeyDown(KEY_INPUT_LCONTROL) && IsKeyPressed(KEY_INPUT_S))
	{
		GameStage_Save(&g_stage, &g_selected_stageinfo);
		DebugConsole_Log(&g_console, "stage saved!");
	}
}




//----------------------------------------------------------------------
//! @brief ゲームの描画処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void RenderPlay(void)
{
	Vec2 offset = Vec2_Sub(&g_view.pos, &g_field.pos);
	Vec2 offset_shadow = Vec2_Create(GameObject_GetX(&g_field_ball, LEFT) - GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field_ball, TOP) - GameObject_GetY(&g_field, TOP));
	Vec2 mouse = Vec2_Sub(&g_raw_mouse, &offset);

	{
		float current_parallax = .2f;
		float parallax = .8f;
		Vec2 mouse_offset = Vec2_Scale(&mouse, -.02f);
		offset = Vec2_Add(&offset, &mouse_offset);
		mouse = Vec2_Sub(&g_raw_mouse, &offset);
		foreach_start(&g_field_layers, GameObject, layer)
		{
			current_parallax *= parallax;
			screen_start(g_filter_screen.sprite.texture.texture)
			{
				ClearDrawScreen();
				GameObject_Field_Render(layer, &Vec2_Scale(&Vec2_Add(&offset, &mouse_offset), current_parallax), 12);
				GraphFilter(g_filter_screen.sprite.texture.texture, DX_GRAPH_FILTER_GAUSS, 16, 128);
			} screen_end;
			GameObject_Render(&g_filter_screen);
		} foreach_end;
	}
	GameObject_Field_Render(&g_field_ball, &offset, 16);

	{
		BOOL first_planet = TRUE;
		foreach_start(&g_stage.planets, GameObject, obj)
		{
			if (GameObject_IsAlive(obj))
			{
				GameObject mouseobj = GameObject_Create(mouse, Vec2_Create(), Vec2_Create(300, 300));
				mouseobj.shape = SHAPE_CIRCLE;
				if (DEBUG_HITBOX)
				{
					mouseobj.sprite.color = COLOR_GRAY;
					mouseobj.edge = .5f;
					GameObject_Render(&mouseobj, &offset);
				}
				switch (obj->type)
				{
				case TYPE_GOAL:
					DrawFormatStringToHandle((int)(GameObject_GetX(obj, LEFT) + offset.x + 2), (int)(GameObject_GetY(obj, BOTTOM, 10) + offset.y), COLOR_WHITE, g_resources.font_main, "%d / 10", g_stage.score);
					GameObject_Render(obj, &offset);
					if (g_tutorial_state == 2)
						GameObject_Msg_Render(&Vec2_Add(&obj->pos, &Vec2_Create(0, -10)), &offset, "ゴールはここ。うまく導こう！");
					break;
				case TYPE_START:
					GameObject_Render(obj, &offset);
					if (DEBUG_HITBOX)
						Vec2_Render(&obj->vel, &Vec2_Add(&obj->pos, &offset), obj->sprite.color);
					break;
				case TYPE_BEAM:
					GameObject_Render(obj, &offset);
					if (~g_tutorial_flag & TUTORIAL_FLAG_TYPE_BEAM && GameObject_IsHit(&mouseobj, obj))
						GameObject_Msg_Render(&Vec2_Add(&obj->pos, &Vec2_Create(0, -10)), &offset, "ビームは触れると消滅してしまう！");
					break;
				case TYPE_WARP:
					GameObject_Render(obj, &offset);
					SetDrawBright(255 - 10, 255 - 140, 255 - 140);
					GameObject_Render(obj, &Vec2_Add(&obj->vel, &offset));
					SetDrawBright(255, 255, 255);
					if (DEBUG_HITBOX)
						Vec2_Render(&obj->vel, &Vec2_Add(&obj->pos, &offset), obj->sprite.color);
					if (~g_tutorial_flag & TUTORIAL_FLAG_TYPE_WARP && GameObject_IsHit(&mouseobj, obj))
						GameObject_Msg_Render(&Vec2_Add(&obj->pos, &Vec2_Create(0, -10)), &offset, "ゲートは触れるともう一つのゲートにワープするぞ！");
					break;
				case TYPE_VEL:
					GameObject_Render(obj, &offset);
					if (DEBUG_HITBOX)
						Vec2_Render(&obj->vel, &Vec2_Add(&obj->pos, &offset), obj->sprite.color);
					if (~g_tutorial_flag & TUTORIAL_FLAG_TYPE_WARP && GameObject_IsHit(&mouseobj, obj))
						GameObject_Msg_Render(&Vec2_Add(&obj->pos, &Vec2_Create(0, -10)), &offset, "ランチャーは触れると打ち出されるぞ！");
					break;
				case TYPE_BEAM_BOUNCE:
					GameObject_Render(obj, &offset);
					if (~g_tutorial_flag & TUTORIAL_FLAG_TYPE_BEAM_BOUNCE && GameObject_IsHit(&mouseobj, obj))
						GameObject_Msg_Render(&Vec2_Add(&obj->pos, &Vec2_Create(0, -10)), &offset, "壁は触れるとバウンドするぞ！");
					break;
				case TYPE_PLANET:
					GameObject_Render(obj, &offset);
					if (g_tutorial_state == 0 && first_planet)
						GameObject_Msg_Render(&Vec2_Add(&obj->pos, &Vec2_Create(0, -10)), &offset, "惑星をクリックして重力のスイッチをON/OFFしてみよう！");
					first_planet = FALSE;
					break;
				default:
					GameObject_Render(obj, &offset);
					break;
				}
			}
		} foreach_end;
	}

	if (g_tutorial_state == 1)
		GameObject_Msg_Render(&Vec2_Add(&g_raw_mouse, &Vec2_Create(0, -10)), &Vec2_Create(), "ドラッグして画面をずらすよ！");

	foreach_start(&g_stage.balls, GameObject, obj)
	{
		if (GameObject_IsAlive(obj))
		{
			screen_start(g_field_ball.sprite.texture.texture)
			{
				Vec2 before = Vec2_Sub(&obj->pos, &obj->vel);
				DrawLineAA(before.x - offset_shadow.x, before.y - offset_shadow.y, obj->pos.x - offset_shadow.x, obj->pos.y - offset_shadow.y, obj->sprite.color);
			} screen_end;
			GameObject_Render(obj, &offset);
		}
	} foreach_end;
	if (GameTimer_IsFinished(&g_field_layer_timer))
	{
		GameTimer_SetRemaining(&g_field_layer_timer, .05f);
		GameTimer_Resume(&g_field_layer_timer);

		foreach_start(&g_field_layers, GameObject, layer)
		{
			screen_start(layer->sprite.texture.texture)
			{
				unsigned int color = GetColor(GetRand(255), GetRand(255), GetRand(255));
				Vec2 pos = Vec2_Create(GetRandRangeF(GameObject_GetX(layer, LEFT), GameObject_GetX(layer, RIGHT)),
					GetRandRangeF(GameObject_GetY(layer, TOP), GameObject_GetY(layer, BOTTOM)));
				DrawCircleAA(pos.x - offset_shadow.x, pos.y - offset_shadow.y, 6, 4, color);
			} screen_end;
		} foreach_end;
	}
	foreach_start(&g_stage.planets, GameObject, obj)
	{
		if (GameObject_IsAlive(obj))
		{
			switch (obj->type)
			{
			case TYPE_PLANET:
				if (Vec2_LengthSquaredTo(&mouse, &obj->pos) < Vec2_LengthSquared(&obj->size))
				{
					GameObject cursor = *obj;
					cursor.sprite = GameSprite_Create(GameTexture_Create(g_resources.texture[5], Vec2_Create(), Vec2_Create(18, 18)));
					cursor.sprite.num_columns = 5;
					GameSprite_SetFrame(&cursor.sprite, obj->state ? 5 : 7);
					GameObject_SetSize(&cursor, 4);
					GameObject_Render(&cursor, &offset);
				}
			}
		}
	} foreach_end;

	if (DEBUG_HITBOX)
	{
		int pos = 0;
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "デバッグ情報");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "stage: %s", g_selected_stageinfo.filename);
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "all: %d", Vector_GetSize(&g_stage.balls));
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "score: %d", g_stage.score);
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "edited: %s", g_stage.edited ? "true" : "false");
		pos++;
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[S]+MClick      - スタート地点 [Unique]");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[G]+MClick      - ゴール地点 [Unique]");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[Q]+MClick      - 惑星");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[B]+MClick      - ビーム");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[R]+MClick      - バウンスビーム");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[W]+MClick      - ワープ");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[V]+MClick      - ランチャー");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[Delete]+MClick - 撤去");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "Ctrl+[O]        - ロード");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "Ctrl+[S]        - セーブ");
	}

	{
		if (GameObject_IsHitPoint(&g_back_button, &g_raw_mouse))
			GameObject_Render(&g_back_button);
		DrawFormatStringToHandle((int)GameObject_GetX(&g_back_button, LEFT, -10), (int)GameObject_GetY(&g_back_button, TOP, -20), COLOR_WHITE, g_resources.font_main, "タイトルへ戻る");
	}
}



//----------------------------------------------------------------------
//! @brief ゲームの終了処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void FinalizePlay(void)
{
	DeleteGraph(g_field_ball.sprite.texture.texture);
	foreach_start(&g_field_layers, GameObject, layer)
	{
		DeleteGraph(layer->sprite.texture.texture);
	} foreach_end;
	{
		int i;
		for (i = 0; i < NUM_BGM; i++)
			StopSoundMem(g_resources.sound_bgm[i]);
	}

	Vector_Delete(&g_field_layers);
	GameStage_Dispose(&g_stage);

	SetMainWindowText(GAME_TITLE);		// ウインドウタイトルの設定
}
