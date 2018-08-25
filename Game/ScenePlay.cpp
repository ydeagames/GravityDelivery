#include "ScenePlay.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Vector.h"
#include "GameUtils.h"


// 定数の定義 ==============================================================




// グローバル変数の定義 ====================================================

GameTimer g_count;
GameObject g_field;
GameObject g_field_ball;
GameObject g_view;

Vector g_balls;
Vector g_planets;
Vec2 g_goal;
Vec2 g_mouse_last_from;
Vec2 g_mouse_last_to;
int g_score;
float g_scale;
Vec2 g_offset_mouse;
Vec2 g_offset_location;



// 関数の宣言 ==============================================================

void InitializePlay(void);  // ゲームの初期化処理
void UpdatePlay(void);      // ゲームの更新処理
void RenderPlay(void);      // ゲームの描画処理
void FinalizePlay(void);    // ゲームの終了処理




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
	g_count = GameTimer_Create();
	GameTimer_SetRemaining(&g_count, .5f);
	GameTimer_Resume(&g_count);

	g_field = GameObject_Field_Create();
	g_field_ball = g_field;
	g_field_ball.size.x *= 2;
	g_field_ball.size.y *= 2;
	g_view = g_field;

	g_balls = Vector_Create(sizeof(GameObject));

	g_planets = Vector_Create(sizeof(GameObject));
	{
		int i;
		GameObject obj;
		obj = GameObject_Create(g_field.pos, Vec2_Create(), Vec2_Create(10, 10));
		obj.fill = TRUE;
		obj.state = 0;
		obj.sprite.color = COLOR_GRAY;
		Vector_AddLast(&g_planets, &obj);
		for (i = 0; i < 100; i++)
		{
			obj.pos = Vec2_Create(GetRandRangeF(GameObject_GetX(&g_field, LEFT), GameObject_GetX(&g_field, RIGHT)), GetRandRangeF(GameObject_GetY(&g_field, TOP), GameObject_GetY(&g_field, BOTTOM)));
			Vector_AddLast(&g_planets, &obj);
		}
	}

	g_goal = g_field.pos;
	g_mouse_last_from = g_field.pos;
	g_mouse_last_to = g_field.pos;

	g_score = 0;
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
		if (IsMousePressed(MOUSE_INPUT_2))
			g_mouse_last_from = GetMousePosition();
		if (IsMouseReleased(MOUSE_INPUT_2))
		{
			g_mouse_last_to = GetMousePosition();
		}
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

	if (IsMousePressed(MOUSE_INPUT_4))
		g_goal = GetMousePosition();

	foreach_start(&g_balls, GameObject, ball)
	{
		GameObject* nearest = NULL;
		float length2_min = -1;
		foreach_start(&g_planets, GameObject, planet)
		{
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

	if (GameTimer_IsFinished(&g_count))
	{
		GameTimer_SetRemaining(&g_count, .5f);
		GameTimer_Resume(&g_count);

		Vec2 vec = Vec2_Scale(&Vec2_Sub(&g_mouse_last_to, &g_mouse_last_from), .1f);
		GameObject obj = GameObject_Create(g_mouse_last_from, vec, Vec2_Create(5, 5));
		obj.fill = TRUE;
		Vector_AddLast(&g_balls, &obj);
	}

	foreach_start(&g_balls, GameObject, ball)
	{
		GameObject_UpdatePosition(ball);

		if (Vec2_LengthSquaredTo(&ball->pos, &g_goal) < 60 * 60)
		{
			VectorIterator_Remove(&itr_ball);
			g_score++;
			continue;
		}

		ball->vel.x *= 0.998f;
		ball->vel.y *= 0.998f;

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

	{
		Vec2 goal = Vec2_Add(&g_goal, &offset);
		DrawCircleAA(goal.x, goal.y, 60, 100, COLOR_RED, TRUE);
	}

	if (IsMouseDown(MOUSE_INPUT_2))
	{
		Vec2 pos = GetMousePosition();
		Vec2 vel = Vec2_Sub(&pos, &g_mouse_last_from);
		Vec2_Render(&vel, &Vec2_Add(&g_mouse_last_from, &offset), COLOR_WHITE);
	}
	else
	{
		Vec2 vel = Vec2_Sub(&g_mouse_last_to, &g_mouse_last_from);
		Vec2_Render(&vel, &Vec2_Add(&g_mouse_last_from, &offset), COLOR_WHITE);
	}
	foreach_start(&g_planets, GameObject, obj)
	{
		if (GameObject_IsAlive(obj))
			GameObject_Render(obj, &offset);
	} foreach_end;
	foreach_start(&g_balls, GameObject, obj)
	{
		if (GameObject_IsAlive(obj))
			GameObject_Render(obj, &offset);
	} foreach_end;

	DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP), COLOR_WHITE, "size: %d", Vector_GetSize(&g_balls));
	DrawFormatStringF(GameObject_GetX(&g_field, LEFT), GameObject_GetY(&g_field, TOP, -20), COLOR_WHITE, "score: %d", g_score);
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
}
