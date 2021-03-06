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
#include "GamePause.h"
#include <assert.h>
#include <math.h>


// 定数の定義 ==============================================================

#define TUTORIAL_FLAG_TYPE_BEAM			(1<<0)
#define TUTORIAL_FLAG_TYPE_WARP			(1<<1)
#define TUTORIAL_FLAG_TYPE_VEL			(1<<2)
#define TUTORIAL_FLAG_TYPE_BEAM_BOUNCE	(1<<3)

#define SPEED_MAX_EXPO					3



// グローバル変数の定義 ====================================================

static GameObject g_field_ball;
static GameObject g_field_layer_top;
static Vector g_field_layers;
static GameTimer g_field_layer_timer;
static GameObject g_view;
static GameObject g_filter_screen;

static GameObject g_back_button;
static GameObject g_menu_button;
static GameObject g_speed_button;

static Vec2 g_mouse_last_from;
static Vec2 g_offset_mouse;
static Vec2 g_offset_location;
static int g_edit_mode;

static Vec2 g_offset_shadow;
static GameObject g_offset_shake;

static BOOL g_mouse_on_last;
static BOOL g_mouse_down;

static GameStage g_stage;
static BOOL g_stage_screenshot = FALSE;

static int g_tutorial_state = 0;
static int g_tutorial_flag = 0;

static int g_speed_expo;

static GameTimer g_planet_circle_timer;



// 関数の宣言 ==============================================================

void InitializePlay(void);  // ゲームの初期化処理
void UpdatePlay(void);      // ゲームの更新処理
void RenderPlay(void);      // ゲームの描画処理
void FinalizePlay(void);    // ゲームの終了処理

