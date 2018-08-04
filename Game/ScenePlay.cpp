#include "ScenePlay.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Vector.h"
#include "GameUtils.h"


// 定数の定義 ==============================================================




// グローバル変数の定義 ====================================================

GameTimer g_count;
GameObject g_field;
Vector g_balls;
Vector g_planets;
Vec2 g_mouse_last_from;
Vec2 g_mouse_last_to;



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
	if (IsMousePressed(MOUSE_INPUT_2))
		g_mouse_last_from = GetMousePosition();
	if (IsMouseReleased(MOUSE_INPUT_2))
	{
		g_mouse_last_to = GetMousePosition();
	}
	if (IsMousePressed(MOUSE_INPUT_1))
	{
		Vec2 mouse = GetMousePosition();
		foreach_start(&g_planets, GameObject, obj)
		{
			if (Vec2_LengthSquaredTo(&mouse, &obj->pos) < Vec2_LengthSquared(&obj->size))
			{
				obj->state = !obj->state;
				obj->sprite.color = obj->state ? COLOR_WHITE : COLOR_GRAY;
			}
		} foreach_end;
	}

	foreach_start(&g_planets, GameObject, planet)
	{
		if (planet->state)
		{
			foreach_start(&g_balls, GameObject, ball)
			{
				float length2 = Vec2_LengthSquaredTo(&ball->pos, &planet->pos);
				if (20 * 20 < length2 && length2 < 400 * 400)
					ball->vel = Vec2_Add(&ball->vel, &Vec2_Scale(&Vec2_Normalized(&Vec2_Sub(&planet->pos, &ball->pos)), 20 / Vec2_LengthTo(&planet->pos, &ball->pos)));
			} foreach_end;
		}
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

		GameObject_Field_CollisionHorizontal(&g_field, ball, CONNECTION_NONE, EDGESIDE_OUTER);
		GameObject_Field_CollisionVertical(&g_field, ball, CONNECTION_NONE, EDGESIDE_OUTER);
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
	if (IsMouseDown(MOUSE_INPUT_2))
	{
		Vec2 pos = GetMousePosition();
		Vec2 vel = Vec2_Sub(&pos, &g_mouse_last_from);
		Vec2_Render(&vel, &g_mouse_last_from, COLOR_WHITE);
	}
	else
	{
		Vec2 vel = Vec2_Sub(&g_mouse_last_to, &g_mouse_last_from);
		Vec2_Render(&vel, &g_mouse_last_from, COLOR_WHITE);
	}
	foreach_start(&g_planets, GameObject, obj)
	{
		if (GameObject_IsAlive(obj))
			GameObject_Render(obj);
	} foreach_end;
	foreach_start(&g_balls, GameObject, obj)
	{
		if (GameObject_IsAlive(obj))
			GameObject_Render(obj);
	} foreach_end;
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
