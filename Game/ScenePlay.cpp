#include "ScenePlay.h"
#include "GameMain.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Vector.h"
#include "GameUtils.h"
#include "GameObjects.h"
#include "SceneManager.h"
#include <assert.h>


// 定数の定義 ==============================================================




// グローバル変数の定義 ====================================================

static GameObject g_field_ball;
static Vector g_field_layers;
static GameTimer g_field_layer_timer;
static GameObject g_view;

static GameObject g_back_button;

static Vector g_balls;
static Vector g_planets;

static int g_score;
static int g_edit_mode;
static BOOL g_edited;

static Vec2 g_mouse_last_from;
static Vec2 g_offset_mouse;
static Vec2 g_offset_location;

static BOOL g_mouse_on_last;
static BOOL g_mouse_down;

static HSND g_bgm;

static int g_tutorial_state = 0;



// 関数の宣言 ==============================================================

void InitializePlay(void);  // ゲームの初期化処理
void UpdatePlay(void);      // ゲームの更新処理
void RenderPlay(void);      // ゲームの描画処理
void FinalizePlay(void);    // ゲームの終了処理

static void LoadStage(void);
static void SaveStage(void);

static BOOL collisionLineCircle(const GameObject* circle, Vec2 pointA, Vec2 pointB);



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
	g_field_ball.size.x *= 2;
	g_field_ball.size.y *= 2;
	g_field_ball.sprite = GameSprite_Create(GameTexture_Create(MakeScreen((int)g_field_ball.size.x, (int)g_field_ball.size.y, TRUE), Vec2_Create(), g_field_ball.size));
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
	g_edited = FALSE;

	g_balls = Vector_Create(sizeof(GameObject));
	g_planets = Vector_Create(sizeof(GameObject));

	g_score = 0;

	g_offset_mouse = Vec2_Create();
	g_mouse_on_last = FALSE;
	g_mouse_down = FALSE;

	g_bgm = 0;

	LoadStage();

	if (0 <= g_bgm && g_bgm < NUM_BGM)
	{
		ChangeVolumeSoundMem(100, g_resources.sound_bgm[g_bgm]);
		PlaySoundMem(g_resources.sound_bgm[g_bgm], DX_PLAYTYPE_LOOP);
	}
}

static void LoadStage(void)
{
	FILE* fp;
	char* fname = g_selected_stage.filepath;

	errno_t err = fopen_s(&fp, fname, "r");

	//assert(err == 0 && "file not opened!");
	if (err == 0)
	{
		int type;
		float pos_x, pos_y;
		float vel_x, vel_y;
		float scale;
		int color;

		Vector_Clear(&g_balls);
		Vector_Clear(&g_planets);

		fscanf_s(fp, "%d", &g_bgm);
		while (fscanf_s(fp, "%d %f %f %f %f %f %d", &type, &pos_x, &pos_y, &vel_x, &vel_y, &scale, &color) != EOF) {
			Vec2 pos = Vec2_Create(pos_x, pos_y);
			Vec2 vel = Vec2_Create(vel_x, vel_y);
			GameObject obj = GameObject_Type_Create(type, &pos, &vel);
			//GameObject_SetSize(&obj, scale);

			Vector_AddLast(&g_planets, &obj);
		}

		fclose(fp);
	}
}

