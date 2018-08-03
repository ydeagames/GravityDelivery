#include "SceneManager.h"
#include <assert.h>

#include "SceneLogo.h"
#include "SceneTitle.h"
#include "ScenePlay.h"
#include "SceneResult.h"

// グローバル変数の定義 ====================================================

SceneID g_active_scene;
SceneID g_next_scene;

// 関数の宣言 ==============================================================

void InitializeScene(SceneID scene_id);
void UpdateScene(SceneID scene_id);
void RenderScene(SceneID scene_id);
void FinalizeScene(SceneID scene_id);

// 関数の定義 ==============================================================

// <シーンの初期化処理>
void InitializeSceneManager(SceneID start_scene_id)
{
	g_active_scene = SCENE_NONE;
	g_next_scene = start_scene_id;
}

// <シーンの更新処理>
void UpdateSceneManager(void)
{
	if (g_next_scene != SCENE_NONE)
	{
		FinalizeScene(g_active_scene);
		InitializeScene(g_next_scene);
		g_active_scene = g_next_scene;
		g_next_scene = SCENE_NONE;
	}

	UpdateScene(g_active_scene);
}

// <シーンの描画処理>
void RenderSceneManager(void)
{
	RenderScene(g_active_scene);
}

// <シーンの終了処理>
void FinalizeSceneManager(void)
{
	FinalizeScene(g_active_scene);
}

// シーン要求
void RequestScene(SceneID scene_id)
{
	g_next_scene = scene_id;
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
