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
	ChangeVolumeSoundMem(100, res.sound_se[0]);
	res.sound_se[1] = LoadSoundMem("Resources/Audio/button45.mp3");
	res.sound_se[2] = LoadSoundMem("Resources/Audio/activate.mp3");
	ChangeVolumeSoundMem(150, res.sound_se[2]);
	res.sound_se[3] = LoadSoundMem("Resources/Audio/short01a.wav");
	ChangeVolumeSoundMem(180, res.sound_se[3]);
	res.sound_se[4] = LoadSoundMem("Resources/Audio/button55.mp3");
	res.sound_se[5] = LoadSoundMem("Resources/Audio/cancel1.mp3");
	ChangeVolumeSoundMem(180, res.sound_se[5]);
	res.sound_se[6] = LoadSoundMem("Resources/Audio/bomb1.mp3");
	ChangeVolumeSoundMem(150, res.sound_se[6]);
	res.sound_se[7] = LoadSoundMem("Resources/Audio/clear.wav");
	ChangeVolumeSoundMem(150, res.sound_se[7]);
	res.sound_se[8] = LoadSoundMem("Resources/Audio/gun-fire05.mp3");
	ChangeVolumeSoundMem(150, res.sound_se[8]);
	res.sound_se[9] = LoadSoundMem("Resources/Audio/teleport01.mp3");
	ChangeVolumeSoundMem(150, res.sound_se[9]);
	res.sound_se[10] = LoadSoundMem("Resources/Audio/beam-gun03.mp3");
	ChangeVolumeSoundMem(170, res.sound_se[10]);
	res.sound_se[11] = LoadSoundMem("Resources/Audio/system31.mp3");
	ChangeVolumeSoundMem(140, res.sound_se[11]);
	res.sound_se[12] = LoadSoundMem("Resources/Audio/gun-fire04.mp3");
	res.sound_se[13] = LoadSoundMem("Resources/Audio/spo_ge_mbat02.mp3");
	ChangeVolumeSoundMem(210, res.sound_se[13]);
	res.sound_se[14] = LoadSoundMem("Resources/Audio/beam-gun05.mp3");
	ChangeVolumeSoundMem(120, res.sound_se[14]);
	res.sound_se[15] = LoadSoundMem("Resources/Audio/deactivate.mp3");
	ChangeVolumeSoundMem(150, res.sound_se[15]);

	// �e�N�X�`��
	res.texture[0] = LoadGraph("Resources/Textures/bomb.png");
	res.texture[1] = LoadGraph("Resources/Textures/goal.png");
	res.texture[2] = LoadGraph("Resources/Textures/planets.png");
	res.texture[3] = LoadGraph("Resources/Textures/start.png");
	res.texture[4] = LoadGraph("Resources/Textures/warp.png");
	res.texture[5] = LoadGraph("Resources/Textures/launcher.png");
	res.texture[6] = LoadGraph("Resources/Textures/doom.png");
	res.texture[7] = LoadGraph("Resources/Textures/goal_doom.png");
	res.texture[8] = LoadGraph("Resources/Textures/gravity_switch.png");
	res.texture[9] = LoadGraph("Resources/Textures/cursor.png");

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
			DeleteGraph(res->texture[i]);
	}
}
