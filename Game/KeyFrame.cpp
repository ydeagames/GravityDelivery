#include "KeyFrame.h"
#include "GameUtils.h"

// �֐��̒�` ==============================================================

// <�L�[�t���[���쐬>
KeyFrame KeyFrame_Create(float time)
{
	return { GameTimer_Create(), time, FALSE };
}

// <�L�[�t���[����ԕύX>
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

// <�L�[�t���[���i���p�[�Z���g�擾>
static float KeyFrame_GetProgressPercentage(KeyFrame* keyframe)
{
	float progress = GameTimer_IsFinished(&keyframe->timer) ? 1 : 1 - GameTimer_GetRemaining(&keyframe->timer) / keyframe->time;
	return keyframe->state ? progress : 1 - progress;
}

// <�L�[�t���[���i���擾>
float KeyFrame_GetProgress(KeyFrame* keyframe, float max)
{
	return GetPercentValue(KeyFrame_GetProgressPercentage(keyframe), max);
}

// <�L�[�t���[���i���擾>
float KeyFrame_GetProgressRange(KeyFrame* keyframe, float min, float max)
{
	return GetPercentValueRange(KeyFrame_GetProgressPercentage(keyframe), min, max);
}

// <�L�[�t���[���쐬>
EasingKeyFrame EasingKeyFrame_Create(KeyFrame* keyframe, Easings easing_forward)
{
	return { easing_forward, ESG_LINEAR, FALSE, keyframe };
}

// <�Ώ�Easing���g���ăL�[�t���[���쐬>
EasingKeyFrame EasingKeyFrame_CreateWith2Easings(KeyFrame* keyframe, Easings easing_forward, Easings easing_back)
{
	return { easing_forward, easing_back, TRUE, keyframe };
}

// <�L�[�t���[���i���p�[�Z���g�擾>
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

// <�L�[�t���[���i���擾>
float EasingKeyFrame_GetProgress(EasingKeyFrame* keyframe, float max)
{
	return GetPercentValue(EasingKeyFrame_GetProgressPercentage(keyframe), max);
}

// <�L�[�t���[���i���擾>
float EasingKeyFrame_GetProgressRange(EasingKeyFrame* keyframe, float min, float max)
{
	return GetPercentValueRange(EasingKeyFrame_GetProgressPercentage(keyframe), min, max);
}
