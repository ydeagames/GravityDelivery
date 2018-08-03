#include "GameTimer.h"

// 定数の定義 ==============================================================

#define RESOLUTION 1000					// ミリ秒→秒

// 関数の定義 ==============================================================

// <<タイマー>> --------------------------------------------------

// <タイマー作成>
GameTimer GameTimer_Create(void)
{
	GameTimer timer = { -1, -1, TRUE };
	// タイマーをリセット
	GameTimer_Reset(&timer);
	return timer;
}

// <タイマー時間>
float GameTimer_GetTime(GameTimer* timer)
{
	// 一時停止されてなければ更新する
	if (!timer->paused)
		timer->last_time = GetNowCount() - timer->start_time;
	return (float)timer->last_time / RESOLUTION;
}

// <タイマー一時停止中>
BOOL GameTimer_IsPaused(GameTimer* timer)
{
	return timer->paused;
}

// <タイマー停止>
void GameTimer_Pause(GameTimer* timer)
{
	// 一時停止されてなければ更新する
	if (!timer->paused)
		timer->last_time = GetNowCount() - timer->start_time;
	timer->paused = TRUE;
}

// <タイマーリセット>
void GameTimer_Reset(GameTimer* timer)
{
	GameTimer_Set(timer, 0);
}

// <タイマー再開>
void GameTimer_Resume(GameTimer* timer)
{
	// 一時停止されていれば再開開始時刻を計算して更新
	if (timer->paused)
		timer->start_time = GetNowCount() - timer->last_time;
	timer->paused = FALSE;
}

// <タイマーセット>
void GameTimer_Set(GameTimer* timer, float new_time)
{
	// ミリ秒
	int new_time_in_ticks = (int)(new_time * RESOLUTION);
	// 開始時刻を計算
	timer->start_time = GetNowCount() - new_time_in_ticks;
	// 終了時刻を計算
	timer->last_time = new_time_in_ticks;
}

// <タイマー残り時間セット>
void GameTimer_SetRemaining(GameTimer* timer, float remaining)
{
	GameTimer_Set(timer, -remaining);
}

// <ゲーム時間制限でタイマー残り時間セット>
void GameTimer_SetRemainingDefault(GameTimer* timer)
{
	GameTimer_SetRemaining(timer, GAME_DURATION_SECONDS);
}

// <タイマー残り時間>
float GameTimer_GetTimeRemaining(GameTimer* timer)
{
	return -GameTimer_GetTime(timer);
}

// <タイマー終了判定>
BOOL GameTimer_IsFinished(GameTimer* timer)
{
	return GameTimer_GetTime(timer) >= 0;
}
