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

// <弾>
#define BULLET_WIDTH  10
#define BULLET_HEIGHT 10
#define BULLET_GROW_SPEED .1f

// 関数の定義 ==============================================================

// <<敵オブジェクト>> ----------------------------------------------

// <敵オブジェクト作成>
GameObject GameObject_Enemy_Create(int texture)
{
	struct FrameSet {
		int start;
		int end;
	};
	struct FrameSet sets[16] = {
		{ 0 + 0,6 + 0 },
		{ 7 + 1,14 + 1 },
		{ 15 + 1,21 + 1 },{ 22 + 1,22 + 1 },
		{ 23 + 1,29 + 1 },{ 30 + 1,30 + 1 },
		{ 31 + 1,34 + 1 },{ 35 + 1,38 + 1 },
		{ 39 + 1,42 + 1 },{ 43 + 1,46 + 1 },
		{ 47 + 1,52 + 1 },
		{ 53 + 3,58 + 3 },{ 59 + 3,59 + 3 },
		{ 60 + 4,63 + 4 },{ 64 + 4,67 + 4 },
		{ 68 + 4,73 + 4 }
	};

	GameObject obj = GameObject_Create(Vec2_Create(), Vec2_Create(), Vec2_Create(ENEMY_WIDTH, ENEMY_HEIGHT));
	obj.type = TYPE_UNKNOWN;
	obj.shape = SHAPE_CIRCLE;
	obj.sprite = GameSprite_Create(GameTexture_Create(g_resources.texture_enemy, Vec2_Create(0, 0), Vec2_Create(36, 36)));
	obj.sprite.animation = GameSpriteAnimation_Create(sets[texture].start, sets[texture].end, 8, 8);
	return obj;
}

// <敵オブジェクト座標Xデフォルト>
void GameObject_Enemy_SetPosDefault(GameObject* obj, const GameObject* field)
{
	obj->pos.x = GetRandRangeF(GameObject_GetX(field, LEFT), GameObject_GetX(field, RIGHT));
	obj->pos.y = GameObject_GetY(field, TOP, 20);
}

// <敵オブジェクト移動Xデフォルト>
void GameObject_Enemy_SetVelDefault(GameObject* obj)
{
	obj->vel = Vec2_Create(0, 5);
}

// <敵オブジェクト更新>
void GameObject_Enemy_Update(GameObject* obj)
{
	if (obj->type == TYPE_UNKNOWN)
	{
		float radius = 15;
		obj->vel.x = cosf(obj->pos.y / radius) * radius;
	}
}

// <<プレイヤーオブジェクト>> ----------------------------------------------

// <プレイヤーオブジェクト作成>
GameObject GameObject_Player_Create(void)
{
	GameObject obj = GameObject_Create(Vec2_Create(), Vec2_Create(), Vec2_Create(PLAYER_WIDTH, PLAYER_HEIGHT));
	obj.type = TYPE_UNKNOWN;
	return obj;
}

// <プレイヤーオブジェクト座標Yデフォルト>
void GameObject_Player_SetPosYDefault(GameObject* obj, const GameObject* field)
{
	obj->pos.y = GameObject_OffsetY(obj, TOP, GameObject_GetY(field, BOTTOM), 16);
}

// <<弾オブジェクト>> ----------------------------------------------

// <弾オブジェクト作成>
GameObject GameObject_Bullet_Create(void)
{
	GameObject obj = GameObject_Create(Vec2_Create(), Vec2_Create(), Vec2_Create(BULLET_WIDTH, BULLET_HEIGHT));
	obj.shape = SHAPE_CIRCLE;
	obj.sprite = GameSprite_Create(GameTexture_Create(g_resources.texture_bullet, Vec2_Create(0, 0), Vec2_Create(32, 32)));
	obj.type = TYPE_UNKNOWN;
	return obj;
}

// <弾オブジェクト座標デフォルト>
void GameObject_Bullet_SetPosDefault(GameObject* obj, const GameObject* player)
{
	obj->pos = player->pos;
	//obj->pos.y = GameObject_OffsetY(obj, BOTTOM, GameObject_GetY(player, BOTTOM));
}

// <弾オブジェクト移動デフォルト>
void GameObject_Bullet_SetVelDefault(GameObject* obj, float offset_angle, int num, int total)
{
	int n = 270 - (total - 1) * 10 + num * 20;
	float angle = ToRadians((float)n);
	obj->vel.x = BULLET_VEL * cosf(offset_angle + angle);
	obj->vel.y = BULLET_VEL * sinf(offset_angle + angle);
}

// <弾オブジェクトサイズ変更>
void GameObject_Bullet_SetSize(GameObject* obj, float scale)
{
	obj->sprite.scale = scale;
	obj->size = Vec2_Create(BULLET_WIDTH * scale, BULLET_HEIGHT * scale);
}

// <弾オブジェクト成長>
void GameObject_Bullet_Grow(GameObject* obj, float grow)
{
	GameObject_Bullet_SetSize(obj, obj->sprite.scale + grow);
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
