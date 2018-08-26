//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.cpp
//!
//! @brief  ゲーム関連のソースファイル
//!
//! @date   日付
//!
//! @author 制作者名
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================

#include "GameMain.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "GameResource.h"



// 定数の定義 ==============================================================




// グローバル変数の定義 ====================================================

GameResource g_resources;
Stage g_selected_stage;
GameObject g_field;
DebugConsole g_console;



// 関数の宣言 ==============================================================

void InitializeGame(void);  // ゲームの初期化処理
void UpdateGame(void);      // ゲームの更新処理
void RenderGame(void);      // ゲームの描画処理
void FinalizeGame(void);    // ゲームの終了処理




// 関数の定義 ==============================================================

//----------------------------------------------------------------------
//! @brief ゲームの初期化処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void InitializeGame(void)
{
	//SetUpMemoryLeakDetector();

	// ファンクションキー
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_11, KEY_INPUT_F3);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_12, KEY_INPUT_F5);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_13, KEY_INPUT_F6);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_14, KEY_INPUT_F7);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_15, KEY_INPUT_F8);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_16, KEY_INPUT_F9);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_17, KEY_INPUT_F10);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_18, KEY_INPUT_F11);

	// リソースを読み込み
	g_resources = GameResource_Create();

	// 画面範囲
	g_field = GameObject_Field_Create();

	// デバッグコンソールを初期化
	{
		Vec2 size = Vec2_Create(200, 200);
		GameObject area = GameObject_Create(Vec2_Create(GameObject_GetX(&g_field, LEFT, -size.x / 2), GameObject_GetY(&g_field, BOTTOM, -size.y / 2)), Vec2_Create(), size);

		g_console = DebugConsole_Create(area, g_resources.font_menu, 20);
	}

	// カーソルを非表示
	SetMouseDispFlag(FALSE);

	// シーンマネージャーを初期化
	InitializeSceneManager(SCENE_LOGO);
}



//----------------------------------------------------------------------
//! @brief ゲームの更新処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void UpdateGame(void)
{
	// 入力マネージャーを更新
	UpdateInputManager();
	// ティックの更新
	GameTick_Update();

	// F11キーが押されていたら
	if (IsKeyPressed(PAD_INPUT_11))
	{
		// デバッグモードをトグル
		extern BOOL g_debug_mode;
		g_debug_mode = !g_debug_mode;
	}

	// シーンマネージャーを更新
	UpdateSceneManager();
}




//----------------------------------------------------------------------
//! @brief ゲームの描画処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void RenderGame(void)
{
	// シーンマネージャーを描画
	RenderSceneManager();

	// デバッグコンソールを描画
	DebugConsole_Render(&g_console);

	// カーソルを描画
	{
		Vec2 point = GetMousePosition();
		DrawTriangleAA(point.x, point.y, point.x + 20, point.y + 10, point.x + 10, point.y + 20, COLOR_GREEN, TRUE);
	}
}



//----------------------------------------------------------------------
//! @brief ゲームの終了処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void FinalizeGame(void)
{
	// シーンマネージャーをファイナライズ
	FinalizeSceneManager();

	// リソースをアンロード
	GameResource_Delete(&g_resources);
}
