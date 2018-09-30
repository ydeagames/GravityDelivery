#pragma once
#include "Easings.h"

// 列挙型の定義 ============================================================

// <シーン> ------------------------------------------------------------

enum tag_SceneID
{
	SCENE_NONE = -1,
	SCENE_LOGO,
	SCENE_TITLE,
	SCENE_PLAY,
	SCENE_RESULT,
	SCENE_UNKNOWN = 100
};
typedef enum tag_SceneID SceneID;

// グローバル変数の宣言 ====================================================

extern BOOL g_close_request;

// 関数の宣言 ==============================================================

// シーンの初期化処理
void InitializeSceneManager(SceneID start_scene_id);

// シーンの更新処理
void UpdateSceneManager(void);

// シーンの描画処理
void RenderSceneManager(void);

// シーンの終了処理
void FinalizeSceneManager(void);

// シーン要求
void RequestScene(SceneID scene_id, unsigned int color = COLOR_WHITE, float time = 0.f, float endtime = -1.f, Easings easeFadeOut = ESG_OUTSINE, Easings easeFadeIn = ESG_INSINE);

// 終了要求
void RequestClose(void);

// シーン
SceneID GetSceneID(void);
