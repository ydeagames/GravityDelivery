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

// <ゴールオブジェクト>
GameObject GameObject_Goal_Create(const Vec2* mouse)
{
	GameObject obj = GameObject_Create(*mouse, Vec2_Create(), Vec2_Create(20, 20));
	obj.sprite = GameSprite_Create(GameTexture_Create(g_resources.texture_planet1, Vec2_Create(), Vec2_Create(26, 26)));
	obj.sprite.size = Vec2_Create(26, 26);
	obj.sprite.num_columns = 8;
	GameSprite_SetFrame(&obj.sprite, 12);
	obj.type = TYPE_GOAL;
	obj.shape = SHAPE_CIRCLE;
	obj.sprite.color = COLOR_RED;
	GameObject_SetSize(&obj, 4, 20);
	return obj;
}

// <スタートオブジェクト>
GameObject GameObject_Start_Create(const Vec2* mouse, const Vec2* vec)
{
	GameObject obj = GameObject_Create(*mouse, *vec, Vec2_Create(10, 10));
	obj.type = TYPE_START;
	GameObject_SetSize(&obj, 1, 5);
	GameTimer_SetRemaining(&obj.count, .5f);
	GameTimer_Resume(&obj.count);
	return obj;
}

// <惑星オブジェクト>
GameObject GameObject_Planet_Create(const Vec2* mouse)
{
	GameObject obj = GameObject_Create(*mouse, Vec2_Create(), Vec2_Create(10, 10));
	obj.sprite = GameSprite_Create(GameTexture_Create(g_resources.texture_planet1, Vec2_Create(), Vec2_Create(26, 26)));
	obj.sprite.size = Vec2_Create(26, 26);
	obj.sprite.num_columns = 8;
	GameSprite_SetFrame(&obj.sprite, 9);
	obj.type = TYPE_PLANET;
	obj.shape = SHAPE_CIRCLE;
	obj.state = 0;
	obj.sprite.color = COLOR_GRAY;
	GameObject_SetSize(&obj, 2, 15);
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
