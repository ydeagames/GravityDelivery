#pragma once
#include "Easings.h"
#include "GameTimer.h"

// �\���̂̒�` ============================================================

// <�L�[�t���[��>
typedef struct {
	GameTimer timer;
	float time;
	BOOL state;
} KeyFrame;

// <�L�[�t���[��>
typedef struct {
	Easings easing_forward;
	Easings easing_back;
	BOOL two_way;
	KeyFrame* keyframe;
} EasingKeyFrame;

// �֐��̐錾 ==============================================================

// <�L�[�t���[���쐬>
KeyFrame KeyFrame_Create(float time);

// <�L�[�t���[����ԕύX>
void KeyFrame_SetState(KeyFrame* keyframe, BOOL state);

// <�L�[�t���[���i���擾>
float KeyFrame_GetProgress(KeyFrame* keyframe, float max = 1);

// <�L�[�t���[���i���擾>
float KeyFrame_GetProgressRange(KeyFrame* keyframe, float min = 0, float max = 1);

// <�L�[�t���[���쐬>
EasingKeyFrame EasingKeyFrame_Create(KeyFrame* keyframe, Easings easing_forward);

// <�Ώ�Easing���g���ăL�[�t���[���쐬>
EasingKeyFrame EasingKeyFrame_CreateWith2Easings(KeyFrame* keyframe, Easings easing_forward, Easings easing_back);

// <�L�[�t���[���i���擾>
float EasingKeyFrame_GetProgress(EasingKeyFrame* keyframe, float max = 1);

// <�L�[�t���[���i���擾>
float EasingKeyFrame_GetProgressRange(EasingKeyFrame* keyframe, float min = 0, float max = 1);
