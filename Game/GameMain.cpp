//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.cpp
//!
//! @brief  �Q�[���֘A�̃\�[�X�t�@�C��
//!
//! @date   ���t
//!
//! @author ����Җ�
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================

#include "GameMain.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "GameResource.h"



// �萔�̒�` ==============================================================




// �O���[�o���ϐ��̒�` ====================================================

GameResource g_resources;
Stage g_selected_stage;
GameObject g_field;
DebugConsole g_console;

Vec2 g_raw_mouse;
Vec2 g_raw_mouse_last;



// �֐��̐錾 ==============================================================

void InitializeGame(void);  // �Q�[���̏���������
void UpdateGame(void);      // �Q�[���̍X�V����
void RenderGame(void);      // �Q�[���̕`�揈��
void FinalizeGame(void);    // �Q�[���̏I������




// �֐��̒�` ==============================================================

//----------------------------------------------------------------------
//! @brief �Q�[���̏���������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void InitializeGame(void)
{
	//SetUpMemoryLeakDetector();

	// �t�@���N�V�����L�[
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_11, KEY_INPUT_F3);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_12, KEY_INPUT_F5);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_13, KEY_INPUT_F6);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_14, KEY_INPUT_F7);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_15, KEY_INPUT_F8);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_16, KEY_INPUT_F9);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_17, KEY_INPUT_F10);
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_18, KEY_INPUT_F11);

	// ���\�[�X��ǂݍ���
	g_resources = GameResource_Create();

	// ��ʔ͈�
	g_field = GameObject_Field_Create();

	// �f�o�b�O�R���\�[����������
	{
		Vec2 size = Vec2_Create(200, 200);
		GameObject area = GameObject_Create(Vec2_Create(GameObject_GetX(&g_field, LEFT, -size.x / 2), GameObject_GetY(&g_field, BOTTOM, -size.y / 2)), Vec2_Create(), size);

		g_console = DebugConsole_Create(area, g_resources.font_menu, 20);
	}

	// �J�[�\�����\��
	SetMouseDispFlag(FALSE);

	// �V�[���}�l�[�W���[��������
	InitializeSceneManager(SCENE_LOGO);
}



//----------------------------------------------------------------------
//! @brief �Q�[���̍X�V����
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void UpdateGame(void)
{
	// ���̓}�l�[�W���[���X�V
	UpdateInputManager();
	// �e�B�b�N�̍X�V
	GameTick_Update();

	// F11�L�[��������Ă�����
	if (IsKeyPressed(PAD_INPUT_11))
	{
		// �f�o�b�O���[�h���g�O��
		g_debug_mode = !g_debug_mode;
	}

	// �}�E�X�̍X�V
	g_raw_mouse_last = g_raw_mouse;
	g_raw_mouse = GetMousePosition();

	// �V�[���}�l�[�W���[���X�V
	UpdateSceneManager();
}




//----------------------------------------------------------------------
//! @brief �Q�[���̕`�揈��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void RenderGame(void)
{
	// �V�[���}�l�[�W���[��`��
	RenderSceneManager();

	// �f�o�b�O�R���\�[����`��
	DebugConsole_Render(&g_console);

	// �J�[�\����`��
	{
		Vec2 point = GetMousePosition();
		DrawTriangleAA(point.x, point.y, point.x + 20, point.y + 10, point.x + 10, point.y + 20, COLOR_GREEN, TRUE);
	}
}



//----------------------------------------------------------------------
//! @brief �Q�[���̏I������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void FinalizeGame(void)
{
	// �V�[���}�l�[�W���[���t�@�C�i���C�Y
	FinalizeSceneManager();

	// ���\�[�X���A�����[�h
	GameResource_Delete(&g_resources);
}
