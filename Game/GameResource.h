#pragma once

// �萔�̒�` ==============================================================

// <�t�H���g> ----------------------------------------------------------

#define FONT_SIZE_MAIN 80			// �t�H���g�T�C�Y
#define FONT_SIZE_MENU 20

// <���\�[�X> ----------------------------------------------------------

#define NUM_BGM 4
#define NUM_SE 4

// �\���̂̐錾 ============================================================

// <�Q�[�����\�[�X>
typedef struct
{
	// <�t�H���g>
	HFNT font_main;
	HFNT font_menu;

	// <�T�E���h>
	HSND sound_bgm[NUM_BGM];
	HSND sound_se[NUM_SE];

	// <�e�N�X�`��>
	// �^�C�g��
	
	// �v���C
	HGRP texture_planet1;
	HGRP texture_planet2;
	HGRP texture_planet3;
	HGRP texture_planet4;
	HGRP texture_planet5;
	HGRP texture_cursor1;

	// ���U���g

	// <����>
	HGRP movie_logo;
} GameResource;

// �֐��̐錾 ==============================================================

// <<���\�[�X>> --------------------------------------------------

// <���\�[�X�쐬�ǂݍ���>
GameResource GameResource_Create(void);

// <���\�[�X�J��>
void GameResource_Delete(GameResource* res);
