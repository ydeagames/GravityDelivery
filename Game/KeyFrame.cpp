#include "KeyFrame.h"
#include "GameUtils.h"

// �ϐ��̐錾 ==============================================================

// <�L�[�t���[���쐬>
KeyFrame KeyFrame_Create(float time, Easings easing_forward)
{
	return { easing_forward, ESG_LINEAR, FALSE, GameTimer_Create(), time, FALSE };
}

// <�Ώ�Easing���g���ăL�[�t���[���쐬>
KeyFrame KeyFrame_CreateWith2Easings(float time, Easings easing_forward, Easings easing_back)
{
	return { easing_forward, easing_back, TRUE, GameTimer_Create(), time, FALSE };
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

// <�L�[�t���[���i���擾>
float KeyFrame_GetProgress(KeyFrame* keyframe, float max)
{
	float progress = GameTimer_IsFinished(&keyframe->timer) ? 1 : 1 - GameTimer_GetRemaining(&keyframe->timer) / keyframe->time;
	float easingprogress;
	if (keyframe->state)
		easingprogress = GetEasingValue(keyframe->easing_forward, progress, 1);
	else
	{
		if (keyframe->two_way)
			easingprogress = 1 - GetEasingValue(keyframe->easing_back, 1 - progress, 1);
		else
			easingprogress = 1 - GetEasingValue(keyframe->easing_forward, progress, 1);
	}
	//DrawFormatStringF(300, 300, COLOR_WHITE, "%f", easingprogress); // Debug
	return GetPercentValue(easingprogress, max);
}

// <�L�[�t���[���i���擾>
float KeyFrame_GetProgressRange(KeyFrame* keyframe, float min, float max)
{
	float easingprogress = KeyFrame_GetProgress(keyframe, max - min);
	if (max < min)
	{
		easingprogress = (max - min) - easingprogress;
		SwapF(&min, &max);
	}
	return easingprogress + min;
}
