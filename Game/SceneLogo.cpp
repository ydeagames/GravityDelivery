#include "SceneLogo.h"
#include "GameMain.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameUtils.h"

// 定数の定義 ==============================================================

// グローバル変数の定義 ====================================================

int g_logo_count;

// 関数の定義 ==============================================================

// ロゴシーンの初期化処理
void InitializeLogo(void)
{
	g_logo_count = 0;

	// 動画再生
	PlayMovieToGraph(g_resources.movie_logo);
}

// ロゴシーンの更新処理
void UpdateLogo(void)
{
	if (IsKeyPressed(PAD_INPUT_2) || g_logo_count++ >= 60 * 3.5f)
		RequestScene(SCENE_TITLE);
}

// ロゴシーンの描画処理
void RenderLogo(void)
{
	// ロゴムービー描画
	{
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)GetPercentValue(1 - GetPercentageRange((float)g_logo_count, 60 * 3, 60 * 3.5f), 255));
			DrawExtendGraph(SCREEN_LEFT, SCREEN_TOP, SCREEN_WIDTH, SCREEN_HEIGHT, g_resources.movie_logo, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)GetPercentValue(1 - GetPercentageRange((float)g_logo_count, 60 * 0, 60 * 0.5f), 255));
			DrawBox(SCREEN_LEFT, SCREEN_TOP, SCREEN_RIGHT, SCREEN_BOTTOM, COLOR_BLACK, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	//DrawFormatString(10, 10, COLOR_WHITE, "ロゴシーン");
	//DrawFormatString(10, 25, COLOR_WHITE, "カウント: %3d / 180", g_logo_count);
}

// ロゴシーンの終了処理
void FinalizeLogo(void)
{
}