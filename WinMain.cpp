//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   WinMain.cpp
//!
//! @brief  �Q�[���v���O�����̃G���g���[�|�C���g�̃\�[�X�t�@�C��
//!
//! @date   2017/3/17
//!
//! @author ���� �W
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "Game\GameMain.h"
#include "Game\SceneManager.h"
#include "Resources\Icon\Icon.h"

#include <shlwapi.h>
#pragma comment( lib , "shlwapi.lib" )




// �O���[�o���ϐ��̒�` ====================================================

HWND hWnd;




// �֐���` ================================================================

// <�J�����g�f�B���N�g���̏C��>
static void UpdateCurrentDir(void)
{
	TCHAR m_Path[MAX_PATH];
	TCHAR m_Path2[MAX_PATH];

	if (GetModuleFileName(NULL, m_Path, MAX_PATH))    //���s�t�@�C���̃t���p�X���擾
	{   //�擾�ɐ���
		TCHAR* ptmp = _tcsrchr(m_Path, _T('\\')); // \�̍Ō�̏o���ʒu���擾
		if (ptmp != NULL)
		{   //�t�@�C�������폜
			*ptmp = _T('\0');

			sprintf_s(m_Path2, MAX_PATH, "%s\\Resources", m_Path);
			if (!PathFileExists(m_Path2) || !PathIsDirectory(m_Path2))
			{
				TCHAR* ptmp = _tcsrchr(m_Path, _T('\\')); // \�̍Ō�̏o���ʒu���擾
				if (ptmp != NULL)
				{   //�t�@�C�������폜
					*ptmp = _T('\0');
				}
			}
			sprintf_s(m_Path2, MAX_PATH, "%s\\Resources", m_Path);
			if (PathFileExists(m_Path2) && PathIsDirectory(m_Path2))
				SetCurrentDirectory(m_Path);
		}
	}
}

// <�v���O�����̏I���|�C���g>
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CLOSE:
		if (GetWindowModeFlag())
		{
			ShowWindow(hWnd, SW_MINIMIZE);
			Sleep(500);
		}
		break;
	}

	return 0;
}

//----------------------------------------------------------------------
//! @brief �v���O�����̃G���g���[�|�C���g
//!
//! @param[in] hInstance     ���݂̃C���X�^���X�̃n���h��
//! @param[in] hPrevInstance �ȑO�̃C���X�^���X�̃n���h��(���NULL)
//! @param[in] lpCmdLine     �R�}���h���C��
//! @param[in] nCmdShow      �E�B���h�E�̕\�����
//!
//! @retval  0 ����I��
//! @retval -1 �ُ�I��
//----------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ���g�p�����̌x���΍�
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nCmdShow);

	UpdateCurrentDir();


#if defined(_DEBUG)
	// �������[���[�N���o�@�\�̃Z�b�g�A�b�v
	SetUpMemoryLeakDetector();
#endif


	// �f�o�b�O���p���O�t�@�C���̏o�̗͂}��
	SetOutApplicationLogValidFlag(FALSE);


	// �N�����[�h�̐ݒ�
#if defined(_DEBUG)
	ChangeWindowMode(TRUE);				// �E�C���h�E���[�h�Ŏ��s
#else
	ChangeWindowMode(FALSE);			// �t���X�N���[���Ŏ��s
	//ChangeWindowMode(TRUE);			// �E�C���h�E���[�h�Ŏ��s
#endif
	SetMainWindowText(GAME_TITLE);		// �E�C���h�E�^�C�g���̐ݒ�
	SetWindowIconID(IDI_ICON1);			// �E�C���h�E�A�C�R���̐ݒ�

	// �I���t�b�N
	SetHookWinProc(WndProc);

	// ������Ԃ̉�ʃ��[�h�̐ݒ�
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);


	// DX���C�u�����̏���������
	if (DxLib_Init() == -1)
	{
		return -1;						// �G���[���N�����璼���ɏI��
	}

	// �E�B���h�E�擾
	hWnd = GetMainWindowHandle();

	// �`���𗠉�ʂɐݒ�
	SetDrawScreen(DX_SCREEN_BACK);

	
	// �Q�[���̏���
	InitializeGame();					// �Q�[���̏���������
	
	while (!ProcessMessage() && !g_close_request)
	{ 
		UpdateSystem();					// �V�X�e���̍X�V
		
		UpdateGame();					// �Q�[���̍X�V����
		RenderGame();					// �Q�[���̕`�揈��

		ScreenFlip();					// ����ʂ̓��e��\��ʂɔ��f
		ClearDrawScreen();				// ����ʂ̏���
	}

	FinalizeGame();						// �Q�[���̏I������


	// DX���C�u�����̏I������
	DxLib_End();


	return 0;							// ����I��
}
