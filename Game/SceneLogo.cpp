#include "SceneLogo.h"
#include "GameMain.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameUtils.h"

// �萔�̒�` ==============================================================

// �O���[�o���ϐ��̒�` ====================================================

static GameTimer g_logo_count;

// �֐��̒�` ==============================================================

// ���S�V�[���̏���������
void InitializeLogo(void)
{
	g_logo_count = GameTimer_Create();
	GameTimer_SetRemaining(&g_logo_count, 3.f);
	GameTimer_Resume(&g_logo_count);

	// ����Đ�
	PlayMovieToGraph(g_resources.movie_logo);
}

// ���S�V�[���̍X�V����
void UpdateLogo(void)
{
	if (IsMousePressed(MOUSE_INPUT_1) || GameTimer_IsFinished(&g_logo_count))
		RequestScene(SCENE_TITLE, COLOR_BLACK, 0, 1);
}

// ���S�V�[���̕`�揈��
void RenderLogo(void)
{
	// ���S���[�r�[�`��
	{
		{
			int bright = (int) GetMinF(
				GetPercentValue(1 - GetPercentageRange(GameTimer_GetRemaining(&g_logo_count), .5f, 0.f), 255),
				GetPercentValue(GetPercentageRange(GameTimer_GetElapsed(&g_logo_count), 0.f, .5f), 255)
			);
			SetDrawBright(bright, bright, bright);
			DrawBox(SCREEN_LEFT, SCREEN_TOP, SCREEN_RIGHT, SCREEN_BOTTOM, COLOR_WHITE, TRUE);
			DrawRotaGraph(SCREEN_CENTER_X, SCREEN_CENTER_Y, 1, 0, g_resources.movie_logo, TRUE);
			SetDrawBright(255, 255, 255);
		}
	}

	//DrawFormatString(10, 10, COLOR_WHITE, "���S�V�[��");
	//DrawFormatString(10, 25, COLOR_WHITE, "�J�E���g: %3d / 180", g_logo_count);
}

// ���S�V�[���̏I������
void FinalizeLogo(void)
{
}