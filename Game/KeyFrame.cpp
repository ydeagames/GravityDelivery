#include "KeyFrame.h"

// 変数の宣言 ==============================================================

// <キーフレーム作成>
KeyFrame KeyFrame_Create(float time, Easings easing_forward)
{
	GameTimer timer = GameTimer_Create();
	GameTimer_SetRemaining(&timer, time);
	return { easing_forward, ESG_LINEAR, FALSE, time, FALSE };
}

KeyFrame KeyFrame_CreateWith2Easings(float time, Easings easing_forward, Easings easing_back)
{
	GameTimer timer = GameTimer_Create();
	GameTimer_SetRemaining(&timer, time);
	return { easing_forward, easing_back, TRUE, time, FALSE };
}

// <キーフレーム状態変更>
void KeyFrame_SetState(KeyFrame* keyframe, BOOL state)
{
	if (keyframe->state != state)
	{
		if (GameTimer_IsFinished(&keyframe->timer))
			GameTimer_SetRemaining(&keyframe->timer, keyframe->time);
		else
		{
			float remaining = GameTimer_GetRemaining(&keyframe->timer);
			GameTimer_SetRemaining(&keyframe->timer, keyframe->time - remaining);
		}
		GameTimer_Resume(&keyframe->timer);
		keyframe->state = state;
	}
}

// <キーフレーム進捗取得>
float KeyFrame_GetProgress(KeyFrame* keyframe)
{
	float progress = 1 - GameTimer_GetRemaining(&keyframe->timer) / keyframe->time;
	if (keyframe->state)
		return GetEasingValue(keyframe->easing_forward, progress, 1);
	else
	{
		if (keyframe->two_way)
			return 1 - GetEasingValue(keyframe->easing_back, 1 - progress, 1);
		else
			return 1 - GetEasingValue(keyframe->easing_forward, progress, 1);
	}
}
