#include "SceneTitle.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameMain.h"
#include "GameObject.h"
#include "GameObjectHit.h"
#include "GameObjects.h"
#include "GameUtils.h"
#include "Easings.h"
#include "Vector.h"
#include "GameStage.h"
#include "KeyFrame.h"




// �萔�̒�` ==============================================================

#define STAGEINFO_DIR "./Resources/Stage"

// �O���[�o���ϐ��̒�` ====================================================

static int g_count;
static Vector g_stageinfos;
static Vector g_stageshots;

static int g_select = -1;
static int g_last_select = -1;

static Vector g_field_layers;
static GameTimer g_field_layer_timer;

static GameObject g_title_logo;

static GameObject g_bar;
static GameObject g_filter_screen;
static KeyFrame g_filter_keyframe;


// �֐��̒�` ==============================================================

// �^�C�g���V�[���̏���������
void InitializeTitle(void)
{
	g_count = 0;

	g_stageinfos = Vector_Create(sizeof(StageInfo));
	StageInfo_InitAndSearchFiles(&g_stageinfos, STAGEINFO_DIR, STAGEINFO_DIR"/*.dat");

	g_stageshots = Vector_Create(sizeof(GameObject));
	foreach_start(&g_stageinfos, StageInfo, info)
	{
		GameObject obj = g_field;
		HGRP screenshot = LoadGraph(info->screenshot_filepath);
		obj.sprite = GameSprite_Create(GameTexture_Create(screenshot, Vec2_Create(), obj.size));
		obj.size = Vec2_Scale(&obj.size, .5f);
		obj.sprite.scale = .5f;
		obj.pos.x -= 150;
		Vector_AddLast(&g_stageshots, &obj);
	} foreach_end;

	{
		int i;
		g_field_layers = Vector_Create(sizeof(GameObject));
		for (i = 0; i < 3; i++)
		{
			GameObject obj = g_field;
			obj.sprite = GameSprite_Create(GameTexture_Create(MakeScreen((int)g_field.size.x, (int)g_field.size.y, TRUE), Vec2_Create(), g_field.size));
			Vector_AddLast(&g_field_layers, &obj);
		}
	}

	g_field_layer_timer = GameTimer_Create();
	GameTimer_SetRemaining(&g_field_layer_timer, .1f);
	GameTimer_Resume(&g_field_layer_timer);

	g_title_logo = GameObject_Create(g_field.pos, Vec2_Create(), Vec2_Create(617, 193));
	g_title_logo.sprite = GameSprite_Create(GameTexture_Create(g_resources.texture[6], Vec2_Create(), Vec2_Create(617, 193)));
	g_title_logo.pos.x -= 150;
	g_title_logo.pos.y -= 200;

	g_bar = g_field;
	g_bar.fill = TRUE;
	g_bar.sprite.color = COLOR_GRAY;
	g_bar.size.x = 300;
	g_bar.pos.x = GameObject_GetX(&g_field, RIGHT, -150);

	g_filter_screen = g_field;
	g_filter_screen.sprite = GameSprite_Create(GameTexture_Create(MakeScreen((int)g_filter_screen.size.x, (int)g_filter_screen.size.y, FALSE), Vec2_Create(), g_filter_screen.size));
	g_filter_keyframe = KeyFrame_Create(.1f);
}

// �^�C�g���V�[���̍X�V����
void UpdateTitle(void)
{
	g_count++;

	//if (IsKeyPressed(PAD_INPUT_2))
	//	RequestScene(SCENE_PLAY);

	if (IsMousePressed(MOUSE_INPUT_1))
	{
		if (0 <= g_last_select && Vector_GetSize(&g_stageinfos))
		{
			StageInfo* stage = (StageInfo*)Vector_Get(&g_stageinfos, g_last_select);
			g_selected_stageinfo = *stage;
			PlaySoundMem(g_resources.sound_se[4], DX_PLAYTYPE_BACK);
			RequestScene(SCENE_PLAY, COLOR_GRAY, 1);
		}
	}

	if (IsKeyDown(KEY_INPUT_LCONTROL) && IsKeyPressed(KEY_INPUT_O))
	{
		char str[MAX_PATH];

		// �V�K�t�@�C���̖��O����͂���
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_STR, COLOR_WHITE);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_CURSOR, COLOR_WHITE);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_SELECT_STR, COLOR_WHITE);
		screen_start(DX_SCREEN_FRONT)
		{
			DrawFormatStringFToHandle(GameObject_GetX(&g_field, CENTER_X, -200), GameObject_GetY(&g_field, BOTTOM, -230),
				COLOR_YELLOW, g_resources.font_main, "�X�e�[�W������͂��Ă�������");
		} screen_end;
		if (KeyInputSingleCharString((int)GameObject_GetX(&g_field, CENTER_X, -200), (int)GameObject_GetY(&g_field, BOTTOM, -200), 30, str, TRUE) == 1)
		{
			g_selected_stageinfo = StageInfo_Create(STAGEINFO_DIR, str);
			RequestScene(SCENE_PLAY, COLOR_GRAY, 1);
		}
	}

	if (g_select != -1 && g_select != g_last_select)
		PlaySoundMem(g_resources.sound_se[1], DX_PLAYTYPE_BACK);
	g_last_select = g_select;

	KeyFrame_SetState(&g_filter_keyframe, GameObject_IsHitPoint(&g_bar, &g_raw_mouse));
}

