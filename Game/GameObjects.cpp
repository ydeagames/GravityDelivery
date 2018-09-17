#include "GameObjects.h"
#include "GameMain.h"
#include "GameUtils.h"
#include <math.h>

// 定数の定義 ==============================================================

// <敵>
#define ENEMY_WIDTH 32
#define ENEMY_HEIGHT 32

// <プレイヤー>
#define PLAYER_WIDTH  42
#define PLAYER_HEIGHT 36

// 関数の定義 ==============================================================

// <オブジェクトサイズ変更>
void GameObject_SetSize(GameObject* obj, float scale, float size)
{
	obj->sprite.scale = scale;
	obj->size = Vec2_Create(size * scale, size * scale);
}

// <弾オブジェクト>
GameObject GameObject_Ball_Create(const Vec2* mouse, const Vec2* vec)
{
	GameObject obj = GameObject_Create(*mouse, *vec, Vec2_Create(5, 5));
	obj.sprite = GameSprite_Create(GameTexture_Create(g_resources.texture_planet2, Vec2_Create(), Vec2_Create(26, 26)));
	obj.sprite.num_columns = 9;
	GameSprite_SetFrame(&obj.sprite, 11);
	return obj;
}

// <ゴールオブジェクト>
GameObject GameObject_Goal_Create(const Vec2* mouse)
{
	GameObject obj = GameObject_Create(*mouse, Vec2_Create(), Vec2_Create(20, 20));
	obj.sprite = GameSprite_Create(GameTexture_Create(g_resources.texture_planet3, Vec2_Create(), Vec2_Create(34, 34)));
	obj.sprite.num_columns = 6;
	GameSprite_SetFrame(&obj.sprite, 17);
	obj.type = TYPE_GOAL;
	obj.shape = SHAPE_CIRCLE;
	obj.sprite.color = COLOR_RED;
	GameObject_SetSize(&obj, 2, 32);
	return obj;
}

// <スタートオブジェクト>
GameObject GameObject_Start_Create(const Vec2* mouse, const Vec2* vec)
{
	GameObject obj = GameObject_Create(*mouse, *vec, Vec2_Create(10, 10));
	obj.sprite = GameSprite_Create(GameTexture_Create(g_resources.texture_planet1, Vec2_Create(), Vec2_Create(26, 26)));
	obj.sprite.num_columns = 8;
	GameSprite_SetFrame(&obj.sprite, 12);
	obj.type = TYPE_START;
	GameObject_SetSize(&obj, 3, 5);
	GameTimer_SetRemaining(&obj.count, .5f);
	GameTimer_Resume(&obj.count);
	return obj;
}

// <惑星オブジェクト>
GameObject GameObject_Planet_Create(const Vec2* mouse)
{
	GameObject obj = GameObject_Create(*mouse, Vec2_Create(), Vec2_Create(10, 10));
	obj.sprite = GameSprite_Create(GameTexture_Create(g_resources.texture_planet1, Vec2_Create(), Vec2_Create(26, 26)));
	obj.sprite.texture.center.x -= 0;
	obj.sprite.texture.center.y += 1.5f;
	obj.sprite.num_columns = 8;
	GameSprite_SetFrame(&obj.sprite, GetRand(7));
	obj.type = TYPE_PLANET;
	obj.shape = SHAPE_CIRCLE;
	obj.state = 0;
	obj.sprite.color = COLOR_GRAY;
	GameObject_SetSize(&obj, 2, 8);
	return obj;
}

// <ビームオブジェクト>
GameObject GameObject_Beam_Create(const Vec2* mouse, const Vec2* next)
{
	GameObject obj = GameObject_Create(*mouse, *next, Vec2_Create(10, 10));
	obj.type = TYPE_BEAM;
	obj.sprite.color = COLOR_YELLOW;
	return obj;
}