static void ShakeField(float magnification);
static GameObject GetRandomParticleObject(int type, const Vec2* pos, float magnification);
static void UpdatePlayTicks(void);
static void UpdateStageEdit(const Vec2* mouse);
static void RenderBalloon(const GameObject* field, const GameObject* obj, const Vec2* offset);




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
		g_back_button.edge = 1;
		g_back_button.sprite.color = COLOR_GRAY;
	}
	{
		Vec2 size = Vec2_Create(80, 50);
		g_menu_button = GameObject_Create(Vec2_Create(GameObject_GetX(&g_field, LEFT, -size.x / 2 - 210), GameObject_GetY(&g_field, BOTTOM, -size.y / 2 - 50)), Vec2_Create(), size);
		g_menu_button.edge = 1;
		g_menu_button.sprite.color = COLOR_GRAY;
	}
	{
		Vec2 size = Vec2_Create(40, 50);
		g_speed_button = GameObject_Create(Vec2_Create(GameObject_GetX(&g_field, LEFT, -size.x / 2 - 300), GameObject_GetY(&g_field, BOTTOM, -size.y / 2 - 50)), Vec2_Create(), size);
		g_speed_button.edge = 1;
		g_speed_button.sprite.color = COLOR_GRAY;
	}

	g_edit_mode = -1;
	g_offset_mouse = Vec2_Create();
	g_mouse_on_last = FALSE;
	g_mouse_down = FALSE;

	g_offset_shadow = Vec2_Create(GameObject_GetX(&g_field_ball, LEFT) - GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field_ball, TOP) - GameObject_GetY(&g_field, TOP));

	g_offset_shake = GameObject_Create();

	g_speed_expo = 0;

	g_stage = GameStage_Create();
	GameStage_Load(&g_stage, &g_selected_stageinfo);

	if (0 <= g_stage.bgm && g_stage.bgm < NUM_BGM)
	{
		PlaySoundMem(g_resources.sound_bgm[g_stage.bgm], DX_PLAYTYPE_LOOP);
	}

	{
		TCHAR title[260];
		sprintf(title, "%s - %s", GAME_TITLE, g_selected_stageinfo.title);
		SetMainWindowText(title);		// ウインドウタイトルの設定
	}

	g_planet_circle_timer = GameTimer_Create();
	GameTimer_SetRemaining(&g_planet_circle_timer, 2);
	GameTimer_Resume(&g_planet_circle_timer);
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
	offset = Vec2_Add(&Vec2_Sub(&g_view.pos, &g_field.pos), &g_offset_shake.pos);
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

	// 惑星 onカーソル
	{
		BOOL mouse_on = FALSE;
		BOOL switched = FALSE;
		BOOL switch_on = FALSE;
		foreach_start(&g_stage.planets, GameObject, obj)
		{
			if (GameObject_IsAlive(obj))
			{
				GameObject mouseobj;
				switch (obj->type)
				{
				case TYPE_PLANET:
					BOOL hit, released, dragged;
					// 惑星 ON/OFF
					{
						mouseobj = GameObject_Create(mouse, Vec2_Create(), Vec2_Create(40, 40));
						mouseobj.shape = SHAPE_CIRCLE;
						hit = GameObject_IsHit(&mouseobj, obj);
					}
					{
						Vec2 diff = Vec2_Sub(&g_raw_mouse, &g_offset_mouse);
						dragged = Vec2_LengthSquared(&diff) >= 5 * 5;
					}
					{
						released = IsMouseReleased(MOUSE_INPUT_1);
					}
					if (!dragged && released && hit)
					{
						obj->state = !obj->state;
						//GameSprite_SetFrame(&obj->sprite, obj->state ? 12 : 9);
						GameObject_SetSize(obj, obj->state ? 1.f : .5f, 24);
						if (g_tutorial_state == 0)
							g_tutorial_state = 1;
						switched = TRUE;
						switch_on = obj->state;
					}
					if (hit)
						mouse_on = TRUE;
					if (dragged && released && g_tutorial_state == 1)
						g_tutorial_state = 2;
					break;
				default:
					mouseobj = GameObject_Create(mouse, Vec2_Create(), Vec2_Create(300, 300));
					if (GameObject_IsHit(&mouseobj, obj) && IsMousePressed(MOUSE_INPUT_1))
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
			PlaySoundMem(g_resources.sound_se[switch_on ? 2 : 15], DX_PLAYTYPE_BACK);
		}
		if (mouse_on && !g_mouse_on_last)
			PlaySoundMem(g_resources.sound_se[1], DX_PLAYTYPE_BACK);
		g_mouse_on_last = mouse_on;
	}

	// Tickの更新
	{
		int i;
		for (i = 0; i < 1 << g_speed_expo; i++)
			UpdatePlayTicks();
	}

	if (IsMousePressed(MOUSE_INPUT_1))
	{
		// タイトルへ戻る
		if (GameObject_IsHitPoint(&g_back_button, &g_raw_mouse))
		{
			RequestScene(SCENE_TITLE, COLOR_GRAY, .5f);
			PlaySoundMem(g_resources.sound_se[5], DX_PLAYTYPE_BACK);
		}
		// メニュー
		if (GameObject_IsHitPoint(&g_menu_button, &g_raw_mouse))
		{
			g_mouse_down = FALSE;
			RequestPause();
		}
	}
	// 速度
	if (GameObject_IsHitPoint(&g_speed_button, &g_raw_mouse))
	{
		BOOL changed = TRUE;
		if (IsMousePressed(MOUSE_INPUT_1))
			g_speed_expo = GetLoop(g_speed_expo + 1, SPEED_MAX_EXPO + 1);
		else if (IsMousePressed(MOUSE_INPUT_2))
			g_speed_expo = GetLoop(g_speed_expo - 1, SPEED_MAX_EXPO + 1);
		else if (IsMousePressed(MOUSE_INPUT_3))
			g_speed_expo = 0;
		else
			changed = FALSE;
		if (changed)
		{
			PlaySoundMem(g_resources.sound_se[11], DX_PLAYTYPE_BACK);
		}
	}
	// タイトルへ戻る onカーソル
	if (GameObject_IsHitPoint(&g_back_button, &g_raw_mouse) && !GameObject_IsHitPoint(&g_back_button, &g_raw_mouse_last))
		PlaySoundMem(g_resources.sound_se[1], DX_PLAYTYPE_BACK);
	// メニュー onカーソル
	if (GameObject_IsHitPoint(&g_menu_button, &g_raw_mouse) && !GameObject_IsHitPoint(&g_menu_button, &g_raw_mouse_last))
		PlaySoundMem(g_resources.sound_se[1], DX_PLAYTYPE_BACK);
	// 速度 onカーソル
	if (GameObject_IsHitPoint(&g_speed_button, &g_raw_mouse) && !GameObject_IsHitPoint(&g_speed_button, &g_raw_mouse_last))
		PlaySoundMem(g_resources.sound_se[1], DX_PLAYTYPE_BACK);

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
					GameObject doom = GameObject_Particles_Create(TYPE_PARTICLE_GOAL_DOOM_SPAWNER, &obj->pos, &Vec2_Create());
					Vector_AddLast(&g_stage.balls, &doom);

					PlaySoundMem(g_resources.sound_se[6], DX_PLAYTYPE_BACK);

					VectorIterator_Remove(&itr_obj);
					break;
				}
			}
		} foreach_end;
		foreach_start(&g_stage.balls, GameObject, obj)
		{
			if (GameObject_IsAlive(obj))
			{
				switch (obj->type)
				{
				case TYPE_PARTICLE_GOAL_DOOM_SPAWNER:
					if (GameTimer_IsFinished(&obj->count))
						RequestScene(SCENE_RESULT, COLOR_WHITE, 1.5f);
					{
						ShakeField(20);
						VectorIterator_Add(&itr_obj, &GetRandomParticleObject(TYPE_PARTICLE_GOAL_DOOM, &obj->pos, GameTimer_GetProgress(&obj->count) * 200));
					}
					break_obj = TRUE;
					break;
				}
			}
		} foreach_end;
	}

	// 画面揺れ
	GameObject_UpdatePosition(&g_offset_shake);
	g_offset_shake.vel = Vec2_Add(&Vec2_Scale(&g_offset_shake.vel, .75f), &Vec2_Scale(&g_offset_shake.pos, -.85f));

	// 惑星円タイマー
	if (GameTimer_IsFinished(&g_planet_circle_timer))
		GameTimer_ResetRemaining(&g_planet_circle_timer);
}

