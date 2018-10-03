#include "GameResource.h"
#include "GameObject.h"

// �萔�̒�` ==============================================================

// <�t�H���g> ----------------------------------------------------------
#define FONT_FILE_MAIN_CUSTOM "Resources\\Fonts\\uzura.ttf"						// �J�X�^���t�H���g�t�@�C��
#define FONT_NAME_MAIN_CUSTOM "������t�H���g"									// �J�X�^���t�H���g�l�[��
#define FONT_NAME "HGP�n�p�p�޼��UB"											// �t�H�[���o�b�N�p�t�H���g�l�[��

// �֐��̒�` ==============================================================

// <<���\�[�X>> --------------------------------------------------

// <���\�[�X�쐬�ǂݍ���>
GameResource GameResource_Create(void)
{
	GameResource res;

	// ���S����
	res.movie_logo = LoadGraph("Resources/Movies/ydeagames.mp4");

	// �t�H���g
	// �J�X�^���t�H���g�t�@�C�����ǂݍ��߂���g��
	if (AddFontResourceEx(FONT_FILE_MAIN_CUSTOM, FR_PRIVATE, NULL) > 0)
		res.font_main = CreateFontToHandle(FONT_NAME_MAIN_CUSTOM, FONT_SIZE_MAIN, 1, DX_FONTTYPE_ANTIALIASING_4X4);
	else
		res.font_main = CreateFontToHandle(FONT_NAME, FONT_SIZE_MAIN, 1, DX_FONTTYPE_ANTIALIASING_4X4);

	// �T�E���h
	res.sound_bgm[0] = LoadSoundMem("Resources/Audio/Protected/bab_music_01.mp3");
	res.sound_bgm[1] = LoadSoundMem("Resources/Audio/Protected/drb_music_catacomb.wav");
	res.sound_bgm[2] = LoadSoundMem("Resources/Audio/Protected/drb_music_brick_dungeon.wav");
	res.sound_bgm[3] = LoadSoundMem("Resources/Audio/Protected/dru_music_ishtar_theme.wav");
	res.sound_se[0] = LoadSoundMem("Resources/Audio/Protected/xev_se_Zakato_DethoutSound.wav");
	res.sound_se[1] = LoadSoundMem("Resources/Audio/Protected/bc_se_17.wav");
	res.sound_se[2] = LoadSoundMem("Resources/Audio/Protected/genpei_se_06.wav");
	res.sound_se[3] = LoadSoundMem("Resources/Audio/Protected/wgl_se_08.wav");
	res.sound_se[4] = LoadSoundMem("Resources/Audio/Protected/bc_se_15.wav");
	res.sound_se[5] = LoadSoundMem("Resources/Audio/Protected/wonder_se_13.wav");
	res.sound_se[6] = LoadSoundMem("Resources/Audio/Protected/xev_se_Miss.wav");
	res.sound_se[7] = LoadSoundMem("Resources/Audio/Protected/dig_music_StageClear.wav");
	res.sound_se[8] = LoadSoundMem("Resources/Audio/Protected/genpei_se_12.wav");
	res.sound_se[9] = LoadSoundMem("Resources/Audio/Protected/map_se_trampoline_jump.wav");
	res.sound_se[10] = LoadSoundMem("Resources/Audio/Protected/gax_Shot.wav");
	res.sound_se[11] = LoadSoundMem("Resources/Audio/Protected/drb_se_jump.wav");
	res.sound_se[12] = LoadSoundMem("Resources/Audio/Protected/xev_se_EnemyStriken_Blaster.wav");
	res.sound_se[13] = LoadSoundMem("Resources/Audio/Protected/sky_se_baron_max_damege_sound.wav");
	res.sound_se[14] = LoadSoundMem("Resources/Audio/Protected/dru_se_sword_sheathing.wav");

	// �e�N�X�`��
	res.texture[0] = LoadGraph("Resources/Textures/Protected/StarLuster_OBJ_other.png");
	res.texture[1] = LoadGraph("Resources/Textures/Protected/StarLuster_OBJ_enemy.png");
	res.texture[2] = LoadGraph("Resources/Textures/Protected/StarLuster_OBJ_enemy_big.png");
	res.texture[3] = LoadGraph("Resources/Textures/Protected/StarLuster_OBJ_photonTorpedo.png");
	res.texture[4] = LoadGraph("Resources/Textures/Protected/StarLuster_OBJ_supplyBase.png");
	res.texture[5] = LoadGraph("Resources/Textures/Protected/Xevious_OBJ_solvalou.png");
	res.texture[6] = LoadGraph("Resources/Textures/gravity_switch.png");
	res.texture[7] = LoadGraph("Resources/Textures/Protected/Xevious_OBJ_enemyBroken.png");

	return res;
}

// <���\�[�X�J��>
void GameResource_Delete(GameResource* res)
{
	// �t�H���g
	DeleteFontToHandle(res->font_main);
	RemoveFontResourceEx(FONT_FILE_MAIN_CUSTOM, FR_PRIVATE, NULL);

	// �T�E���h
	{
		int i;
		for (i = 0; i < NUM_BGM; i++)
			DeleteSoundMem(res->sound_bgm[i]);
		for (i = 0; i < NUM_SE; i++)
			DeleteSoundMem(res->sound_se[i]);
	}

	// �e�N�X�`��
	// ���S����
	DeleteGraph(res->movie_logo);

	// �e�N�X�`��
	{
		int i;
		for (i = 0; i < NUM_TEXTURE; i++)
			DeleteSoundMem(res->texture[i]);
	}
}
