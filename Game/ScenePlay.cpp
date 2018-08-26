#include "ScenePlay.h"
#include "GameMain.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Vector.h"
#include "GameUtils.h"
#include "GameObjects.h"
#include <assert.h>


// 定数の定義 ==============================================================




// グローバル変数の定義 ====================================================

GameObject g_field;
GameObject g_field_ball;
GameObject g_view;

int mode;

Vector g_balls;
Vector g_planets;

int g_score;
Vec2 g_mouse_last_from;
Vec2 g_offset_mouse;
Vec2 g_offset_location;




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
	g_field = GameObject_Field_Create();
	g_field_ball = g_field;
	g_field_ball.size.x *= 2;
	g_field_ball.size.y *= 2;
	g_view = g_field;

	g_balls = Vector_Create(sizeof(GameObject));
	g_planets = Vector_Create(sizeof(GameObject));

	g_score = 0;

	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_12, KEY_INPUT_F5);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_13, KEY_INPUT_F6);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_14, KEY_INPUT_F7);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_15, KEY_INPUT_F8);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_17, KEY_INPUT_F10);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_18, KEY_INPUT_F11);

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
			GameObject obj = GameObject_Create(pos, vel, Vec2_Create(10, 10));
			obj.shape = SHAPE_CIRCLE;
			obj.type = type;
			obj.fill = TRUE;
			obj.state = 0;
			obj.sprite.color = color;
			GameObject_Bullet_SetSize(&obj, scale);

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
		if (IsMousePressed(MOUSE_INPUT_3))
		{
			g_offset_mouse = GetMousePosition();
			g_offset_location = g_view.pos;
		}
		if (IsMouseDown(MOUSE_INPUT_3))
		{
			Vec2 diff = Vec2_Sub(&GetMousePosition(), &g_offset_mouse);
			g_view.pos = Vec2_Add(&g_offset_location, &diff);
		}

		GameObject_Field_CollisionVertical(&g_field_ball, &g_view, CONNECTION_BARRIER, EDGESIDE_INNER);
		GameObject_Field_CollisionHorizontal(&g_field_ball, &g_view, CONNECTION_BARRIER, EDGESIDE_INNER);
	}

	offset = Vec2_Sub(&g_view.pos, &g_field.pos);

	{
		if (IsMousePressed(MOUSE_INPUT_2) && IsKeyDown(PAD_INPUT_12))
		{
			g_mouse_last_from = GetMousePosition();
			mode = 1;
		}
		if (IsMouseReleased(MOUSE_INPUT_2) && mode == 1)
		{
			Vec2 mouse_last_to = GetMousePosition();
			GameObject obj = GameObject_Create(g_mouse_last_from, Vec2_Sub(&mouse_last_to, &g_mouse_last_from), Vec2_Create(10, 10));
			obj.type = TYPE_START;
			GameObject_Bullet_SetSize(&obj, 1);
			GameTimer_SetRemaining(&obj.count, .5f);
			GameTimer_Resume(&obj.count);

			foreach_start(&g_planets, GameObject, planet)
			{
				if (planet->type == TYPE_START)
					VectorIterator_Remove(&itr_planet);
			} foreach_end;

			Vector_AddLast(&g_planets, &obj);
			mode = -1;
		}
	}
	if (IsMousePressed(MOUSE_INPUT_2) && IsKeyDown(PAD_INPUT_13))
	{
		Vec2 mouse = Vec2_Sub(&GetMousePosition(), &offset);
		GameObject obj = GameObject_Create(mouse, Vec2_Create(), Vec2_Create(60, 60));
		obj.type = TYPE_GOAL;
		obj.fill = TRUE;
		obj.shape = SHAPE_CIRCLE;
		obj.sprite.color = COLOR_RED;
		GameObject_Bullet_SetSize(&obj, 6);

		foreach_start(&g_planets, GameObject, planet)
		{
			if (planet->type == TYPE_GOAL)
				VectorIterator_Remove(&itr_planet);
		} foreach_end;

		Vector_AddLast(&g_planets, &obj);
		mode = 2;
	}
	if (IsMousePressed(MOUSE_INPUT_2) && IsKeyDown(PAD_INPUT_14))
	{
		Vec2 mouse = Vec2_Sub(&GetMousePosition(), &offset);
		GameObject obj = GameObject_Create(mouse, Vec2_Create(), Vec2_Create(10, 10));
		obj.type = TYPE_PLANET;
		obj.fill = TRUE;
		obj.shape = SHAPE_CIRCLE;
		obj.state = 0;
		obj.sprite.color = COLOR_GRAY;
		GameObject_Bullet_SetSize(&obj, 1);
		Vector_AddLast(&g_planets, &obj);
		mode = 3;
	}
	if (IsMousePressed(MOUSE_INPUT_2) && IsKeyDown(PAD_INPUT_15))
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
		mode = 4;
	}
	if (IsKeyPressed(PAD_INPUT_17))
	{
		LoadStage();
		DebugConsole_Log(&g_console, "stage loaded!");
	}
	if (IsKeyPressed(PAD_INPUT_18))
	{
		SaveStage();
		DebugConsole_Log(&g_console, "stage saved!");
	}

	if (IsMousePressed(MOUSE_INPUT_1))
	{
		Vec2 mouse = Vec2_Sub(&GetMousePosition(), &offset);
		foreach_start(&g_planets, GameObject, obj)
		{
			if (Vec2_LengthSquaredTo(&mouse, &obj->pos) < Vec2_LengthSquared(&obj->size))
			{
				obj->state = !obj->state;
				obj->sprite.color = obj->state ? COLOR_WHITE : COLOR_GRAY;
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

	foreach_start(&g_planets, GameObject, obj)
	{
		if (GameObject_IsAlive(obj))
		{
			switch (obj->type)
			{
			case TYPE_GOAL:
				DrawFormatStringF(GameObject_GetX(obj, LEFT), GameObject_GetY(obj, BOTTOM, 10), COLOR_WHITE, "%d / 10", g_score);
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
			GameObject_Render(obj, &offset);
	} foreach_end;

	DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -0), COLOR_WHITE, "デバッグ情報");
	DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20), COLOR_WHITE, "stage: %s", g_selected_stage.filename);
	DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -40), COLOR_WHITE, "size: %d", Vector_GetSize(&g_balls));
	DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -60), COLOR_WHITE, "score: %d", g_score);
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
