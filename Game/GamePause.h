#pragma once
#include "GameObject.h"

// �O���[�o���ϐ��̐錾 ====================================================

extern GameObject g_backscreen;

// �֐��̐錾 ==============================================================

// �|�[�Y�̏���������
void InitializeGamePause(void);

// �|�[�Y�̍X�V����
void UpdateGamePause(void);

// �|�[�Y�̕`�揈��
void RenderGamePause(void);

// �|�[�Y�̏I������
void FinalizeGamePause(void);

// �|�[�Y�v��
void RequestPause(void);

// �|�[�Y����
BOOL IsGamePaused(void);