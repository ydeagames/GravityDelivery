#include "SceneTitle.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameMain.h"
#include "GameObject.h"
#include "GameUtils.h"
#include "Easings.h"

// 定数の定義 ==============================================================

// グローバル変数の定義 ====================================================

static int g_count;

// 関数の定義 ==============================================================

// タイトルシーンの初期化処理
void InitializeResult(void)
{
	g_count = 0;
}

// タイトルシーンの更新処理
void UpdateResult(void)
{
	g_count++;

	if (IsKeyPressed(PAD_INPUT_2))
		RequestScene(SCENE_TITLE);
}

// タイトルシーンの描画処理
void RenderResult(void)
{
	int bright = (int)GetEasingValue(ESG_INCUBIC, GetPercentageRange((float)g_count, 60 * 0, 60 * 0.5f), 255);		// フェードイン
	SetDrawBright(bright, bright, bright);
	{
		DrawRotaGraph(SCREEN_CENTER_X, SCREEN_CENTER_Y, 1, 0, g_resources.texture_result, TRUE);
		{
			int duration = 90;
			int idle = 30;

			DrawRotaGraph(SCREEN_CENTER_X, SCREEN_CENTER_Y + 96, 1, 0, g_resources.texture_result_text, TRUE);
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA,
					(int)GetEasingValueRange(ESG_INCIRC, GetPercentageRange((float)(g_count % (duration + idle)), 0, (float)duration), 255, 32));
				DrawRotaGraph(SCREEN_CENTER_X, SCREEN_CENTER_Y + 96,
					GetEasingValueRange(ESG_LINEAR, GetPercentageRange((float)(g_count % (duration + idle)), 0, (float)duration), 1.1f, 1.f),
					0, g_resources.texture_result_text, TRUE
				);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}

		//DrawFormatString(10, 10, COLOR_WHITE, "タイトルシーン");
		//DrawFormatString(10, 25, COLOR_WHITE, "カウント: %3d / 180", g_result_count);

		{
			GameObject box = GameObject_Field_Create();
			box.sprite.color = COLOR_BLACK;
			box.pos.y += 5;
			box.size.x -= 100;
			box.size.y = 110;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
			GameObject_Render(&box);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	SetDrawBright(255, 255, 255);
}

// タイトルシーンの終了処理
void FinalizeResult(void)
{
}