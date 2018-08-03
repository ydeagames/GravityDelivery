#include "GameTimer.h"

// �萔�̒�` ==============================================================

#define RESOLUTION 1000					// �~���b���b

// �֐��̒�` ==============================================================

// <<�^�C�}�[>> --------------------------------------------------

// <�^�C�}�[�쐬>
GameTimer GameTimer_Create(void)
{
	GameTimer timer = { -1, -1, TRUE };
	// �^�C�}�[�����Z�b�g
	GameTimer_Reset(&timer);
	return timer;
}

// <�^�C�}�[����>
float GameTimer_GetTime(GameTimer* timer)
{
	// �ꎞ��~����ĂȂ���΍X�V����
	if (!timer->paused)
		timer->last_time = GetNowCount() - timer->start_time;
	return (float)timer->last_time / RESOLUTION;
}

// <�^�C�}�[�ꎞ��~��>
BOOL GameTimer_IsPaused(GameTimer* timer)
{
	return timer->paused;
}

// <�^�C�}�[��~>
void GameTimer_Pause(GameTimer* timer)
{
	// �ꎞ��~����ĂȂ���΍X�V����
	if (!timer->paused)
		timer->last_time = GetNowCount() - timer->start_time;
	timer->paused = TRUE;
}

// <�^�C�}�[���Z�b�g>
void GameTimer_Reset(GameTimer* timer)
{
	GameTimer_Set(timer, 0);
}

// <�^�C�}�[�ĊJ>
void GameTimer_Resume(GameTimer* timer)
{
	// �ꎞ��~����Ă���΍ĊJ�J�n�������v�Z���čX�V
	if (timer->paused)
		timer->start_time = GetNowCount() - timer->last_time;
	timer->paused = FALSE;
}

// <�^�C�}�[�Z�b�g>
void GameTimer_Set(GameTimer* timer, float new_time)
{
	// �~���b
	int new_time_in_ticks = (int)(new_time * RESOLUTION);
	// �J�n�������v�Z
	timer->start_time = GetNowCount() - new_time_in_ticks;
	// �I���������v�Z
	timer->last_time = new_time_in_ticks;
}

// <�^�C�}�[�c�莞�ԃZ�b�g>
void GameTimer_SetRemaining(GameTimer* timer, float remaining)
{
	GameTimer_Set(timer, -remaining);
}

// <�Q�[�����Ԑ����Ń^�C�}�[�c�莞�ԃZ�b�g>
void GameTimer_SetRemainingDefault(GameTimer* timer)
{
	GameTimer_SetRemaining(timer, GAME_DURATION_SECONDS);
}

// <�^�C�}�[�c�莞��>
float GameTimer_GetTimeRemaining(GameTimer* timer)
{
	return -GameTimer_GetTime(timer);
}

// <�^�C�}�[�I������>
BOOL GameTimer_IsFinished(GameTimer* timer)
{
	return GameTimer_GetTime(timer) >= 0;
}
