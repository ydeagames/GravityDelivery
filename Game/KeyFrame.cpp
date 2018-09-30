#include "KeyFrame.h"
#include "GameUtils.h"

// 関数の定義 ==============================================================

// <キーフレーム作成>
KeyFrame KeyFrame_Create(float time)
{
	return { GameTimer_Create(), time, FALSE };
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

// <キーフレーム進捗パーセント取得>
static float KeyFrame_GetProgressPercentage(KeyFrame* keyframe)
{
	float progress = GameTimer_IsFinished(&keyframe->timer) ? 1 : 1 - GameTimer_GetRemaining(&keyframe->timer) / keyframe->time;
	return keyframe->state ? progress : 1 - progress;
}

// <キーフレーム進捗取得>
float KeyFrame_GetProgress(KeyFrame* keyframe, float max)
{
	return GetPercentValue(KeyFrame_GetProgressPercentage(keyframe), max);
}

// <キーフレーム進捗取得>
float KeyFrame_GetProgressRange(KeyFrame* keyframe, float min, float max)
{
	return GetPercentValueRange(KeyFrame_GetProgressPercentage(keyframe), min, max);
}

// <キーフレーム作成>
EasingKeyFrame EasingKeyFrame_Create(KeyFrame* keyframe, Easings easing_forward)
{
	return { easing_forward, ESG_LINEAR, FALSE, keyframe };
}

// <対象Easingを使ってキーフレーム作成>
EasingKeyFrame EasingKeyFrame_CreateWith2Easings(KeyFrame* keyframe, Easings easing_forward, Easings easing_back)
{
	return { easing_forward, easing_back, TRUE, keyframe };
}

// <キーフレーム進捗パーセント取得>
static float EasingKeyFrame_GetProgressPercentage(EasingKeyFrame* keyframe)
{
	float progress = KeyFrame_GetProgressPercentage(keyframe->keyframe);
	if (keyframe->keyframe->state)
		return GetEasingValue(keyframe->easing_forward, progress, 1);
	else
	{
		if (keyframe->two_way)
			return 1 - GetEasingValue(keyframe->easing_back, 1 - progress, 1);
		else
			return GetEasingValue(keyframe->easing_forward, progress, 1);
	}
}

// <キーフレーム進捗取得>
float EasingKeyFrame_GetProgress(EasingKeyFrame* keyframe, float max)
{
	return GetPercentValue(EasingKeyFrame_GetProgressPercentage(keyframe), max);
}

// <キーフレーム進捗取得>
float EasingKeyFrame_GetProgressRange(EasingKeyFrame* keyframe, float min, float max)
{
	return GetPercentValueRange(EasingKeyFrame_GetProgressPercentage(keyframe), min, max);
}
