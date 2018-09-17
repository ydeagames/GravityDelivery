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



// 関数の宣言 ==============================================================

void InitializePlay(void);  // ゲームの初期化処理
void UpdatePlay(void);      // ゲームの更新処理
void RenderPlay(void);      // ゲームの描画処理
void FinalizePlay(void);    // ゲームの終了処理

static void LoadStage(void);
static void SaveStage(void);




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

	LoadStage();
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
	Vec2 offset;

	{
		if (IsMousePressed(MOUSE_INPUT_2))
		{
			g_offset_mouse = GetMousePosition();
			g_offset_location = g_view.pos;
		}
		if (IsMouseDown(MOUSE_INPUT_2))
		{
			Vec2 diff = Vec2_Sub(&GetMousePosition(), &g_offset_mouse);
			g_view.pos = Vec2_Add(&g_offset_location, &diff);
		}

		GameObject_Field_CollisionVertical(&g_field_ball, &g_view, CONNECTION_BARRIER, EDGESIDE_INNER);
		GameObject_Field_CollisionHorizontal(&g_field_ball, &g_view, CONNECTION_BARRIER, EDGESIDE_INNER);
	}

	offset = Vec2_Sub(&g_view.pos, &g_field.pos);

	{
		if (IsMousePressed(MOUSE_INPUT_3) && IsKeyDown(PAD_INPUT_12))
		{
			g_mouse_last_from = GetMousePosition();
			g_edit_mode = 1;
			g_edited = TRUE;
		}
		if (IsMouseReleased(MOUSE_INPUT_3) && g_edit_mode == 1)
		{
			Vec2 mouse_last_to = GetMousePosition();
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
	if (IsMousePressed(MOUSE_INPUT_3) && IsKeyDown(PAD_INPUT_13))
	{
		Vec2 mouse = Vec2_Sub(&GetMousePosition(), &offset);
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
		Vec2 mouse = Vec2_Sub(&GetMousePosition(), &offset);
		GameObject obj = GameObject_Planet_Create(&mouse);
		Vector_AddLast(&g_planets, &obj);
		g_edit_mode = 3;
		g_edited = TRUE;
	}
	if (IsMousePressed(MOUSE_INPUT_3) && IsKeyDown(PAD_INPUT_15))
	{
		Vec2 mouse = Vec2_Sub(&GetMousePosition(), &offset);
		foreach_start(&g_planets, GameObject, obj)
		{
			if (GameObject_IsAlive(obj))
			{
				switch (obj->type)
				{
				case TYPE_PLANET:
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

	if (IsMousePressed(MOUSE_INPUT_1))
	{
		Vec2 mouse = Vec2_Sub(&GetMousePosition(), &offset);
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
						GameSprite_SetFrame(&obj->sprite, obj->state ? 12 : 9);
					}
				}
			}
		} foreach_end;
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
				case TYPE_GOAL:
					if (GameObject_IsHit(ball, planet))
					{
						VectorIterator_Remove(&itr_ball);
						g_score++;
					}

					break;
				}
			}
		} foreach_end;

		if (nearest != NULL)
			ball->vel = Vec2_Add(&ball->vel, &Vec2_Scale(&Vec2_Normalized(&Vec2_Sub(&nearest->pos, &ball->pos)), 20 / Vec2_LengthTo(&nearest->pos, &ball->pos)));
	} foreach_end;

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

	foreach_start(&g_planets, GameObject, planet)
	{
		if (GameObject_IsAlive(planet))
		{
			switch (planet->type)
			{
			case TYPE_START:
				if (GameTimer_IsFinished(&planet->count))
				{
					GameTimer_SetRemaining(&planet->count, .5f);
					GameTimer_Resume(&planet->count);

					Vec2 vec = Vec2_Scale(&planet->vel, .1f);
					GameObject obj = GameObject_Create(planet->pos, vec, Vec2_Create(5, 5));
					obj.fill = TRUE;
					Vector_AddLast(&g_balls, &obj);
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
		if (GameObject_IsHitPoint(&g_back_button, &GetMousePosition()))
		{
			RequestScene(SCENE_TITLE);
		}
	}

	if (g_score >= 10 && !g_edited)
		RequestScene(SCENE_RESULT);
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

	GameObject_Field_Render(&g_field_ball, &offset);

	foreach_start(&g_planets, GameObject, obj)
	{
		if (GameObject_IsAlive(obj))
		{
			switch (obj->type)
			{
			case TYPE_GOAL:
				DrawFormatStringF(GameObject_GetX(obj, LEFT) + offset.x, GameObject_GetY(obj, BOTTOM, 10) + offset.y, COLOR_WHITE, "%d / 10", g_score);
				GameObject_Render(obj, &offset);
				break;
			case TYPE_PLANET:
				GameObject_Render(obj, &offset);
				break;
			case TYPE_START:
				Vec2_Render(&obj->vel, &Vec2_Add(&obj->pos, &offset), obj->sprite.color);
				break;
			}
		}
	} foreach_end;
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

	if (DEBUG_HITBOX)
	{
		int pos = 0;
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "デバッグ情報 (F3-ヒットボックス F5-スタート地点 F6-ゴール地点 F7-惑星設置 F8-惑星撤去 F9-新規作成 F10-ロード F11-セーブ)");
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "stage: %s", g_selected_stage.filename);
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "all: %d", Vector_GetSize(&g_balls));
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "score: %d", g_score);
		DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20.f * pos++), COLOR_GRAY, "edited: %s", g_edited ? "true" : "false");
	}

	{
		if (GameObject_IsHitPoint(&g_back_button, &GetMousePosition()))
			GameObject_Render(&g_back_button);
		DrawFormatStringF(GameObject_GetX(&g_back_button, LEFT, -10), GameObject_GetY(&g_back_button, TOP, -20), COLOR_WHITE, "タイトルへ戻る");
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
	Vector_Delete(&g_balls);
	Vector_Delete(&g_planets);
}
