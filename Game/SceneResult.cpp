#include "SceneTitle.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameMain.h"
#include "GameObject.h"
#include "GameObjectHit.h"
#include "GameObjects.h"
#include "GameUtils.h"
#include "Easings.h"

// 定数の定義 ==============================================================

// グローバル変数の定義 ====================================================

static GameObject g_back_button;

static Vector g_field_layers;
static GameTimer g_field_layer_timer;

static GameObject g_title_logo;

// 関数の定義 ==============================================================

// タイトルシーンの初期化処理
void InitializeResult(void)
{
	Vec2 size = Vec2_Create(150, 50);
	g_back_button = GameObject_Create(Vec2_Create(GameObject_GetX(&g_field, LEFT, -size.x / 2 - 50), GameObject_GetY(&g_field, BOTTOM, -size.y / 2 - 50)), Vec2_Create(), size);
	g_back_button.fill = TRUE;
	g_back_button.sprite.color = COLOR_GRAY;

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

	ChangeVolumeSoundMem(150, g_resources.sound_se[7]);
	PlaySoundMem(g_resources.sound_se[7], DX_PLAYTYPE_BACK);

	{
		TCHAR title[260];
		sprintf(title, "%s - %s - クリア", GAME_TITLE, g_selected_stage.title);
		SetMainWindowText(title);		// ウインドウタイトルの設定
	}
}

// タイトルシーンの更新処理
void UpdateResult(void)
{
	//if (IsKeyPressed(PAD_INPUT_2))
	//	RequestScene(SCENE_TITLE);

	if (IsMousePressed(MOUSE_INPUT_1))
	{
		if (GameObject_IsHitPoint(&g_back_button, &GetMousePosition()))
		{
			RequestScene(SCENE_TITLE, COLOR_GRAY, .5f);
			PlaySoundMem(g_resources.sound_se[5], DX_PLAYTYPE_BACK);
		}
	}
	if (GameObject_IsHitPoint(&g_back_button, &g_raw_mouse) && !GameObject_IsHitPoint(&g_back_button, &g_raw_mouse_last))
		PlaySoundMem(g_resources.sound_se[1], DX_PLAYTYPE_BACK);
}

// タイトルシーンの描画処理
void RenderResult(void)
{
	int pos = -7;
	{
		float current_parallax = .2f;
		float parallax = .8f;
		Vec2 mouse_offset = Vec2_Scale(&g_raw_mouse, -.02f);
		foreach_start(&g_field_layers, GameObject, layer)
		{
			current_parallax *= parallax;
			GameObject_Field_Render(layer, &Vec2_Scale(&mouse_offset, current_parallax), 1);
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

	DrawFormatStringToHandle(SCREEN_RIGHT - 270, SCREEN_BOTTOM + 20 * pos++, COLOR_WHITE, g_resources.font_main, "プレイいただき\n   ありがとうございます。");
	pos++;
	pos++;
	DrawFormatStringToHandle(SCREEN_RIGHT - 270, SCREEN_BOTTOM + 20 * pos++, COLOR_WHITE, g_resources.font_main, "引き続き次のステージを\n   お楽しみください。");
	pos++;

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

	pos = 0;
	{
		GameObject bar = GameObject_Create(g_field.pos, Vec2_Create(), Vec2_Create(SCREEN_WIDTH, 60));
		bar.sprite.color = COLOR_GRAY;
		bar.fill = TRUE;
		SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
		GameObject_Render(&bar);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		{
			char name[260];
			char *lastdot;
			strcpy(name, g_selected_stage.filename);
			lastdot = strrchr(name, '.');
			if (lastdot != NULL)
				*lastdot = '\0';
			DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_CENTER_Y - 20 + 20 * pos++, COLOR_WHITE, g_resources.font_main, "ステージクリア");
			DrawFormatStringToHandle(SCREEN_CENTER_X - 200, SCREEN_CENTER_Y - 20 + 20 * pos++, COLOR_WHITE, g_resources.font_main, "%s", name);
		}
	}

	{
		if (GameObject_IsHitPoint(&g_back_button, &GetMousePosition()))
			GameObject_Render(&g_back_button);
		DrawFormatStringToHandle((int)GameObject_GetX(&g_back_button, LEFT, -10), (int)GameObject_GetY(&g_back_button, TOP, -20), COLOR_WHITE, g_resources.font_main, "タイトルへ戻る");
	}
}

// タイトルシーンの終了処理
void FinalizeResult(void)
{
	foreach_start(&g_field_layers, GameObject, layer)
	{
		DeleteGraph(layer->sprite.texture.texture);
	} foreach_end;

	SetMainWindowText(GAME_TITLE);		// ウインドウタイトルの設定
}