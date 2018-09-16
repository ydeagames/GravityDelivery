#include "GameResource.h"
#include "GameObject.h"

// �萔�̒�` ==============================================================

// <�t�H���g> ----------------------------------------------------------
#define FONT_FILE_MAIN_CUSTOM "Resources\\Fonts\\TheSlavicFont-Regular.ttf"		// �J�X�^���t�H���g�t�@�C��
#define FONT_NAME_MAIN_CUSTOM "The Slavic Font"									// �J�X�^���t�H���g�l�[��
#define FONT_NAME "HGP�n�p�p�޼��UB"										// �t�H�[���o�b�N�p�t�H���g�l�[��
#define FONT_NAME_MENU "HGP�n�p�p�޼��UB"

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


	// �e�N�X�`��
	// ���S����
	res.movie_logo = LoadGraph("Resources\\Movies\\ydeagames.avi");
	// �^�C�g��

	// �v���C
	res.texture_planet1 = LoadGraph("Resources\\Textures\\Protected\\StarLuster_OBJ_other.png");

	return res;
}

// <���\�[�X�J��>
void GameResource_Delete(GameResource* res)
{
	// �t�H���g
	DeleteFontToHandle(res->font_main);
	RemoveFontResourceEx(FONT_FILE_MAIN_CUSTOM, FR_PRIVATE, NULL);

	// �T�E���h


	// �e�N�X�`��
	// ���S����
	DeleteGraph(res->movie_logo);
	// �^�C�g��

	// �v���C
	DeleteGraph(res->texture_planet1);
	
	// �^�C�g��

}