// 画面揺れ
static void ShakeField(float magnification)
{
	Vec2 delta_pos = Vec2_Scale(&Vec2_Create(GetRandRangeF(-1, 1), GetRandRangeF(-1, 1)), magnification);
	g_offset_shake.pos = Vec2_Add(&g_offset_shake.pos, &delta_pos);
}

// ランダムオブジェクト
static GameObject GetRandomParticleObject(int type, const Vec2* pos, float magnification)
{
	Vec2 delta_pos = Vec2_Scale(&Vec2_Create(GetRandRangeF(-1, 1), GetRandRangeF(-1, 1)), magnification);
	Vec2 particle_pos = Vec2_Add(pos, &delta_pos);
	return GameObject_Particles_Create(type, &particle_pos, &Vec2_Create());
}

// 1ティックの更新
static void UpdatePlayTicks(void)
{
	// 重力が存在するか
	BOOL gravity_exists = TRUE;

	// 惑星, ボール当たり判定
	foreach_start(&g_stage.balls, GameObject, ball)
	{
		foreach_start(&g_stage.planets, GameObject, planet)
		{
			if (GameObject_IsAlive(planet))
			{
				GameObject line = GameObject_CreateLine(Vec2_Sub(&ball->pos, &ball->vel), ball->pos);
				if (GameObject_IsHit(planet, &line))
				{
					switch (planet->type)
					{
					case TYPE_PLANET:
						// 衝突したら消す
						if (ball->type == TYPE_PARTICLE_BALL)
						{
							VectorIterator_Set(&itr_ball, &GetRandomParticleObject(TYPE_PARTICLE_DOOM, &planet->pos, 5));
							ShakeField(5);
							PlaySoundMem(g_resources.sound_se[8], DX_PLAYTYPE_BACK);
						}
						else if (ball->type != TYPE_PARTICLE_DOOM)
							VectorIterator_Remove(&itr_ball);
						break_planet = TRUE;
						break;
					case TYPE_BEAM:
						// 衝突したら消す
						if (ball->type == TYPE_PARTICLE_BALL)
						{
							VectorIterator_Set(&itr_ball, &GetRandomParticleObject(TYPE_PARTICLE_DOOM, &ball->pos, 20));
							ShakeField(10);

							PlaySoundMem(g_resources.sound_se[14], DX_PLAYTYPE_BACK);
						}
						else if (ball->type != TYPE_PARTICLE_DOOM)
							VectorIterator_Remove(&itr_ball);
						break_planet = TRUE;
						break;
					case TYPE_WARP:
						// 衝突したらワープ
						ball->pos = Vec2_Add(&planet->pos, &planet->vel);
						if (ball->type == TYPE_PARTICLE_BALL)
						{
							PlaySoundMem(g_resources.sound_se[9], DX_PLAYTYPE_BACK);
						}
						break_planet = TRUE;
						break;
					case TYPE_VEL:
						// 衝突したら速度変更
						ball->vel = Vec2_Scale(&planet->vel, .1f);
						if (ball->type == TYPE_PARTICLE_BALL)
						{
							PlaySoundMem(g_resources.sound_se[10], DX_PLAYTYPE_BACK);
						}
						break_planet = TRUE;
						break;
					case TYPE_BEAM_BOUNCE:
						// 衝突したら消す
						Vec2 planet_p1 = Vec2_Create(GameObject_GetRawX(planet, LEFT), GameObject_GetRawY(planet, TOP));
						Vec2 planet_p2 = Vec2_Create(GameObject_GetRawX(planet, RIGHT), GameObject_GetRawY(planet, BOTTOM));
						Vec2 planet_line = Vec2_Sub(&planet_p2, &planet_p1);

						Vec2 vecA;
						Vec2 vecB;

						Vec2_Decompose(&ball->vel, &planet_line, &vecA, &vecB);

						vecB = Vec2_Negate(&vecB);

						ball->vel = Vec2_Add(&vecA, &vecB);
						ball->pos = Vec2_Add(&ball->pos, &ball->vel);

						if (ball->type == TYPE_PARTICLE_BALL)
						{
							//ShakeField(1);
							PlaySoundMem(g_resources.sound_se[13], DX_PLAYTYPE_BACK);
						}
						break_planet = TRUE;
						break;
					case TYPE_GOAL:
						// 衝突したら加点
						if (ball->type == TYPE_PARTICLE_BALL)
						{
							g_stage.score++;
							if (g_tutorial_state == 2)
								g_tutorial_state = 3;
							PlaySoundMem(g_resources.sound_se[0], DX_PLAYTYPE_BACK);
							PlaySoundMem(g_resources.sound_se[12], DX_PLAYTYPE_BACK);
							{
								ShakeField(15);
								VectorIterator_Set(&itr_ball, &GetRandomParticleObject(TYPE_PARTICLE_GOAL_DOOM, &planet->pos, 40));
							}
						}
						break_planet = TRUE;
						break;
					}
				}
			}
		} foreach_end;
	} foreach_end;

	// 惑星, ボール作用
	foreach_start(&g_stage.balls, GameObject, ball)
	{
		GameObject* nearest = NULL;
		float length2_min = -1;

		foreach_start(&g_stage.planets, GameObject, planet)
		{
			if (GameObject_IsAlive(planet))
			{
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
					}
				}
			}
		} foreach_end;

		switch (ball->type)
		{
		case TYPE_PARTICLE_BALL:
		case TYPE_PARTICLE_ACTIVE:
		case TYPE_PARTICLE_GRAVITY:
			// 一番近いものを寄せる
			if (nearest != NULL)
				ball->vel = Vec2_Add(&ball->vel, &Vec2_Scale(&Vec2_Normalized(&Vec2_Sub(&nearest->pos, &ball->pos)), .1f));
			break;
		}
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
			if (GameSpriteAnimation_Update(&planet->sprite) == ANIMATION_FINISHED && !planet->sprite.animation.loop_flag)
			{
				VectorIterator_Remove(&itr_planet);
				continue;
			}

			switch (planet->type)
			{
			case TYPE_PLANET:
				if (planet->state)
				{
					planet->sprite.angle += ToRadians(1);
					{
						Vec2 particle_pos = Vec2_Add(&planet->pos, &Vec2_Scale(&Vec2_Create(cosf(planet->sprite.angle*21.8f), sinf(planet->sprite.angle*21.8f)), 80));
						GameObject particle = GameObject_Particles_Create(TYPE_PARTICLE_ACTIVE, &particle_pos, &Vec2_Create());
						Vector_AddLast(&g_stage.balls, &particle);
					}

					gravity_exists = TRUE;
				}
				break;
			case TYPE_START:
				if (GameTimer_IsFinished(&planet->count))
				{
					GameTimer_SetRemaining(&planet->count, .5f / (1 << g_speed_expo));
					GameTimer_Resume(&planet->count);

					{
						Vec2 vec = Vec2_Scale(&planet->vel, .1f);
						GameObject obj = GameObject_Ball_Create(&planet->pos, &vec);
						Vector_AddLast(&g_stage.balls, &obj);
					}
				}

				break;
			case TYPE_WARP:
				if (planet->state)
					planet->sprite.angle -= ToRadians(1);
				break;
			}
		}
	} foreach_end;

	// ボール 更新
	foreach_start(&g_stage.balls, GameObject, ball)
	{
		if (GameObject_IsAlive(ball))
		{
			GameObject_UpdatePosition(ball);

			// 減速
			//ball->vel.x *= 0.998f;
			//ball->vel.y *= 0.998f;

			// ワールドボーダー
			if (GameObject_Field_CollisionHorizontal(&g_field_ball, ball, CONNECTION_NONE, EDGESIDE_OUTER) ||
				GameObject_Field_CollisionVertical(&g_field_ball, ball, CONNECTION_NONE, EDGESIDE_OUTER))
			{
				VectorIterator_Remove(&itr_ball);
				continue;
			}

			switch (ball->type)
			{
			case TYPE_PARTICLE_BALL:
				screen_start(g_field_ball.sprite.texture.texture)
				{
					Vec2 before = Vec2_Sub(&ball->pos, &ball->vel);
					GameObject line = GameObject_CreateLine(before, ball->pos);
					line.sprite.color = ball->sprite.color;
					line.edge = 1;
					GameObject_Render(&line, &Vec2_Negate(&g_offset_shadow));
				} screen_end;
				break;
			}

			if (GameSpriteAnimation_Update(&ball->sprite) == ANIMATION_FINISHED && !ball->sprite.animation.loop_flag)
			{
				VectorIterator_Remove(&itr_ball);
				continue;
			}
		}
	} foreach_end;
	foreach_start(&g_stage.balls, GameObject, ball)
	{
		if (GameObject_IsAlive(ball))
		{
			switch (ball->type)
			{
			case TYPE_PARTICLE_BALL:
				if (gravity_exists && GetRand(100) == 0)
				{
					GameObject active = GetRandomParticleObject(TYPE_PARTICLE_GRAVITY, &ball->pos, 2);

					VectorIterator_Add(&itr_ball, &active);
				}
				break;
			}
		}
	} foreach_end;
}

