#include "GameResource.h"
#include "GameObject.h"

// �萔�̒�` ==============================================================

// <�t�H���g> ----------------------------------------------------------
#define FONT_FILE_MAIN_CUSTOM "Resources\\Fonts\\TheSlavicFont-Regular.ttf"		// �J�X�^���t�H���g�t�@�C��
#define FONT_NAME_MAIN_CUSTOM "The Slavic Font"									// �J�X�^���t�H���g�l�[��
#define FONT_NAME "HGP�n�p�p�޼��UB"										// �t�H�[���o�b�N�p�t�H���g�l�[��
#define FONT_NAME_MENU "HGP�n�p�p�޼��UB"

// <�T�E���h> ----------------------------------------------------------
#define SOUND_BGM "Resources\\Audio\\nv_01.mp3"
#define SOUND_SE_HIT "Resources\\Audio\\Protected\\xev_se_EnemyStriken_Zapper.wav"
#define SOUND_SE_HIT_BOSS "Resources\\Audio\\Protected\\xev_se_Zakato_DethoutSound.wav"
#define SOUND_SE_HIT_BOSS_DEAD "Resources\\Audio\\Protected\\xev_se_EnemyStriken_Blaster.wav"
#define SOUND_SE_CHARGE "Resources\\Audio\\Protected\\xev_se_Andorgenesis_loop.wav"
#define SOUND_SE_SHOOT "Resources\\Audio\\Protected\\xev_se_ZapperShoot.wav"
#define SOUND_SE_SHOOTBIG "Resources\\Audio\\Protected\\xev_se_EnemyStriken_Blaster.wav"
#define SOUND_SE_DEAD "Resources\\Audio\\Protected\\xev_se_Miss.wav"

// <�e�N�X�`��> --------------------------------------------------------
// �^�C�g��
#define TEXTURE_TITLE "Resources\\Textures\\Title\\title.png"
#define TEXTURE_TITLE_TEXT "Resources\\Textures\\Title\\press_button.png"
// �v���C
#define TEXTURE_PLAYER_PROTECTED "Resources\\Textures\\Protected\\Solvalou.png"		// ���\�[�X�ی�e�N�X�`��
#define TEXTURE_PLAYER "Resources\\Textures\\Solvalou.png"							// �I�[�v���e�N�X�`��
#define TEXTURE_BULLET_PROTECTED "Resources\\Textures\\Protected\\Bullet.png"		// ���\�[�X�ی�e�N�X�`��
#define TEXTURE_BULLET "Resources\\Textures\\Bullet.png"							// �I�[�v���e�N�X�`��
#define TEXTURE_ENEMY_PROTECTED "Resources\\Textures\\Protected\\Enemies.png"		// ���\�[�X�ی�e�N�X�`��
#define TEXTURE_ENEMY "Resources\\Textures\\Enemies.png"							// �I�[�v���e�N�X�`��
#define TEXTURE_EXPLOSION_PROTECTED "Resources\\Textures\\Protected\\Explosion.png"	// ���\�[�X�ی�e�N�X�`��
#define TEXTURE_EXPLOSION "Resources\\Textures\\Explosion.png"						// �I�[�v���e�N�X�`��
#define TEXTURE_MAP_PROTECTED "Resources\\Textures\\Protected\\Map.png"				// ���\�[�X�ی�e�N�X�`��
#define TEXTURE_MAP "Resources\\Textures\\Map.png"									// �I�[�v���e�N�X�`��
#define TEXTURE_CLOUD_PROTECTED "Resources\\Textures\\Protected\\Cloud.png"			// ���\�[�X�ی�e�N�X�`��
#define TEXTURE_CLOUD "Resources\\Textures\\Cloud.png"								// �I�[�v���e�N�X�`��
// ���U���g
#define TEXTURE_RESULT "Resources\\Textures\\Result\\result.png"
#define TEXTURE_RESULT_TEXT "Resources\\Textures\\Result\\press_button_back.png"

// <����> --------------------------------------------------------------
#define MOVIE_LOGO "Resources\\Movies\\ydeagames.avi"								// ���S���[�r�[

// �֐��̒�` ==============================================================

// <<���\�[�X>> --------------------------------------------------