// <オブジェクト>
GameObject GameObject_Type_Create(int type, const Vec2* mouse, const Vec2* vec)
{
	switch (type)
	{
	default:
	case TYPE_PLANET:
		return GameObject_Planet_Create(mouse);
	case TYPE_START:
		return GameObject_Start_Create(mouse, vec);
	case TYPE_GOAL:
		return GameObject_Goal_Create(mouse);
	case TYPE_BEAM:
		return GameObject_Beam_Create(mouse, vec);
	}
}

// <オブジェクトループ描画>
void GameObject_RenderLoop(const GameObject* obj, const GameObject* tile_area, GameObject* tile_obj, const Vec2* translate)
{
	float box_xc = GameObject_GetX(obj, CENTER_X) + translate->x;
	float box_ym = GameObject_GetY(obj, CENTER_Y) + translate->y;
	Vec2 box_t = Vec2_Create(box_xc, box_ym);
	float go_left = GameObject_GetX(obj, LEFT);
	float go_right = GameObject_GetX(obj, RIGHT);
	float go_top = GameObject_GetY(obj, TOP);
	float go_bottom = GameObject_GetY(obj, BOTTOM);

	float sp_left = GameObject_GetX(tile_area, LEFT);
	float sp_right = GameObject_GetX(tile_area, RIGHT);
	float sp_top = GameObject_GetY(tile_area, TOP);
	float sp_bottom = GameObject_GetY(tile_area, BOTTOM);

	Vec2 sp_pos = Vec2_Add(&box_t, &obj->sprite.offset);
	Vec2 sp_size = Vec2_Scale(&obj->sprite.texture.size, obj->sprite.scale);
	float offset_x = GetLoopRangeF(go_left, sp_left, sp_right) - sp_left;
	float offset_y = GetLoopRangeF(go_top, sp_top, sp_bottom) - sp_top;

	if (sp_size.x >= 1.f && sp_size.y >= 1.f)
	{
		for (float iy = go_top + sp_size.y / 2 - offset_y/* - center_offset.y*/; iy < go_bottom; iy += sp_size.y)
		{
			for (float ix = go_left + sp_size.x / 2 - offset_x/* - center_offset.x*/; ix < go_right; ix += sp_size.x)
			{
				GameObject_Render(tile_obj, translate);
			}
		}
	}
}

// <フィールド描画>
void GameObject_Field_Render(const GameObject* field, const Vec2* translate, int fadeout)
{
	GameObject_Render(field, translate);
	SetDrawScreen(field->sprite.texture.texture);
	{
		GameObject fieldback = *field;
		fieldback.sprite = GameSprite_CreateNone();
		fieldback.sprite.color = COLOR_BLACK;
		fieldback.fill = TRUE;
		fieldback.pos = Vec2_Scale(&fieldback.size, .5f);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeout);
		GameObject_Render(&fieldback);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	SetDrawScreen(DX_SCREEN_BACK);
}

// <メッセージ描画>
void GameObject_Msg_Render(const Vec2* pos, const Vec2* translate, const char* msg)
{
	int margin = 12;
	int triangle = 20;
	Vec2 size = Vec2_Create((float)(GetDrawStringWidthToHandle(msg, strlen(msg), g_resources.font_main) + margin * 2), (float)(FONT_SIZE_MAIN + margin * 2));
	GameObject obj = GameObject_Create(Vec2_Create(pos->x + size.x / 2, pos->y - size.y / 2 - triangle + 1), Vec2_Create(), size);
	obj.fill = TRUE;
	GameObject_Render(&obj, translate);
	DrawTriangleAA(pos->x + translate->x, pos->y + translate->y,
		pos->x + translate->x, pos->y - triangle + translate->y,
		pos->x + triangle + translate->x, pos->y - triangle + translate->y,
		obj.sprite.color, TRUE);
	DrawStringFToHandle(GameObject_GetX(&obj, LEFT, (float)-margin) + translate->x, GameObject_GetY(&obj, TOP, (float)-margin) + translate->y, msg, COLOR_BLACK, g_resources.font_main);
}
