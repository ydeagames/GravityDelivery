#pragma once

// �\���̂̐錾 ============================================================

// <�Q�[���^�C�}�[>>
typedef struct
{
	int start_time;	// �J�n����
	int last_time;	// �I������
	BOOL paused;	// �ꎞ��~
	float remaining;// �I������
} GameTimer;

// �萔�̒�` ==============================================================

#define GAME_DURATION_SECONDS 60		// �Q�[���̐�������

// �֐��̐錾 ==============================================================

// <<�^�C�}�[>> --------------------------------------------------

// <�^�C�}�[�쐬>
GameTimer GameTimer_Create(void);

// <�^�C�}�[����>
float GameTimer_GetTime(GameTimer* timer);

// <�^�C�}�[�ꎞ��~��>
BOOL GameTimer_IsPaused(GameTimer* timer);

// <�^�C�}�[��~>
void GameTimer_Pause(GameTimer* timer);

// <�^�C�}�[���Z�b�g>
void GameTimer_Reset(GameTimer* timer);

// <�^�C�}�[�ĊJ>
void GameTimer_Resume(GameTimer* timer);

// <�^�C�}�[�Z�b�g>
void GameTimer_Set(GameTimer* timer, float new_time);


// <�^�C�}�[�c�莞�ԃZ�b�g>
void GameTimer_SetRemaining(GameTimer* timer, float remaining);

// <�Q�[�����Ԑ����Ń^�C�}�[�c�莞�ԃZ�b�g>
void GameTimer_SetRemainingDefault(GameTimer* timer);

// <�^�C�}�[�c�莞�ԃ��Z�b�g>
void GameTimer_ResetRemaining(GameTimer* timer);

// <�g�[�^�����Ԏ擾>
float GameTimer_GetTotal(GameTimer* timer);

// <�i���擾>
float GameTimer_GetProgress(GameTimer* timer);

// <�^�C�}�[�c�莞��>
float GameTimer_GetRemaining(GameTimer* timer);

// <�^�C�}�[�o�ߎ���>
float GameTimer_GetElapsed(GameTimer* timer);

// <�^�C�}�[�I������>
BOOL GameTimer_IsFinished(GameTimer* timer);
