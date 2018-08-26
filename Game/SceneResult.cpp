#include "SceneTitle.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameMain.h"
#include "GameObject.h"
#include "GameUtils.h"
#include "Easings.h"

// �萔�̒�` ==============================================================

// �O���[�o���ϐ��̒�` ====================================================

// �֐��̒�` ==============================================================

// �^�C�g���V�[���̏���������
void InitializeResult(void)
{
}

// �^�C�g���V�[���̍X�V����
void UpdateResult(void)
{
	if (IsKeyPressed(PAD_INPUT_2))
		RequestScene(SCENE_TITLE);
}

// �^�C�g���V�[���̕`�揈��
void RenderResult(void)
{
	int pos = 0;
	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "�X�e�[�W�N���A");
	DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_TOP + 200 + 20 * pos++, COLOR_WHITE, g_resources.font_menu, "%s", g_selected_stage.filename);
}

// �^�C�g���V�[���̏I������
void FinalizeResult(void)
{
}