// ステージ作成の更新: 機能のハンドラー
static void UpdateStageEdit_HandlePlanetControl(const Vec2* mouse, int id, int key, int type, BOOL unique)
{
	if ((IsMousePressed(MOUSE_INPUT_2) || IsMousePressed(MOUSE_INPUT_3)) && IsKeyDown(key))
	{
		g_mouse_last_from = *mouse;
		g_edit_mode = id;
		g_stage.edited = TRUE;
	}
	if ((IsMouseReleased(MOUSE_INPUT_2) || IsMouseReleased(MOUSE_INPUT_3)) && g_edit_mode == id)
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

// ステージ作成の更新
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
	if ((IsMousePressed(MOUSE_INPUT_2) || IsMousePressed(MOUSE_INPUT_3)) && IsKeyDown(KEY_INPUT_DELETE))
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

	// BGM
	if (IsKeyDown(KEY_INPUT_LCONTROL) && IsKeyPressed(KEY_INPUT_A))
	{
		// 新規ファイルの名前を入力する
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_STR, COLOR_WHITE);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_CURSOR, COLOR_WHITE);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_SELECT_STR, COLOR_WHITE);
		screen_start(DX_SCREEN_FRONT)
		{
			DrawFormatStringFToHandle(GameObject_GetX(&g_field, CENTER_X, -200), GameObject_GetY(&g_field, BOTTOM, -230),
				COLOR_YELLOW, g_resources.font_main, "BGM IDを入力してください (0 〜 %d)", NUM_BGM - 1);
		} screen_end;
		{
			int num = KeyInputNumber((int)GameObject_GetX(&g_field, CENTER_X, -200), (int)GameObject_GetY(&g_field, BOTTOM, -200), NUM_BGM - 1, 0, TRUE);
			if (0 <= num && num < NUM_BGM)
			{
				StopSoundMem(g_resources.sound_bgm[g_stage.bgm]);
				g_stage.bgm = num;
				PlaySoundMem(g_resources.sound_bgm[g_stage.bgm], DX_PLAYTYPE_LOOP);
			}
		}
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
		g_stage_screenshot = TRUE;
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
	Vec2 offset = Vec2_Add(&Vec2_Sub(&g_view.pos, &g_field.pos), &g_offset_shake.pos);
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
					RenderBalloon(&g_field, obj, &offset);
					break;
				case TYPE_START:
					GameObject_Render(obj, &offset);
					if (DEBUG_HITBOX)
						Vec2_Render(&obj->vel, &Vec2_Add(&obj->pos, &offset), obj->sprite.color);
					RenderBalloon(&g_field, obj, &offset);
					break;
				case TYPE_BEAM:
					GameObject_Render(obj, &offset);
					if (~g_tutorial_flag & TUTORIAL_FLAG_TYPE_BEAM && GameObject_IsHit(&mouseobj, obj))
						GameObject_Msg_Render(&Vec2_Add(&obj->pos, &Vec2_Create(0, -10)), &offset, "ビームは触れると消滅してしまう！");
					break;
				case TYPE_WARP:
					GameObject_Render(obj, &offset);
					//SetDrawBright(255 - 10, 255 - 140, 255 - 140);
					//GameObject_Render(obj, &Vec2_Add(&obj->vel, &offset));
					//SetDrawBright(255, 255, 255);
					{
						GameObject out = *obj;
						GameSprite_SetFrame(&out.sprite, 1);
						out.sprite.angle *= -1;
						out.pos = Vec2_Add(&out.pos, &obj->vel);
						GameObject_Render(&out, &offset);
					}
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
				{
					GameObject circle = *obj;
					circle.sprite = GameSprite_CreateNone();
					circle.sprite.color = 0x6bd0ff;
					circle.fill = TRUE;
					circle.shape = SHAPE_CIRCLE;
					circle.size = Vec2_Scale(&obj->size, GetEasingValueRange(ESG_LINEAR, GameTimer_GetProgress(&g_planet_circle_timer), 0, 6));
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)GetEasingValueRange(ESG_OUTQUAD, GameTimer_GetProgress(&g_planet_circle_timer), 192, 0));
					GameObject_Render(&circle, &offset);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
				GameObject_Render(obj, &offset);
				{
					GameObject mouseobj = GameObject_Create(mouse, Vec2_Create(), Vec2_Create(40, 40));
					mouseobj.shape = SHAPE_CIRCLE;
					if (GameObject_IsHit(&mouseobj, obj))
					{
						GameObject cursor = *obj;
						cursor.sprite = GameSprite_Create(GameTexture_Create(g_resources.texture[9], Vec2_Create(), Vec2_Create(18, 18)));
						cursor.sprite.num_columns = 2;
						GameSprite_SetFrame(&cursor.sprite, obj->state ? 0 : 1);
						GameObject_SetSize(&cursor, 4);
						GameObject_Render(&cursor, &offset);
					}
				}
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

	foreach_start(&g_stage.balls, GameObject, ball)
	{
		if (GameObject_IsAlive(ball))
		{
			switch (ball->type)
			{
			case TYPE_PARTICLE_GRAVITY:
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(ClampF(Vec2_Length(&ball->vel), 0, 1) * (sinf(GameTimer_GetRemaining(&ball->count) * 16) + 1) * 128));
				GameObject_Render(ball, &offset);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				break;
			default:
				GameObject_Render(ball, &offset);
				break;
			}
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
				DrawCircleAA(pos.x - g_offset_shadow.x, pos.y - g_offset_shadow.y, 6, 4, color);
			} screen_end;
		} foreach_end;
	}

	if (DEBUG_HITBOX)
	{
		int pos = 0;
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "デバッグ情報");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "stage: %s", g_selected_stageinfo.title);
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "all: %d", Vector_GetSize(&g_stage.balls));
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "score: %d", g_stage.score);
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "edited: %s", g_stage.edited ? "true" : "false");
		pos++;
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[S]+右クリック      - スタート地点 [Unique]");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[G]+右クリック      - ゴール地点 [Unique]");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[Q]+右クリック      - 惑星");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[B]+右クリック      - ビーム");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[R]+右クリック      - バウンスビーム");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[W]+右クリック      - ワープ");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[V]+右クリック      - ランチャー");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "[Delete]+右クリック - 撤去");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "Ctrl+[A]            - BGM変更");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "Ctrl+[O]            - ロード");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "Ctrl+[S]            - セーブ");
	}

	{
		g_back_button.fill = GameObject_IsHitPoint(&g_back_button, &g_raw_mouse);
		GameObject_Render(&g_back_button);
		DrawFormatStringToHandle((int)GameObject_GetX(&g_back_button, LEFT, -10), (int)GameObject_GetY(&g_back_button, TOP, -20), COLOR_WHITE, g_resources.font_main, "タイトルへ戻る");

		g_menu_button.fill = GameObject_IsHitPoint(&g_menu_button, &g_raw_mouse);
		GameObject_Render(&g_menu_button);
		DrawFormatStringToHandle((int)GameObject_GetX(&g_menu_button, LEFT, -10), (int)GameObject_GetY(&g_menu_button, TOP, -20), COLOR_WHITE, g_resources.font_main, "ポーズ");

		g_speed_button.fill = GameObject_IsHitPoint(&g_speed_button, &g_raw_mouse);
		GameObject_Render(&g_speed_button);
		DrawFormatStringToHandle((int)GameObject_GetX(&g_speed_button, LEFT, -10), (int)GameObject_GetY(&g_speed_button, TOP, -20), COLOR_WHITE, g_resources.font_main, "x%d", 1 << g_speed_expo);
	}

	if (g_stage_screenshot)
	{
		g_stage_screenshot = FALSE;
		screen_start(DX_SCREEN_BACK)
		{
			GameObject_Render(&g_backscreen);
			SaveDrawScreenToPNG((int)GameObject_GetX(&g_backscreen, LEFT), (int)GameObject_GetY(&g_backscreen, TOP),
				(int)GameObject_GetX(&g_backscreen, RIGHT), (int)GameObject_GetY(&g_backscreen, BOTTOM), g_selected_stageinfo.screenshot_filepath);
		} screen_end;
	}
}

