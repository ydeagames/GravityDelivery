#pragma once

// �\���̂̐錾 ============================================================

// <�Q�[�����\�[�X>
typedef struct
{
	// <�t�H���g>
	HFNT font_main;
	HFNT font_menu;

	// <�T�E���h>
	HSND sound_bgm;

	// <�e�N�X�`��>
	// �^�C�g��
	
	// �v���C
	HGRP texture_planet1;
	// ���U���g

	// <����>
	HGRP movie_logo;
} GameResource;

// �萔�̒�` ==============================================================

// <�t�H���g> ----------------------------------------------------------

#define FONT_SIZE_MAIN 80			// �t�H���g�T�C�Y
#define FONT_SIZE_MENU 20

// �֐��̐錾 ==============================================================

// <<���\�[�X>> --------------------------------------------------

// <���\�[�X�쐬�ǂݍ���>
GameResource GameResource_Create(void);

// <���\�[�X�J��>
void GameResource_Delete(GameResource* res);
