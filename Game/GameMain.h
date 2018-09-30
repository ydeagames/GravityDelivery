//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.h
//!
//! @brief  �Q�[���֘A�̃w�b�_�t�@�C��
//!
//! @date   ���t  2018/09/30
//!
//! @author ����Җ�  YdeaGames
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once

#include "GameResource.h"
#include "DebugConsole.h"
#include "GameStage.h"




// �萔�̒�` ==============================================================

#define screen_start(screen) \
{ \
	HGRP screen_stack = GetDrawScreen(); \
	SetDrawScreen(screen); \
	{
#define screen_end \
	} \
	SetDrawScreen(screen_stack); \
}



// <�V�X�e��> ----------------------------------------------------------
#define GAME_TITLE "�d�̓X�C�b�`"              // �Q�[���^�C�g��



// <���> --------------------------------------------------------------
#define SCREEN_WIDTH    (1600)                  // ��ʂ̕�[pixel]
#define SCREEN_HEIGHT   (800)                  // ��ʂ̍���[pixel]

#define SCREEN_TOP      (0)                    // ��ʂ̏�[
#define SCREEN_BOTTOM   (SCREEN_HEIGHT)        // ��ʂ̉��[
#define SCREEN_LEFT     (0)                    // ��ʂ̍��[
#define SCREEN_RIGHT    (SCREEN_WIDTH)         // ��ʂ̉E�[

#define SCREEN_CENTER_X (SCREEN_WIDTH  / 2)    // ��ʂ̒���(X���W)
#define SCREEN_CENTER_Y (SCREEN_HEIGHT / 2)    // ��ʂ̒���(Y���W)




// �\���̂̒�` ============================================================




// �O���[�o���ϐ��̐錾 ====================================================

extern GameResource g_resources;
extern StageInfo g_selected_stageinfo;
extern GameObject g_field;
extern DebugConsole g_console;

extern Vec2 g_raw_mouse;
extern Vec2 g_raw_mouse_last;



// �֐��̐錾 ==============================================================

// �Q�[���̏���������
void InitializeGame(void);

// �Q�[���̍X�V����
void UpdateGame(void);

// �Q�[���̕`�揈��
void RenderGame(void);

// �Q�[���̏I������
void FinalizeGame(void);
