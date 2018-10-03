#include "GamePause.h"
#include "InputManager.h"
#include "KeyFrame.h"
#include "GameObject.h"
#include "GameObjectHit.h"
#include "GameMain.h"
#include "SceneManager.h"
#include "ScreenShot.h"

// �萔�̒�` ==============================================================

#define NUM_MENU_ITEMS 5

// �O���[�o���ϐ��̒�` ====================================================

GameObject g_backscreen;
static BOOL g_paused;
static KeyFrame g_menu_keyframe;
static EasingKeyFrame g_menu_keyframe_menu;
static EasingKeyFrame g_menu_keyframe_back;
static GameObject g_menu;
static char* g_menu_items[NUM_MENU_ITEMS] = { "�Q�[���ɖ߂�", "�^�C�g���ɖ߂�", "�X�N���[���V���b�g���B��", "�X�N���[���V���b�g������", "�Q�[�����I������" };
static int g_last_select = -1;
static BOOL screenshot_task = FALSE;
static BOOL mouse_on_menu = FALSE;

// �֐��̐錾 ==============================================================

// �|�[�Y�̏���������
void InitializeGamePause(void)
{
	g_backscreen = g_field;
	g_backscreen.sprite = GameSprite_Create(GameTexture_Create(MakeScreen((int)g_backscreen.size.x, (int)g_backscreen.size.y, FALSE), Vec2_Create(), g_backscreen.size));

	g_paused = FALSE;
	g_menu_keyframe = KeyFrame_Create(.5f);
	g_menu_keyframe_menu = EasingKeyFrame_CreateWith2Easings(&g_menu_keyframe, ESG_OUTBOUNCE, ESG_OUTBOUNCE);
	g_menu_keyframe_back = EasingKeyFrame_CreateWith2Easings(&g_menu_keyframe, ESG_OUTQUART, ESG_OUTQUART);

	g_menu = g_field;
	g_menu.fill = TRUE;
	g_menu.sprite.color = COLOR_WHITE;
	g_menu.size.x = 300;
	g_menu.pos.x = GameObject_GetX(&g_field, RIGHT, -150);
}

// �|�[�Y�̍X�V����
void UpdateGamePause(void)
{
	if (IsKeyPressed(KEY_INPUT_ESCAPE))
	{
		if (!g_paused)
			RequestPause();
		else
		{
			g_paused = FALSE;
			PlaySoundMem(g_resources.sound_se[5], DX_PLAYTYPE_BACK);
		}
	}

	KeyFrame_SetState(&g_menu_keyframe, g_paused);
	if (g_paused)
	{
		if (IsMousePressed(MOUSE_INPUT_1))
		{
			switch (g_last_select)
			{
			case 0:
				g_paused = FALSE;
				PlaySoundMem(g_resources.sound_se[5], DX_PLAYTYPE_BACK);
				break;
			case 1:
				if (GetSceneID() != SCENE_TITLE)
				{
					RequestScene(SCENE_TITLE, COLOR_GRAY, .5f);
					g_paused = FALSE;
					PlaySoundMem(g_resources.sound_se[5], DX_PLAYTYPE_BACK);
				}
				break;
			case 2:
				screenshot_task = TRUE;
				break;
			case 3:
				OpenScreenShotDir();
				break;
			case 4:
				RequestClose();
				break;
			default:
				if (!mouse_on_menu)
				{
					g_paused = FALSE;
					PlaySoundMem(g_resources.sound_se[5], DX_PLAYTYPE_BACK);
				}
				break;
			}
			ConsumeMouse(MOUSE_INPUT_1);
		}
	}
}

// �|�[�Y�̕`�揈��
void RenderGamePause(void)
{
	int select = -1;
	int pos = -NUM_MENU_ITEMS - 4;
	int i;

	Vec2 offset = Vec2_Create(EasingKeyFrame_GetProgressRange(&g_menu_keyframe_menu, g_menu.size.x, 0));
	Vec2 mouse = Vec2_Add(&g_raw_mouse, &Vec2_Negate(&offset));

	if (screenshot_task)
	{
		GameObject_Render(&g_backscreen);
		SaveScreenShotToPNG_Log(&g_field);
		screenshot_task = FALSE;
	}

	GraphFilter(g_backscreen.sprite.texture.texture, DX_GRAPH_FILTER_GAUSS, 16, (int)EasingKeyFrame_GetProgress(&g_menu_keyframe_back, 512));
	GameObject_Render(&g_backscreen);

	{
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
			GameObject_Render(&g_menu, &offset);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		DrawFormatStringToHandle((int)(SCREEN_RIGHT - 275 + offset.x), (int)(SCREEN_BOTTOM + 20 * pos++ + offset.y), COLOR_BLACK, g_resources.font_main, "���j���[");
		pos++;

		for (i = 0; i < NUM_MENU_ITEMS; i++)
		{
			GameObject rect = GameObject_Create(Vec2_Create(GameObject_GetX(&g_field, RIGHT, -150), (float)(SCREEN_BOTTOM + 20 * pos + 10)), Vec2_Create(), Vec2_Create(300, 20));
			rect.fill = TRUE;
			rect.sprite.color = COLOR_GRAY;
			if (GameObject_IsHitPoint(&rect, &mouse))
			{
				SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
				GameObject_Render(&rect, &offset);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				select = i;
			}
			DrawFormatStringToHandle((int)(SCREEN_RIGHT - 275 + offset.x), (int)(SCREEN_BOTTOM + 20 * pos + offset.y),
				(i==1&&GetSceneID()==SCENE_TITLE) ? COLOR_GRAY : COLOR_BLACK, g_resources.font_main, g_menu_items[i]);
			pos++;
		}
	}

	mouse_on_menu = GameObject_IsHitPoint(&g_menu, &mouse);

	if (g_paused && GetSceneID() == SCENE_PLAY)
		DrawFormatStringToHandle(SCREEN_CENTER_X - 100, SCREEN_CENTER_Y - 50, COLOR_WHITE, g_resources.font_main, "Paused");

	if (select != g_last_select)
		PlaySoundMem(g_resources.sound_se[1], DX_PLAYTYPE_BACK);
	g_last_select = select;
}

// �|�[�Y�̏I������
void FinalizeGamePause(void)
{
	DeleteGraph(g_backscreen.sprite.texture.texture);
}

// �|�[�Y�v��
void RequestPause(void)
{
	g_paused = TRUE;
	ChangeVolumeSoundMem(100, g_resources.sound_se[3]);
	PlaySoundMem(g_resources.sound_se[3], DX_PLAYTYPE_BACK);
}

// �|�[�Y����
BOOL IsGamePaused(void)
{
	return g_paused;
}
