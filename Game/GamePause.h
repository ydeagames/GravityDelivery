#pragma once
#include "GameObject.h"

// グローバル変数の宣言 ====================================================

extern GameObject g_backscreen;

// 関数の宣言 ==============================================================

// ポーズの初期化処理
void InitializeGamePause(void);

// ポーズの更新処理
void UpdateGamePause(void);

// ポーズの描画処理
void RenderGamePause(void);

// ポーズの終了処理
void FinalizeGamePause(void);

// ポーズ要求
void RequestPause(void);

// ポーズ中か
BOOL IsGamePaused(void);