// <���\�[�X�쐬�ǂݍ���>
GameResource GameResource_Create(void)
{
	GameResource res;

	// �t�H���g
	// �J�X�^���t�H���g�t�@�C�����ǂݍ��߂���g��
	if (AddFontResourceEx(FONT_FILE_MAIN_CUSTOM, FR_PRIVATE, NULL) > 0)
		res.font_main = CreateFontToHandle(FONT_NAME_MAIN_CUSTOM, FONT_SIZE_MAIN, 3, DX_FONTTYPE_ANTIALIASING_4X4);
	else
		res.font_main = CreateFontToHandle(FONT_NAME, FONT_SIZE_MAIN, 3, DX_FONTTYPE_ANTIALIASING_4X4);
	res.font_menu = CreateFontToHandle(FONT_NAME_MENU, FONT_SIZE_MENU, 16, DX_FONTTYPE_ANTIALIASING_4X4);

	// �T�E���h
	res.sound_bgm = LoadSoundMem(SOUND_BGM);
	res.sound_se_hit = LoadSoundMem(SOUND_SE_HIT);
	res.sound_se_hit_boss = LoadSoundMem(SOUND_SE_HIT_BOSS);
	res.sound_se_hit_boss_dead = LoadSoundMem(SOUND_SE_HIT_BOSS_DEAD);
	res.sound_se_charge = LoadSoundMem(SOUND_SE_CHARGE);
	res.sound_se_shoot = LoadSoundMem(SOUND_SE_SHOOT);
	res.sound_se_shootbig = LoadSoundMem(SOUND_SE_SHOOTBIG);
	res.sound_se_dead = LoadSoundMem(SOUND_SE_DEAD);

	// �e�N�X�`��
	// ���S����
	res.movie_logo = LoadGraph(MOVIE_LOGO);
	// �^�C�g��
	res.texture_title = LoadGraph(TEXTURE_TITLE);
	res.texture_title_text = LoadGraph(TEXTURE_TITLE_TEXT);
	// �v���C
	res.texture_player = LoadGraph(TEXTURE_PLAYER_PROTECTED);
	if (res.texture_player == TEXTURE_MISSING)										// ���\�[�X�ی�e�N�X�`��������������
		res.texture_player = LoadGraph(TEXTURE_PLAYER);								// �I�[�v���e�N�X�`����ǂݍ���
	res.texture_bullet = LoadGraph(TEXTURE_BULLET_PROTECTED);
	if (res.texture_bullet == TEXTURE_MISSING)										// ���\�[�X�ی�e�N�X�`��������������
		res.texture_bullet = LoadGraph(TEXTURE_BULLET);								// �I�[�v���e�N�X�`����ǂݍ���
	res.texture_enemy = LoadGraph(TEXTURE_ENEMY_PROTECTED);
	if (res.texture_enemy == TEXTURE_MISSING)										// ���\�[�X�ی�e�N�X�`��������������
		res.texture_enemy = LoadGraph(TEXTURE_ENEMY);								// �I�[�v���e�N�X�`����ǂݍ���
	res.texture_explosion = LoadGraph(TEXTURE_EXPLOSION_PROTECTED);
	if (res.texture_explosion == TEXTURE_MISSING)									// ���\�[�X�ی�e�N�X�`��������������
		res.texture_explosion = LoadGraph(TEXTURE_EXPLOSION);						// �I�[�v���e�N�X�`����ǂݍ���
	res.texture_map = LoadGraph(TEXTURE_MAP_PROTECTED);
	if (res.texture_map == TEXTURE_MISSING)											// ���\�[�X�ی�e�N�X�`��������������
		res.texture_map = LoadGraph(TEXTURE_MAP);									// �I�[�v���e�N�X�`����ǂݍ���
	res.texture_cloud = LoadGraph(TEXTURE_CLOUD_PROTECTED);
	if (res.texture_cloud == TEXTURE_MISSING)										// ���\�[�X�ی�e�N�X�`��������������
		res.texture_cloud = LoadGraph(TEXTURE_CLOUD);								// �I�[�v���e�N�X�`����ǂݍ���
	// ���U���g
	res.texture_result = LoadGraph(TEXTURE_RESULT);
	res.texture_result_text = LoadGraph(TEXTURE_RESULT_TEXT);

	return res;
}

// <���\�[�X�J��>
void GameResource_Delete(GameResource* res)
{
	// �t�H���g
	DeleteFontToHandle(res->font_main);
	RemoveFontResourceEx(FONT_FILE_MAIN_CUSTOM, FR_PRIVATE, NULL);

	// �T�E���h
	DeleteSoundMem(res->sound_bgm);
	DeleteSoundMem(res->sound_se_hit);
	DeleteSoundMem(res->sound_se_hit_boss);
	DeleteSoundMem(res->sound_se_hit_boss_dead);
	DeleteSoundMem(res->sound_se_charge);
	DeleteSoundMem(res->sound_se_shoot);
	DeleteSoundMem(res->sound_se_shootbig);
	DeleteSoundMem(res->sound_se_dead);

	// �e�N�X�`��
	// ���S����
	DeleteGraph(res->movie_logo);
	// �^�C�g��
	DeleteGraph(res->texture_title);
	DeleteGraph(res->texture_title_text);
	// �v���C
	DeleteGraph(res->texture_player);
	DeleteGraph(res->texture_bullet);
	DeleteGraph(res->texture_enemy);
	DeleteGraph(res->texture_explosion);
	DeleteGraph(res->texture_map);
	DeleteGraph(res->texture_cloud);
	// �^�C�g��
	DeleteGraph(res->texture_result);
	DeleteGraph(res->texture_result_text);
}
