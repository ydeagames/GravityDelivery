#pragma once
#include "Easings.h"
#include "GameTimer.h"

// 構造体の定義 ============================================================

// <キーフレーム>
typedef struct {
	Easings easing_forward;
	Easings easing_back;
	BOOL two_way;
	GameTimer timer;
	float time;
	BOOL state
} KeyFrame;

// 変数の宣言 ==============================================================

// <キーフレーム作成>
KeyFrame KeyFrame_Create(float time, Easings easing_forward, Easings easing_back);

// <キーフレーム状態変更>
void KeyFrame_SetState(KeyFrame* keyframe, BOOL state);

// <キーフレーム進捗取得>
float KeyFrame_GetProgress(KeyFrame* keyframe);
