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
#include "GameObjectHit.h"
#include "GameResource.h"
#include "ScreenShot.h"
#include "GameStage.h"
#include "GamePause.h"




// �萔�̒�` ==============================================================




// �O���[�o���ϐ��̒�` ====================================================

GameResource g_resources;
StageInfo g_selected_stageinfo;
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
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_10, KEY_INPUT_F3);

	// ���\�[�X��ǂݍ���
	g_resources = GameResource_Create();

	// ��ʔ͈�
	g_field = GameObject_Field_Create();

	// �f�o�b�O�R���\�[����������
	{
		Vec2 size = Vec2_Create(200, 200);
		GameObject area = GameObject_Create(Vec2_Create(GameObject_GetX(&g_field, LEFT, -size.x / 2), GameObject_GetY(&g_field, BOTTOM, -size.y / 2)), Vec2_Create(), size);

		g_console = DebugConsole_Create(area, g_resources.font_main, 20);
	}

	// �J�[�\�����\��
	SetMouseDispFlag(FALSE);

	// �|�[�Y��������
	InitializeGamePause();

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

	// F3�L�[��������Ă�����
	if (IsJoypadPressed(PAD_INPUT_10))
	{
		// �f�o�b�O���[�h���g�O��
		g_debug_mode = !g_debug_mode;
	}

	// �}�E�X�̍X�V
	g_raw_mouse_last = g_raw_mouse;
	g_raw_mouse = GetMousePosition();

	// �J�[�\���\����\��
	SetMouseDispFlag(!GameObject_IsHitPoint(&g_field, &g_raw_mouse));

	// �|�[�Y���X�V
	UpdateGamePause();

	// �V�[���}�l�[�W���[���X�V
	if (!IsGamePaused())
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
	screen_start(g_backscreen.sprite.texture.texture)
	{
		ClearDrawScreen();
		// �V�[���}�l�[�W���[��`��
		RenderSceneManager();
	} screen_end;

	// �|�[�Y��`��
	RenderGamePause();

	// �f�o�b�O�R���\�[����`��
	DebugConsole_Render(&g_console);

	// �J�[�\����`��
	{
		Vec2 point = GetMousePosition();
		DrawTriangleAA(point.x, point.y, point.x + 20, point.y + 10, point.x + 10, point.y + 20, COLOR_GREEN, TRUE);
	}

	// �X�N���[���V���b�g
	if (IsKeyPressed(KEY_INPUT_F2))
		SaveScreenShotToPNG_Log(&g_field);
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

	// �|�[�Y���t�@�C�i���C�Y
	FinalizeGamePause();

	// ���\�[�X���A�����[�h
	GameResource_Delete(&g_resources);
}