// �^�C�g���V�[���̕`�揈��
void RenderTitle(void)
{
	int list_size = Vector_GetSize(&g_stageinfos);
	int pos = -list_size - 4;

	screen_start(g_filter_screen.sprite.texture.texture)
	{
		ClearDrawScreen();
		{
			{
				float current_parallax = .2f;
				float parallax = .8f;
				Vec2 mouse_offset = Vec2_Scale(&g_raw_mouse, -.02f);
				foreach_start(&g_field_layers, GameObject, layer)
				{
					current_parallax *= parallax;
					GameObject_Field_Render(layer, &Vec2_Scale(&mouse_offset, current_parallax), 12);
				} foreach_end;
			}

			if (GameTimer_IsFinished(&g_field_layer_timer))
			{
				GameTimer_SetRemaining(&g_field_layer_timer, .05f);
				GameTimer_Resume(&g_field_layer_timer);

				foreach_start(&g_field_layers, GameObject, layer)
				{
					screen_start(layer->sprite.texture.texture);
					{
						unsigned int color = GetColor(GetRand(255), GetRand(255), GetRand(255));
						Vec2 pos = Vec2_Create(GetRandRangeF(GameObject_GetX(layer, LEFT), GameObject_GetX(layer, RIGHT)),
							GetRandRangeF(GameObject_GetY(layer, TOP), GameObject_GetY(layer, BOTTOM)));
						DrawCircleAA(pos.x, pos.y, 2, 4, color);
					} screen_end;
				} foreach_end;
			}

			GameObject_Render(&g_title_logo);

			DrawFormatStringToHandle((int)GameObject_GetX(&g_title_logo, LEFT, -120), (int)GameObject_GetY(&g_title_logo, BOTTOM, 180), COLOR_WHITE, g_resources.font_main,
				"�d�̓X�C�b�`�ɂ悤����\n"
				"���̈��͂𑀍삵�ăS�[����ڎw�����I\n"
				"�X�e�[�W��I�����ăQ�[���X�^�[�g\n");
		}
		GraphFilter(g_filter_screen.sprite.texture.texture, DX_GRAPH_FILTER_GAUSS, 16, (int)KeyFrame_GetProgress(&g_filter_keyframe, 256));
	} screen_end;
	GameObject_Render(&g_filter_screen);

	{
		{
			SetDrawBlendMode(DX_BLENDMODE_ADD, (int)KeyFrame_GetProgressRange(&g_filter_keyframe, 128, 256));
			GameObject_Render(&g_bar);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		DrawFormatStringToHandle(SCREEN_RIGHT - 250, SCREEN_BOTTOM + 20 * pos++, COLOR_WHITE, g_resources.font_main, "�X�e�[�W�I��");
		pos++;

		g_select = -1;
		foreach_start(&g_stageinfos, StageInfo, stage)
		{
			GameObject rect = GameObject_Create(Vec2_Create(GameObject_GetX(&g_field, RIGHT, -150), (float)(SCREEN_BOTTOM + 20 * pos + 10)), Vec2_Create(), Vec2_Create(300, 20));
			rect.fill = TRUE;
			rect.sprite.color = COLOR_GRAY;
			if (GameObject_IsHitPoint(&rect, &g_raw_mouse))
			{
				SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
				GameObject_Render(&rect);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				g_select = itr_stage.current - 1;
			}
			DrawFormatStringToHandle(SCREEN_RIGHT - 250, SCREEN_BOTTOM + 20 * pos, COLOR_WHITE, g_resources.font_main, stage->title);
			pos++;
		} foreach_end;

		if (0<=g_select&&g_select<Vector_GetSize(&g_stageshots))
		{
			GameObject* obj = (GameObject*) Vector_Get(&g_stageshots, g_select);
			if (obj->sprite.texture.texture != TEXTURE_MISSING)
			{
				GameObject frame = *obj;
				frame.sprite = GameSprite_CreateNone();
				frame.sprite.color = COLOR_WHITE;
				frame.fill = TRUE;
				frame.size = Vec2_Add(&frame.size, &Vec2_Create(20, 20));
				GameObject_Render(&frame);
				GameObject_Render(obj);
			}
		}
	}
}

// �^�C�g���V�[���̏I������
void FinalizeTitle(void)
{
	foreach_start(&g_field_layers, GameObject, layer)
	{
		DeleteGraph(layer->sprite.texture.texture);
	} foreach_end;
	DeleteGraph(g_filter_screen.sprite.texture.texture);

	foreach_start(&g_stageshots, GameObject, stageshot)
	{
		DeleteGraph(stageshot->sprite.texture.texture);
	} foreach_end;
	Vector_Delete(&g_stageshots);

	Vector_Delete(&g_stageinfos);
}