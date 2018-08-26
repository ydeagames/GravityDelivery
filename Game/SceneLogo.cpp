#include "SceneLogo.h"
#include "GameMain.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameUtils.h"

// 定数の定義 ==============================================================

// グローバル変数の定義 ====================================================

static GameTimer g_logo_count;

// 関数の定義 ==============================================================

// ロゴシーンの初期化処理
void InitializeLogo(void)
{
	g_logo_count = GameTimer_Create();
	GameTimer_SetRemaining(&g_logo_count, 3.f);
	GameTimer_Resume(&g_logo_count);

	// 動画再生
	PlayMovieToGraph(g_resources.movie_logo);
}

// ロゴシーンの更新処理
void UpdateLogo(void)
{
	if (IsMousePressed(MOUSE_INPUT_1) || GameTimer_IsFinished(&g_logo_count))
		RequestScene(SCENE_TITLE);
}

// ロゴシーンの描画処理
void RenderLogo(void)
{
	// ロゴムービー描画
	{
		{
			int bright = (int) GetMinF(
				GetPercentValue(1 - GetPercentageRange(GameTimer_GetRemaining(&g_logo_count), .5f, 0.f), 255),
				GetPercentValue(GetPercentageRange(GameTimer_GetElapsed(&g_logo_count), 0.f, .5f), 255)
			);
			SetDrawBright(bright, bright, bright);
			DrawBox(SCREEN_LEFT, SCREEN_TOP, SCREEN_RIGHT, SCREEN_BOTTOM, COLOR_WHITE, TRUE);
			DrawRotaGraph(SCREEN_CENTER_X, SCREEN_CENTER_Y, 1, 0, g_resources.movie_logo, TRUE);
			SetDrawBright(255, 255, 255);
		}
	}

	//DrawFormatString(10, 10, COLOR_WHITE, "ロゴシーン");
	//DrawFormatString(10, 25, COLOR_WHITE, "カウント: %3d / 180", g_logo_count);
}

// ロゴシーンの終了処理
void FinalizeLogo(void)
{
}