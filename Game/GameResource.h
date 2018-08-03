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
	HSND sound_se_hit;
	HSND sound_se_hit_boss;
	HSND sound_se_hit_boss_dead;
	HSND sound_se_charge;
	HSND sound_se_shoot;
	HSND sound_se_shootbig;
	HSND sound_se_dead;

	// <�e�N�X�`��>
	// �^�C�g��
	HGRP texture_title;
	HGRP texture_title_text;
	// �v���C
	HGRP texture_player;
	HGRP texture_bullet;
	HGRP texture_enemy;
	HGRP texture_explosion;
	HGRP texture_map;
	HGRP texture_cloud;
	// ���U���g
	HGRP texture_result;
	HGRP texture_result_text;

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