static void SaveStage(void)
{
	FILE* fp;
	char* fname = g_selected_stage.filepath;

	errno_t err = fopen_s(&fp, fname, "w");
	assert(err == 0 && "file not opened!");

	{
		fprintf_s(fp, "%d", g_bgm);

		foreach_start(&g_planets, GameObject, obj)
		{
			fprintf_s(fp, "%d %f %f %f %f %f %d\n", obj->type, obj->pos.x, obj->pos.y, obj->vel.x, obj->vel.y, obj->sprite.scale, obj->sprite.color);
		} foreach_end;

		fclose(fp);
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

	offset = Vec2_Sub(&g_view.pos, &g_field.pos);
	mouse = Vec2_Sub(&g_raw_mouse, &offset);
	mouse_last = Vec2_Sub(&g_raw_mouse_last, &offset);

	{
		float current_parallax = .2f;
		float parallax = .8f;
		Vec2 mouse_offset = Vec2_Scale(&mouse, -.02f);
		offset = Vec2_Add(&offset, &mouse_offset);
		mouse = Vec2_Sub(&g_raw_mouse, &offset);
	}

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

	{
		if (IsMousePressed(MOUSE_INPUT_3) && IsKeyDown(PAD_INPUT_12))
		{
			g_mouse_last_from = g_raw_mouse;
			g_edit_mode = 1;
			g_edited = TRUE;
		}
		if (IsMouseReleased(MOUSE_INPUT_3) && g_edit_mode == 1)
		{
			Vec2 mouse_last_to = g_raw_mouse;
			GameObject obj = GameObject_Start_Create(&g_mouse_last_from, &Vec2_Sub(&mouse_last_to, &g_mouse_last_from));

			foreach_start(&g_planets, GameObject, planet)
			{
				if (planet->type == TYPE_START)
					VectorIterator_Remove(&itr_planet);
			} foreach_end;

			Vector_AddLast(&g_planets, &obj);
			g_edit_mode = -1;
		}
	}
	{
		if (IsMousePressed(MOUSE_INPUT_3) && IsKeyDown(PAD_INPUT_16))
		{
			g_mouse_last_from = g_raw_mouse;
			g_edit_mode = 5;
			g_edited = TRUE;
		}
		if (IsMouseReleased(MOUSE_INPUT_3) && g_edit_mode == 5)
		{
			Vec2 mouse_last_to = g_raw_mouse;
			GameObject obj = GameObject_Beam_Create(&g_mouse_last_from, &mouse_last_to);

			Vector_AddLast(&g_planets, &obj);
			g_edit_mode = -1;
		}
	}
	if (IsMousePressed(MOUSE_INPUT_3) && IsKeyDown(PAD_INPUT_13))
	{
		GameObject obj = GameObject_Goal_Create(&mouse);

		foreach_start(&g_planets, GameObject, planet)
		{
			if (planet->type == TYPE_GOAL)
				VectorIterator_Remove(&itr_planet);
		} foreach_end;

		Vector_AddLast(&g_planets, &obj);
		g_edit_mode = 2;
		g_edited = TRUE;
	}
	if (IsMousePressed(MOUSE_INPUT_3) && IsKeyDown(PAD_INPUT_14))
	{
		GameObject obj = GameObject_Planet_Create(&mouse);
		Vector_AddLast(&g_planets, &obj);
		g_edit_mode = 3;
		g_edited = TRUE;
	}
	if (IsMousePressed(MOUSE_INPUT_3) && IsKeyDown(PAD_INPUT_15))
	{
		foreach_start(&g_planets, GameObject, obj)
		{
			if (GameObject_IsAlive(obj))
			{
				switch (obj->type)
				{
				case TYPE_PLANET:
				case TYPE_BEAM:
					if (Vec2_LengthSquaredTo(&mouse, &obj->pos) < Vec2_LengthSquared(&obj->size))
					{
						VectorIterator_Remove(&itr_obj);
					}
				}
			}
		} foreach_end;
		g_edit_mode = 4;
		g_edited = TRUE;
	}
	if (IsKeyPressed(PAD_INPUT_17))
	{
		LoadStage();
		DebugConsole_Log(&g_console, "stage loaded!");
		g_edited = FALSE;
	}
	if (IsKeyPressed(PAD_INPUT_18))
	{
		SaveStage();
		DebugConsole_Log(&g_console, "stage saved!");
		g_edited = FALSE;
	}

	if (IsMouseReleased(MOUSE_INPUT_1))
	{
		Vec2 diff = Vec2_Sub(&g_raw_mouse, &g_offset_mouse);
		if (Vec2_LengthSquared(&diff) < 5 * 5)
		{
			foreach_start(&g_planets, GameObject, obj)
			{
				if (GameObject_IsAlive(obj))
				{
					switch (obj->type)
					{
					case TYPE_PLANET:
						if (Vec2_LengthSquaredTo(&mouse, &obj->pos) < Vec2_LengthSquared(&obj->size))
						{
							obj->state = !obj->state;
							//GameSprite_SetFrame(&obj->sprite, obj->state ? 12 : 9);
							GameObject_SetSize(obj, obj->state ? 4.f : 2.f, 8);
							if (g_tutorial_state == 0)
								g_tutorial_state = 1;
							PlaySoundMem(g_resources.sound_se[2], DX_PLAYTYPE_BACK);
						}
					}
				}
			} foreach_end;
		}
		else if (g_tutorial_state == 1)
			g_tutorial_state = 2;
	}

	foreach_start(&g_balls, GameObject, ball)
	{
		GameObject* nearest = NULL;
		float length2_min = -1;

		foreach_start(&g_planets, GameObject, planet)
		{
			if (GameObject_IsAlive(planet))
			{
				switch (planet->type)
				{
				case TYPE_PLANET:
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

						if (GameObject_IsHit(planet, ball))
							VectorIterator_Remove(&itr_ball);
					}

					break;
				case TYPE_BEAM:
					if (collisionLineCircle(ball, planet->pos, planet->vel))
						VectorIterator_Remove(&itr_ball);
					break;
				case TYPE_GOAL:
					if (GameObject_IsHit(ball, planet))
					{
						VectorIterator_Remove(&itr_ball);
						g_score++;
						ChangeVolumeSoundMem(100, g_resources.sound_se[0]);
						if (g_tutorial_state == 2)
							g_tutorial_state = 3;
						PlaySoundMem(g_resources.sound_se[0], DX_PLAYTYPE_BACK);
					}

					break;
				}
			}
		} foreach_end;

		if (nearest != NULL)
			ball->vel = Vec2_Add(&ball->vel, &Vec2_Scale(&Vec2_Normalized(&Vec2_Sub(&nearest->pos, &ball->pos)), .1f));
	} foreach_end;

	/*
	foreach_start(&g_balls, GameObject, ball1)
	{
		foreach_start(&g_balls, GameObject, ball2)
		{
			if (ball1 != ball2)
				if (GameObject_IsHit(ball1, ball2))
				{
					VectorIterator_Remove(&itr_ball1);
					break;
				}
		} foreach_end;
	} foreach_end;
	/**/

	foreach_start(&g_planets, GameObject, planet)
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
						Vector_AddLast(&g_balls, &obj);
					}
				}

				break;
			}
		}
	} foreach_end;

	foreach_start(&g_balls, GameObject, ball)
	{
		GameObject_UpdatePosition(ball);

		//ball->vel.x *= 0.998f;
		//ball->vel.y *= 0.998f;

		if (GameObject_Field_CollisionHorizontal(&g_field_ball, ball, CONNECTION_NONE, EDGESIDE_OUTER) ||
			GameObject_Field_CollisionVertical(&g_field_ball, ball, CONNECTION_NONE, EDGESIDE_OUTER))
			VectorIterator_Remove(&itr_ball);
	} foreach_end;

	if (IsMousePressed(MOUSE_INPUT_1))
	{
		if (GameObject_IsHitPoint(&g_back_button, &g_raw_mouse))
		{
			RequestScene(SCENE_TITLE, COLOR_GRAY, .5f);
			PlaySoundMem(g_resources.sound_se[5], DX_PLAYTYPE_BACK);
		}
	}
	if (GameObject_IsHitPoint(&g_back_button, &g_raw_mouse) && !GameObject_IsHitPoint(&g_back_button, &g_raw_mouse_last))
		PlaySoundMem(g_resources.sound_se[1], DX_PLAYTYPE_BACK);

	{
		BOOL mouse_on = FALSE;
		foreach_start(&g_planets, GameObject, obj)
		{
			if (GameObject_IsAlive(obj))
			{
				switch (obj->type)
				{
				case TYPE_PLANET:
					if (Vec2_LengthSquaredTo(&mouse, &obj->pos) < Vec2_LengthSquared(&obj->size))
						mouse_on = TRUE;
				}
			}
		} foreach_end;
		if (mouse_on && !g_mouse_on_last)
			PlaySoundMem(g_resources.sound_se[1], DX_PLAYTYPE_BACK);
		g_mouse_on_last = mouse_on;
	}

	if (g_score >= 10 && !g_edited)
	{
		foreach_start(&g_planets, GameObject, obj)
		{
			if (GameObject_IsAlive(obj))
			{
				switch (obj->type)
				{
				case TYPE_GOAL:
					obj->sprite = GameSprite_Create(GameTexture_Create(g_resources.texture_planet1, Vec2_Create(), Vec2_Create(26, 26)));
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

static BOOL collisionLineCircle(const GameObject* circle, Vec2 pointA, Vec2 pointB)
{
	//ベクトルを生成
	Vec2 vecAB = Vec2_Sub(&pointB, &pointA);
	Vec2 vecAP = Vec2_Sub(&circle->pos, &pointA);
	Vec2 vecBP = Vec2_Sub(&circle->pos, &pointB);

	//ABの単位ベクトルを計算
	Vec2 normalAB = Vec2_Normalized(&vecAB);

	//AからXまでの距離を
	//単位ベクトルABとベクトルAPの内積で求める
	float lenAX = Vec2_Dot(&normalAB, &vecAP);

	float shortestDistance;  //線分APとPの最短距離
	if (lenAX < 0) {
		//AXが負なら APが円の中心までの最短距離
		shortestDistance = Vec2_Length(&vecAP);
	}
	else if (lenAX > Vec2_Length(&vecAB)) {
		//AXがAPよりも長い場合は、BPが円の中心
		//までの最短距離
		shortestDistance = Vec2_Length(&vecBP);
	}
	else {
		//PがAB上にあるので、PXが最短距離
		//単位ベクトルABとベクトルAPの外積で求める
		shortestDistance = GetAbsF(Vec2_Cross(&normalAB, &vecAP));
	}

	{
		//Xの座標を求める(AXの長さより計算）
		Vec2 pointX = Vec2_Create(pointA.x + (normalAB.x * lenAX),
			pointA.y + (normalAB.y * lenAX));
		float r1 = GetMinF(circle->size.x, circle->size.y) / 2;

		BOOL hit = FALSE;
		if (shortestDistance < r1) {
			//最短距離が円の半径よりも小さい場合は、当たり
			hit = TRUE;
		}
		return hit;
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
			GameObject_Field_Render(layer, &Vec2_Scale(&Vec2_Add(&offset, &mouse_offset), current_parallax), 1);
		} foreach_end;
	}
	GameObject_Field_Render(&g_field_ball, &offset, 3);

	{
		BOOL first_planet = TRUE;
		foreach_start(&g_planets, GameObject, obj)
		{
			if (GameObject_IsAlive(obj))
			{
				switch (obj->type)
				{
				case TYPE_GOAL:
					DrawFormatStringToHandle((int) (GameObject_GetX(obj, LEFT) + offset.x + 2), (int) (GameObject_GetY(obj, BOTTOM, 10) + offset.y), COLOR_WHITE, g_resources.font_main, "%d / 10", g_score);
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
					DrawLineAA(obj->pos.x + offset.x, obj->pos.y + offset.y, obj->vel.x + offset.x, obj->vel.y + offset.y, obj->sprite.color, 3);
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

	foreach_start(&g_balls, GameObject, obj)
	{
		if (GameObject_IsAlive(obj))
		{
			SetDrawScreen(g_field_ball.sprite.texture.texture);
			{
				Vec2 before = Vec2_Sub(&obj->pos, &obj->vel);
				DrawLineAA(before.x - offset_shadow.x, before.y - offset_shadow.y, obj->pos.x - offset_shadow.x, obj->pos.y - offset_shadow.y, obj->sprite.color);
			}
			SetDrawScreen(DX_SCREEN_BACK);
			GameObject_Render(obj, &offset);
		}
	} foreach_end;
	if (GameTimer_IsFinished(&g_field_layer_timer))
	{
		GameTimer_SetRemaining(&g_field_layer_timer, .3f);
		GameTimer_Resume(&g_field_layer_timer);

		foreach_start(&g_field_layers, GameObject, layer)
		{
			SetDrawScreen(layer->sprite.texture.texture);
			{
				unsigned int color = GetColor(GetRand(255), GetRand(255), GetRand(255));
				Vec2 pos = Vec2_Create(GetRandRangeF(GameObject_GetX(layer, LEFT), GameObject_GetX(layer, RIGHT)),
					GetRandRangeF(GameObject_GetY(layer, TOP), GameObject_GetY(layer, BOTTOM)));
				DrawCircleAA(pos.x - offset_shadow.x, pos.y - offset_shadow.y, 2, 4, color);
			}
			SetDrawScreen(DX_SCREEN_BACK);
		} foreach_end;
	}
	foreach_start(&g_planets, GameObject, obj)
	{
		if (GameObject_IsAlive(obj))
		{
			switch (obj->type)
			{
			case TYPE_PLANET:
				if (Vec2_LengthSquaredTo(&mouse, &obj->pos) < Vec2_LengthSquared(&obj->size))
				{
					GameObject cursor = *obj;
					cursor.sprite = GameSprite_Create(GameTexture_Create(g_resources.texture_cursor1, Vec2_Create(), Vec2_Create(18, 18)));
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
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "デバッグ情報 (F5-スタート地点 F6-ゴール地点 F7-惑星設置 F8-惑星撤去 F9-ビーム F10-ロード F11-セーブ)");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "stage: %s", g_selected_stage.filename);
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "all: %d", Vector_GetSize(&g_balls));
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "score: %d", g_score);
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "edited: %s", g_edited ? "true" : "false");
	}

	{
		if (GameObject_IsHitPoint(&g_back_button, &g_raw_mouse))
			GameObject_Render(&g_back_button);
		DrawFormatStringToHandle((int) GameObject_GetX(&g_back_button, LEFT, -10), (int) GameObject_GetY(&g_back_button, TOP, -20), COLOR_WHITE, g_resources.font_main, "タイトルへ戻る");
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
	Vector_Delete(&g_balls);
	Vector_Delete(&g_planets);
}