// 場外吹き出し表示
static void RenderBalloon(const GameObject* field, const GameObject* obj, const Vec2* offset)
{
	GameObject hitrect = *field;
	hitrect.pos = Vec2_Sub(&field->pos, offset);
	if (!GameObject_IsHit(&hitrect, obj))
	{
		Vec2 planet_pos_disp = Vec2_Add(&obj->pos, offset);
		Vec2 vec = Vec2_Sub(&planet_pos_disp, &field->pos);
		Vec2 size = Vec2_Sub(&Vec2_Scale(&g_view.size, .5f), &Vec2_Create(40, 40));
		Vec2 trace = (size.y / size.x < GetAbsF(vec.y / vec.x)) ? Vec2_Scale(&vec, GetAbsF(size.y / vec.y)) : Vec2_Scale(&vec, GetAbsF(size.x / vec.x));
		Vec2 point = Vec2_Add(&field->pos, &trace);
		{
			Vec2 p1 = Vec2_Scale(&Vec2_Normalized(&trace), 50);
			Vec2 p2 = Vec2_Scale(&Vec2_Normalized(&Vec2_Rotate(&trace, ToRadians(90))), 20);
			Vec2 p3 = Vec2_Scale(&Vec2_Normalized(&Vec2_Rotate(&trace, ToRadians(-90))), 20);
			DrawTriangleAA(p1.x + point.x, p1.y + point.y, p2.x + point.x, p2.y + point.y, p3.x + point.x, p3.y + point.y, COLOR_WHITE, TRUE);
		}
		DrawCircleAA(point.x, point.y, 20, 20, COLOR_WHITE, TRUE);
		{
			GameObject disp = *obj;
			disp.pos = point;
			GameObject_SetSize(&disp, 1);
			GameObject_Render(&disp);
		}
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
	DeleteGraph(g_field_layer_top.sprite.texture.texture);
	foreach_start(&g_field_layers, GameObject, layer)
	{
		DeleteGraph(layer->sprite.texture.texture);
	} foreach_end;
	{
		int i;
		for (i = 0; i < NUM_BGM; i++)
			StopSoundMem(g_resources.sound_bgm[i]);
	}
	DeleteGraph(g_filter_screen.sprite.texture.texture);

	Vector_Delete(&g_field_layers);
	GameStage_Dispose(&g_stage);

	SetMainWindowText(GAME_TITLE);		// ウインドウタイトルの設定
}
