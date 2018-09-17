//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.h
//!
//! @brief  ゲーム関連のヘッダファイル
//!
//! @date   日付
//!
//! @author 制作者名
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 ==================================================
#pragma once

#include "GameResource.h"
#include "DebugConsole.h"




// 定数の定義 ==============================================================

// <システム> ----------------------------------------------------------
#define GAME_TITLE "重力スイッチ"              // ゲームタイトル


// <画面> --------------------------------------------------------------
#define SCREEN_WIDTH    (1600)                  // 画面の幅[pixel]
#define SCREEN_HEIGHT   (800)                  // 画面の高さ[pixel]

#define SCREEN_TOP      (0)                    // 画面の上端
#define SCREEN_BOTTOM   (SCREEN_HEIGHT)        // 画面の下端
#define SCREEN_LEFT     (0)                    // 画面の左端
#define SCREEN_RIGHT    (SCREEN_WIDTH)         // 画面の右端

#define SCREEN_CENTER_X (SCREEN_WIDTH  / 2)    // 画面の中央(X座標)
#define SCREEN_CENTER_Y (SCREEN_HEIGHT / 2)    // 画面の中央(Y座標)




// 構造体の定義 ============================================================

typedef struct {
	char filename[260];
	char filepath[260];
} Stage;




// グローバル変数の宣言 ====================================================

extern GameResource g_resources;
extern Stage g_selected_stage;
extern GameObject g_field;
extern DebugConsole g_console;




// 関数の宣言 ==============================================================

// ゲームの初期化処理
void InitializeGame(void);

// ゲームの更新処理
void UpdateGame(void);

// ゲームの描画処理
void RenderGame(void);

// ゲームの終了処理
void FinalizeGame(void);
