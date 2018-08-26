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

	// F3�L�[���f�o�b�O�`��ɐݒ�
	SetJoypadInputToKeyInput(DX_INPUT_KEY_PAD1, PAD_INPUT_11, KEY_INPUT_F3);

	// ���\�[�X��ǂݍ���
	g_resources = GameResource_Create();

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
		extern BOOL g_debug_mode;
		g_debug_mode = !g_debug_mode;
	}

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
