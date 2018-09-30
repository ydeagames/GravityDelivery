#include "SceneManager.h"
#include <assert.h>

#include "SceneLogo.h"
#include "SceneTitle.h"
#include "ScenePlay.h"
#include "SceneResult.h"

#include "GameMain.h"
#include "GameTimer.h"
#include "GameUtils.h"

// 列挙の定義 ==============================================================

typedef enum {
	FADE_IDLE = 0,
	FADE_OUT,
	FADE_IN,
	FADE_FINISHED
} FadeState;

// グローバル変数の定義 ====================================================

BOOL g_close_request = FALSE;

SceneID g_active_scene;
SceneID g_next_scene;

static GameTimer g_fade_timer;
static FadeState g_fade_phase;
static float g_fade_endtime;
static unsigned int g_fade_color;

// 関数の宣言 ==============================================================

void InitializeScene(SceneID scene_id);
void UpdateScene(SceneID scene_id);
void RenderScene(SceneID scene_id);
void FinalizeScene(SceneID scene_id);

// 関数の定義 ==============================================================

// <シーンの初期化処理>
void InitializeSceneManager(SceneID start_scene_id)
{
	g_close_request = FALSE;

	g_active_scene = SCENE_NONE;
	g_next_scene = start_scene_id;

	g_fade_timer = GameTimer_Create();
	g_fade_phase = FADE_OUT;
	g_fade_endtime = 0;
}

// <シーンの更新処理>
void UpdateSceneManager(void)
{
	if (g_next_scene != SCENE_NONE)
	{
		switch (g_fade_phase)
		{
		case FADE_OUT:
			if (GameTimer_IsFinished(&g_fade_timer))
			{
				GameTimer_SetRemaining(&g_fade_timer, g_fade_endtime);
				g_fade_phase = FADE_IN;

				FinalizeScene(g_active_scene);
				InitializeScene(g_next_scene);
				g_active_scene = g_next_scene;
			}
			break;
		case FADE_IN:
			if (GameTimer_IsFinished(&g_fade_timer))
			{
				GameTimer_SetRemaining(&g_fade_timer, 0);
				GameTimer_Pause(&g_fade_timer);
				g_fade_phase = FADE_FINISHED;
			}
			break;
		case FADE_FINISHED:
			g_fade_phase = FADE_IDLE;

			g_active_scene = g_next_scene;
			g_next_scene = SCENE_NONE;
			break;
		}
	}

	UpdateScene(g_active_scene);
}

// <シーンの描画処理>
void RenderSceneManager(void)
{
	RenderScene(g_active_scene);

	switch (g_fade_phase)
	{
	case FADE_OUT:
	{
		float opacity = GameTimer_GetProgress(&g_fade_timer);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(opacity * 255));
		DrawBox(SCREEN_LEFT, SCREEN_TOP, SCREEN_RIGHT, SCREEN_BOTTOM, g_fade_color, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	}
	case FADE_IN:
	{
		float opacity = 1 - GameTimer_GetProgress(&g_fade_timer);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(opacity * 255));
		DrawBox(SCREEN_LEFT, SCREEN_TOP, SCREEN_RIGHT, SCREEN_BOTTOM, g_fade_color, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	}
	}
}

// <シーンの終了処理>
void FinalizeSceneManager(void)
{
	FinalizeScene(g_active_scene);
}

// シーン要求
void RequestScene(SceneID scene_id, unsigned int color, float time, float endtime)
{
	if (g_fade_phase == FADE_IDLE)
	{
		g_next_scene = scene_id;

		GameTimer_SetRemaining(&g_fade_timer, time);
		GameTimer_Resume(&g_fade_timer);
		g_fade_phase = FADE_OUT;
		g_fade_color = color;
		if (endtime >= 0)
			g_fade_endtime = endtime;
		else
			g_fade_endtime = time;
	}
}

// 終了要求
void RequestClose(void)
{
	g_close_request = TRUE;
}

// シーンを初期化
void InitializeScene(SceneID scene_id)
{
	switch (scene_id)
	{
	case SCENE_NONE: break;
	case SCENE_LOGO: InitializeLogo(); break;
	case SCENE_TITLE: InitializeTitle(); break;
	case SCENE_PLAY: InitializePlay(); break;
	case SCENE_RESULT: InitializeResult(); break;
	default:
		assert(FALSE && "Unknown Scene in Initialize");
	}
}

// シーンを更新
void UpdateScene(SceneID scene_id)
{
	switch (scene_id)
	{
	case SCENE_NONE: break;
	case SCENE_LOGO: UpdateLogo(); break;
	case SCENE_TITLE: UpdateTitle(); break;
	case SCENE_PLAY: UpdatePlay(); break;
	case SCENE_RESULT: UpdateResult(); break;
	default:
		assert(FALSE && "Unknown Scene in Update");
	}
}

// シーンを描画
void RenderScene(SceneID scene_id)
{
	switch (scene_id)
	{
	case SCENE_NONE: break;
	case SCENE_LOGO: RenderLogo(); break;
	case SCENE_TITLE: RenderTitle(); break;
	case SCENE_PLAY: RenderPlay(); break;
	case SCENE_RESULT: RenderResult(); break;
	default:
		assert(FALSE && "Unknown Scene in Render");
	}
}

// シーンの終了処理
void FinalizeScene(SceneID scene_id)
{
	switch (scene_id)
	{
	case SCENE_NONE: break;
	case SCENE_LOGO: FinalizeLogo(); break;
	case SCENE_TITLE: FinalizeTitle(); break;
	case SCENE_PLAY: FinalizePlay(); break;
	case SCENE_RESULT: FinalizeResult(); break;
	default:
		assert(FALSE && "Unknown Scene in Finalize");
	}
}
