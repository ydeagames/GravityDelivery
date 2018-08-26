#include "SceneTitle.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameMain.h"
#include "GameObject.h"
#include "GameUtils.h"
#include "Easings.h"
#include "Vector.h"
#include <direct.h>
#include <io.h>




// �萔�̒�` ==============================================================

// �O���[�o���ϐ��̒�` ====================================================

static int g_count;
static Vector g_stages;

// �֐��̒�` ==============================================================

void listfiles(char* path, char* filter)
{
	struct _finddata_t fdata;

	intptr_t fh = _findfirst(filter, &fdata);
	Vector_Clear(&g_stages);
	if (fh != -1)
	{
		do {
			if ((fdata.attrib & _A_SUBDIR) == 0)
			{
				Stage stage;
				strcpy_s(stage.filename, fdata.name);
				snprintf(stage.filepath, 260, "%s/%s", path, fdata.name);
				Vector_AddLast(&g_stages, &stage);
			}
		} while (_findnext(fh, &fdata) == 0);
		_findclose(fh);
	}
}

// �^�C�g���V�[���̏���������
void InitializeTitle(void)
{
	g_count = 0;
	g_stages = Vector_Create(sizeof(Stage));

	_mkdir("./Resources/Stage");
	listfiles("./Resources/Stage", "./Resources/Stage/*.dat");
}

// �^�C�g���V�[���̍X�V����
void UpdateTitle(void)
{
	g_count++;

	//if (IsKeyPressed(PAD_INPUT_2))
	//	RequestScene(SCENE_PLAY);

	if (IsMousePressed(MOUSE_INPUT_1))
	{
		int pos = 0;
		foreach_start(&g_stages, Stage, stage)
		{
			GameObject rect = GameObject_Create(Vec2_Create(SCREEN_CENTER_X, (float)(SCREEN_TOP + 400 + 20 * pos + 10)), Vec2_Create(), Vec2_Create(SCREEN_WIDTH, 20));
			if (GameObject_IsHitPoint(&rect, &GetMousePosition()))
			{
				g_selected_stage = *stage;
				RequestScene(SCENE_PLAY);
			}
			pos++;
		} foreach_end;
	}

	if (IsKeyPressed(PAD_INPUT_16))
	{
		char str[260];

		// �V�K�t�@�C���̖��O����͂���
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_STR, COLOR_WHITE);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_CURSOR, COLOR_WHITE);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_SELECT_STR, COLOR_WHITE);
		if (KeyInputSingleCharString(GameObject_GetX(&g_field, CENTER_X, -200), GameObject_GetY(&g_field, BOTTOM, -200), 30, str, TRUE) == 1)
		{
			strcpy_s(g_selected_stage.filename, str);
			snprintf(g_selected_stage.filepath, 260, "%s/%s", "./Resources/Stage", str);
			RequestScene(SCENE_PLAY);
		}
	}
}

// �^�C�g���V�[���̕`�揈��
void RenderTitle(void)
{
	int pos = 0;
	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "���d�̓X�C�b�`��");
	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "�@�����N���b�N����Əd�͂�؂�ւ����܂��B");
	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "�@�E�N���b�N�h���b�O�Ńp��(���_�ړ�)���ł��܂��B");
	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "�A�d�͂𒲐����ăS�[���܂Ńv���C���[�𓱂��Ăˁ�");
	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "�B�S�[���ɐG�ꂽ��N���A�I");

	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "���X�e�[�W�����N���b�N���ăX�^�[�g��");

	pos = 0;
	foreach_start(&g_stages, Stage, stage)
	{
		GameObject rect = GameObject_Create(Vec2_Create(SCREEN_CENTER_X, (float)(SCREEN_TOP + 400 + 20 * pos + 10)), Vec2_Create(), Vec2_Create(SCREEN_WIDTH, 20));
		rect.fill = TRUE;
		rect.sprite.color = COLOR_GRAY;
		if (GameObject_IsHitPoint(&rect, &GetMousePosition()))
			GameObject_Render(&rect);
		DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 400 + 20 * pos, COLOR_WHITE, g_resources.font_menu, stage->filename);
		pos++;
	} foreach_end;
}

// �^�C�g���V�[���̏I������
void FinalizeTitle(void)
{
	Vector_Delete(&g_stages);
}