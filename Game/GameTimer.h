#pragma once

// 構造体の宣言 ============================================================

// <ゲームタイマー>>
typedef struct
{
	int start_time;	// 開始時間
	int last_time;	// 終了時間
	BOOL paused;	// 一時停止
	float remaining;// 終了時間
} GameTimer;

// 定数の定義 ==============================================================

#define GAME_DURATION_SECONDS 60		// ゲームの制限時間

// 関数の宣言 ==============================================================

// <<タイマー>> --------------------------------------------------

// <タイマー作成>
GameTimer GameTimer_Create(void);

// <タイマー時間>
float GameTimer_GetTime(GameTimer* timer);

// <タイマー一時停止中>
BOOL GameTimer_IsPaused(GameTimer* timer);

// <タイマー停止>
void GameTimer_Pause(GameTimer* timer);

// <タイマーリセット>
void GameTimer_Reset(GameTimer* timer);

// <タイマー再開>
void GameTimer_Resume(GameTimer* timer);

// <タイマーセット>
void GameTimer_Set(GameTimer* timer, float new_time);


// <タイマー残り時間セット>
void GameTimer_SetRemaining(GameTimer* timer, float remaining);

// <ゲーム時間制限でタイマー残り時間セット>
void GameTimer_SetRemainingDefault(GameTimer* timer);

// <タイマー残り時間リセット>
void GameTimer_ResetRemaining(GameTimer* timer);

// <トータル時間取得>
float GameTimer_GetTotal(GameTimer* timer);

// <進捗取得>
float GameTimer_GetProgress(GameTimer* timer);

// <タイマー残り時間>
float GameTimer_GetRemaining(GameTimer* timer);

// <タイマー経過時間>
float GameTimer_GetElapsed(GameTimer* timer);

// <タイマー終了判定>
BOOL GameTimer_IsFinished(GameTimer* timer);
