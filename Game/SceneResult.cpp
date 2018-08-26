#include "SceneTitle.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameMain.h"
#include "GameObject.h"
#include "GameUtils.h"
#include "Easings.h"

// 定数の定義 ==============================================================

// グローバル変数の定義 ====================================================

// 関数の定義 ==============================================================

// タイトルシーンの初期化処理
void InitializeResult(void)
{
}

// タイトルシーンの更新処理
void UpdateResult(void)
{
	if (IsKeyPressed(PAD_INPUT_2))
		RequestScene(SCENE_TITLE);
}

// タイトルシーンの描画処理
void RenderResult(void)
{
	int pos = 0;
	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "ステージクリア");
	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "%s", g_selected_stage.filename);
}

// タイトルシーンの終了処理
void FinalizeResult(void)
{
}