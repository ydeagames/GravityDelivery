#pragma once
#include "Easings.h"
#include "GameTimer.h"

// 構造体の定義 ============================================================

// <キーフレーム>
typedef struct {
	GameTimer timer;
	float time;
	BOOL state;
} KeyFrame;

// <キーフレーム>
typedef struct {
	Easings easing_forward;
	Easings easing_back;
	BOOL two_way;
	KeyFrame* keyframe;
} EasingKeyFrame;

// 関数の宣言 ==============================================================

// <キーフレーム作成>
KeyFrame KeyFrame_Create(float time);

// <キーフレーム状態変更>
void KeyFrame_SetState(KeyFrame* keyframe, BOOL state);

// <キーフレーム進捗取得>
float KeyFrame_GetProgress(KeyFrame* keyframe, float max = 1);

// <キーフレーム進捗取得>
float KeyFrame_GetProgressRange(KeyFrame* keyframe, float min = 0, float max = 1);

// <キーフレーム作成>
EasingKeyFrame EasingKeyFrame_Create(KeyFrame* keyframe, Easings easing_forward);

// <対象Easingを使ってキーフレーム作成>
EasingKeyFrame EasingKeyFrame_CreateWith2Easings(KeyFrame* keyframe, Easings easing_forward, Easings easing_back);

// <キーフレーム進捗取得>
float EasingKeyFrame_GetProgress(EasingKeyFrame* keyframe, float max = 1);

// <キーフレーム進捗取得>
float EasingKeyFrame_GetProgressRange(EasingKeyFrame* keyframe, float min = 0, float max = 1);
