#pragma once

// �萔�̒�` ==============================================================

// <�t�H���g> ----------------------------------------------------------

#define FONT_SIZE_MAIN 20			// �t�H���g�T�C�Y

// <���\�[�X> ----------------------------------------------------------

#define NUM_BGM 4
#define NUM_SE 15

#define NUM_TEXTURE 8

// �\���̂̐錾 ============================================================

// <�Q�[�����\�[�X>
typedef struct
{
	// <�t�H���g>
	HFNT font_main;

	// <�T�E���h>
	HSND sound_bgm[NUM_BGM];
	HSND sound_se[NUM_SE];

	// <�e�N�X�`��>
	HGRP texture[NUM_TEXTURE];

	// <����>
	HGRP movie_logo;
} GameResource;

// �֐��̐錾 ==============================================================

// <<���\�[�X>> --------------------------------------------------

// <���\�[�X�쐬�ǂݍ���>
GameResource GameResource_Create(void);

// <���\�[�X�J��>
void GameResource_Delete(GameResource* res);
