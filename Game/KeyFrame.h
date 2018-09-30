#pragma once
#include "Easings.h"
#include "GameTimer.h"

// �\���̂̒�` ============================================================

// <�L�[�t���[��>
typedef struct {
	Easings easing_forward;
	Easings easing_back;
	BOOL two_way;
	GameTimer timer;
	float time;
	BOOL state;
} KeyFrame;

// �֐��̐錾 ==============================================================

// <�L�[�t���[���쐬>
KeyFrame KeyFrame_Create(float time, Easings easing_forward);

// <�Ώ�Easing���g���ăL�[�t���[���쐬>
KeyFrame KeyFrame_CreateWith2Easings(float time, Easings easing_forward, Easings easing_back);

// <�L�[�t���[����ԕύX>
void KeyFrame_SetState(KeyFrame* keyframe, BOOL state);

// <�L�[�t���[���i���擾>
float KeyFrame_GetProgress(KeyFrame* keyframe, float max = 1);

// <�L�[�t���[���i���擾>
float KeyFrame_GetProgressRange(KeyFrame* keyframe, float min = 0, float max = 1);
