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
#include <direct.h>
#include <io.h>




// 定数の定義 ==============================================================

// グローバル変数の定義 ====================================================

static int g_count;
static Vector g_stageinfos;

static int g_last_select = -1;

static Vector g_field_layers;
static GameTimer g_field_layer_timer;

static GameObject g_title_logo;

// 関数の定義 ==============================================================

static StageInfo Stage_Create(const char* dirpath, const char* name)
{
	StageInfo stage;
	strcpy_s(stage.filename, name);
	snprintf(stage.filepath, MAX_PATH, "%s/%s", dirpath, name);
	{
		char *lastdot;
		strcpy(stage.title, stage.filename);
		lastdot = strrchr(stage.title, '.');
		if (lastdot != NULL)
			*lastdot = '\0';
	}
	return stage;
}

static void listfiles(Vector* lists, char* path, char* filter)
{
	struct _finddata_t fdata;

	intptr_t fh = _findfirst(filter, &fdata);
	Vector_Clear(lists);
	if (fh != -1)
	{
		do {
			if ((fdata.attrib & _A_SUBDIR) == 0)
			{
				StageInfo stage = Stage_Create(path, fdata.name);
				Vector_AddLast(lists, &stage);
			}
		} while (_findnext(fh, &fdata) == 0);
		_findclose(fh);
	}
}

// タイトルシーンの初期化処理
void InitializeTitle(void)
{
	g_count = 0;
	g_stageinfos = Vector_Create(sizeof(StageInfo));

	_mkdir("./Resources/Stage");
	listfiles(&g_stageinfos, "./Resources/Stage", "./Resources/Stage/*.dat");

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
}

// タイトルシーンの更新処理
void UpdateTitle(void)
{
	g_count++;

	//if (IsKeyPressed(PAD_INPUT_2))
	//	RequestScene(SCENE_PLAY);

	if (IsMousePressed(MOUSE_INPUT_1))
	{
		if (0 <= g_last_select)
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

		// 新規ファイルの名前を入力する
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_STR, COLOR_WHITE);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_NORMAL_CURSOR, COLOR_WHITE);
		SetKeyInputStringColor2(DX_KEYINPSTRCOLOR_SELECT_STR, COLOR_WHITE);
		SetDrawScreen(DX_SCREEN_FRONT);
		DrawFormatStringFToHandle(GameObject_GetX(&g_field, CENTER_X, -200), GameObject_GetY(&g_field, BOTTOM, -230),
			COLOR_YELLOW, g_resources.font_main, "ステージ名を入力してください");
		SetDrawScreen(DX_SCREEN_BACK);
		if (KeyInputSingleCharString((int)GameObject_GetX(&g_field, CENTER_X, -200), (int)GameObject_GetY(&g_field, BOTTOM, -200), 30, str, TRUE) == 1)
		{
			strcat_s(str, sizeof(g_selected_stageinfo.filename), ".dat");
			g_selected_stageinfo = Stage_Create("./Resources/Stage", str);
			RequestScene(SCENE_PLAY, COLOR_GRAY, 1);
		}
	}
}

// タイトルシーンの描画処理
void RenderTitle(void)
{
	int select = -1;
	int list_size = Vector_GetSize(&g_stageinfos);
	int pos = -list_size - 4;

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
			SetDrawScreen(layer->sprite.texture.texture);
			{
				unsigned int color = GetColor(GetRand(255), GetRand(255), GetRand(255));
				Vec2 pos = Vec2_Create(GetRandRangeF(GameObject_GetX(layer, LEFT), GameObject_GetX(layer, RIGHT)),
					GetRandRangeF(GameObject_GetY(layer, TOP), GameObject_GetY(layer, BOTTOM)));
				DrawCircleAA(pos.x, pos.y, 2, 4, color);
			}
			SetDrawScreen(DX_SCREEN_BACK);
		} foreach_end;
	}

	{
		GameObject bar = g_field;
		bar.fill = TRUE;
		bar.sprite.color = COLOR_GRAY;
		bar.size.x = 300;
		bar.pos.x = GameObject_GetX(&g_field, RIGHT, -150);
		SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
		GameObject_Render(&bar);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	GameObject_Render(&g_title_logo);

	DrawFormatStringToHandle((int)GameObject_GetX(&g_title_logo, LEFT, -120), (int)GameObject_GetY(&g_title_logo, BOTTOM, 180), COLOR_WHITE, g_resources.font_main,
		"重力スイッチにようこそ\n"
		"星の引力を操作してゴールを目指そう！\n"
		"ステージを選択してゲームスタート\n");

	DrawFormatStringToHandle(SCREEN_RIGHT - 250, SCREEN_BOTTOM + 20 * pos++, COLOR_WHITE, g_resources.font_main, "ステージ選択");
	pos++;

	foreach_start(&g_stageinfos, StageInfo, stage)
	{
		GameObject rect = GameObject_Create(Vec2_Create(GameObject_GetX(&g_field, RIGHT, -150), (float)(SCREEN_BOTTOM + 20 * pos + 10)), Vec2_Create(), Vec2_Create(300, 20));
		rect.fill = TRUE;
		rect.sprite.color = COLOR_GRAY;
		if (GameObject_IsHitPoint(&rect, &GetMousePosition()))
		{
			SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
			GameObject_Render(&rect);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			select = itr_stage.current - 1;
		}
		DrawFormatStringToHandle(SCREEN_RIGHT - 250, SCREEN_BOTTOM + 20 * pos, COLOR_WHITE, g_resources.font_main, stage->title);
		pos++;
	} foreach_end;

	if (select != g_last_select)
		PlaySoundMem(g_resources.sound_se[1], DX_PLAYTYPE_BACK);
	g_last_select = select;
}

// タイトルシーンの終了処理
void FinalizeTitle(void)
{
	foreach_start(&g_field_layers, GameObject, layer)
	{
		DeleteGraph(layer->sprite.texture.texture);
	} foreach_end;

	Vector_Delete(&g_stageinfos);
}