#include "SceneLogo.h"
#include "GameMain.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameUtils.h"

// �萔�̒�` ==============================================================

// �O���[�o���ϐ��̒�` ====================================================

int g_logo_count;

// �֐��̒�` ==============================================================

// ���S�V�[���̏���������
void InitializeLogo(void)
{
	g_logo_count = 0;

	// ����Đ�
	PlayMovieToGraph(g_resources.movie_logo);
}

// ���S�V�[���̍X�V����
void UpdateLogo(void)
{
	if (IsKeyPressed(PAD_INPUT_2) || g_logo_count++ >= 60 * 3.5f)
		RequestScene(SCENE_TITLE);
}

// ���S�V�[���̕`�揈��
void RenderLogo(void)
{
	// ���S���[�r�[�`��
	{
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)GetPercentValue(1 - GetPercentageRange((float)g_logo_count, 60 * 3, 60 * 3.5f), 255));
			DrawExtendGraph(SCREEN_LEFT, SCREEN_TOP, SCREEN_WIDTH, SCREEN_HEIGHT, g_resources.movie_logo, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)GetPercentValue(1 - GetPercentageRange((float)g_logo_count, 60 * 0, 60 * 0.5f), 255));
			DrawBox(SCREEN_LEFT, SCREEN_TOP, SCREEN_RIGHT, SCREEN_BOTTOM, COLOR_BLACK, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	//DrawFormatString(10, 10, COLOR_WHITE, "���S�V�[��");
	//DrawFormatString(10, 25, COLOR_WHITE, "�J�E���g: %3d / 180", g_logo_count);
}

// ���S�V�[���̏I������
void FinalizeLogo(void)
{
}