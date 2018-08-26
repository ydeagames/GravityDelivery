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

void listfiles(char *path)
{
	struct _finddata_t fdata;

	intptr_t fh = _findfirst(path, &fdata);
	Vector_Clear(&g_stages);
	if (fh != -1)
	{
		do {
			if ((fdata.attrib & _A_SUBDIR) == 0)
			{
				Stage stage;
				strcpy_s(stage.filename, fdata.name);
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

	_mkdir("Stage");
	listfiles("./Stage/*.dat");
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
}

// �^�C�g���V�[���̕`�揈��
void RenderTitle(void)
{
	int pos = 0;
	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "���d�̓X�C�b�`��");
	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "�@�����N���b�N����Əd�͂�؂�ւ����܂�");
	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "�A�d�͂𒲐����ăS�[���܂Ńv���C���[�𓱂��Ă�");
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