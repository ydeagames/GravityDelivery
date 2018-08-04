#include "ScenePlay.h"
#include "GameObject.h"
#include "InputManager.h"


// 定数の定義 ==============================================================




// グローバル変数の定義 ====================================================

GameObject g_ball;
Vec2 g_mouse_last;



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
	if (IsMousePressed(MOUSE_INPUT_1))
		g_mouse_last = GetMousePosition();
	if (IsMouseReleased(MOUSE_INPUT_1))
	{
		Vec2 vel = Vec2_Scale(&Vec2_Sub(&g_mouse_last, &GetMousePosition()), .1f);
		g_ball = GameObject_Create(g_mouse_last, vel, Vec2_Create(5, 5));
		g_ball.fill = TRUE;
	}

	GameObject_UpdatePosition(&g_ball);
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
	if (IsMouseDown(MOUSE_INPUT_1))
	{
		Vec2 pos = GetMousePosition();
		Vec2 vel = Vec2_Sub(&g_mouse_last, &pos);
		Vec2_Render(&vel, &pos, COLOR_WHITE);
	}
	if (GameObject_IsAlive(&g_ball))
		GameObject_Render(&g_ball);
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
