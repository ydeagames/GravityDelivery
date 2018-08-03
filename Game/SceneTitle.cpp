#include "SceneTitle.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameMain.h"
#include "GameObject.h"
#include "GameUtils.h"
#include "Easings.h"

// �萔�̒�` ==============================================================

// �O���[�o���ϐ��̒�` ====================================================

static int g_count;

// �֐��̒�` ==============================================================

// �^�C�g���V�[���̏���������
void InitializeTitle(void)
{
	g_count = 0;
}

// �^�C�g���V�[���̍X�V����
void UpdateTitle(void)
{
	g_count++;

	if (IsKeyPressed(PAD_INPUT_2))
		RequestScene(SCENE_PLAY);
}

// �^�C�g���V�[���̕`�揈��
void RenderTitle(void)
{
	int bright = (int)GetEasingValue(ESG_INCUBIC, GetPercentageRange((float)g_count, 60 * 0, 60 * 0.5f), 255);		// �t�F�[�h�C��
	SetDrawBright(bright, bright, bright);
	{
		DrawRotaGraph(SCREEN_CENTER_X, SCREEN_CENTER_Y, 1, 0, g_resources.texture_title, TRUE);
		{
			int duration = 90;
			int idle = 30;

			DrawRotaGraph(SCREEN_CENTER_X, SCREEN_CENTER_Y + 96, 1, 0, g_resources.texture_title_text, TRUE);
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 
					(int)GetEasingValueRange(ESG_INCIRC, GetPercentageRange((float)(g_count % (duration + idle)), 0, (float)duration), 255, 32));
				DrawRotaGraph(SCREEN_CENTER_X, SCREEN_CENTER_Y + 96,
					GetEasingValueRange(ESG_LINEAR, GetPercentageRange((float)(g_count % (duration + idle)), 0, (float)duration), 1.1f, 1.f),
					0, g_resources.texture_title_text, TRUE
				);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}

		//DrawFormatString(10, 10, COLOR_WHITE, "�^�C�g���V�[��");
		//DrawFormatString(10, 25, COLOR_WHITE, "�J�E���g: %3d / 180", g_title_count);

		{
			GameObject box = GameObject_Field_Create();
			box.sprite.color = COLOR_BLACK;
			box.pos.y += 5;
			box.size.x -= 100;
			box.size.y = 110;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
			GameObject_Render(&box);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		DrawFormatStringToHandle(SCREEN_LEFT + 70, SCREEN_CENTER_Y - 35 + 20 * 0, COLOR_WHITE, g_resources.font_menu, "�@�}�E�X��WASD�L�[�ő��삵�܂��B�}�E�X���q���ăl�I");
		DrawFormatStringToHandle(SCREEN_LEFT + 70, SCREEN_CENTER_Y - 35 + 20 * 1, COLOR_WHITE, g_resources.font_menu, "�A�������Ń`���[�W�e���I�e���������񂠂āA�l���̓G��|���܂�");
		DrawFormatStringToHandle(SCREEN_LEFT + 70, SCREEN_CENTER_Y - 35 + 20 * 2, COLOR_WHITE, g_resources.font_menu, "�B�l���̓G����G�������܂�");
		DrawFormatStringToHandle(SCREEN_LEFT + 70, SCREEN_CENTER_Y - 35 + 20 * 3, COLOR_WHITE, g_resources.font_menu, "�C�N���b�N���Ēe���Ԃ��悤�I");
	}
	SetDrawBright(255, 255, 255);
}

// �^�C�g���V�[���̏I������
void FinalizeTitle(void)
{
}