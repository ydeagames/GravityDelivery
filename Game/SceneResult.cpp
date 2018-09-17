#include "SceneTitle.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameMain.h"
#include "GameObject.h"
#include "GameUtils.h"
#include "Easings.h"

// 定数の定義 ==============================================================

// グローバル変数の定義 ====================================================

static GameObject g_back_button;

// 関数の定義 ==============================================================

// タイトルシーンの初期化処理
void InitializeResult(void)
{
	{
		Vec2 size = Vec2_Create(150, 50);
		g_back_button = GameObject_Create(Vec2_Create(GameObject_GetX(&g_field, LEFT, -size.x / 2 - 50), GameObject_GetY(&g_field, BOTTOM, -size.y / 2 - 50)), Vec2_Create(), size);
		g_back_button.fill = TRUE;
		g_back_button.sprite.color = COLOR_GRAY;
	}
}

// タイトルシーンの更新処理
void UpdateResult(void)
{
	//if (IsKeyPressed(PAD_INPUT_2))
	//	RequestScene(SCENE_TITLE);

	if (IsMousePressed(MOUSE_INPUT_1))
	{
		if (GameObject_IsHitPoint(&g_back_button, &GetMousePosition()))
		{
			RequestScene(SCENE_TITLE, COLOR_GRAY, .5f);
			PlaySoundMem(g_resources.sound_se[5], DX_PLAYTYPE_BACK);
		}
	}
	if (GameObject_IsHitPoint(&g_back_button, &g_raw_mouse) && !GameObject_IsHitPoint(&g_back_button, &g_raw_mouse_last))
		PlaySoundMem(g_resources.sound_se[1], DX_PLAYTYPE_BACK);
}

// タイトルシーンの描画処理
void RenderResult(void)
{
	int pos = 0;
	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "ステージクリア");
	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "%s", g_selected_stage.filename);

	{
		if (GameObject_IsHitPoint(&g_back_button, &GetMousePosition()))
			GameObject_Render(&g_back_button);
		DrawFormatStringF(GameObject_GetX(&g_back_button, LEFT, -10), GameObject_GetY(&g_back_button, TOP, -20), COLOR_WHITE, "タイトルへ戻る");
	}
}

// タイトルシーンの終了処理
void FinalizeResult(void)
{
}