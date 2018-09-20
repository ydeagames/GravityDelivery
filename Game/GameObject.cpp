#include "GameObject.h"
#include "GameUtils.h"
#include "GameMain.h"
#include "Vector.h"
#include <math.h>

// グローバル変数の定義 ====================================================

BOOL g_debug_mode = FALSE;

// <<ティック>> --------------------------------------------------------

// <最終時刻>
static int g_lastcount = -1;
// <デルタミリ秒>
static int g_deltamilliseconds = 0;

// 関数の宣言 ==============================================================

static BOOL GameObject_IsHitBoxPoint(const GameObject* obj, const Vec2* p);
static BOOL GameObject_IsHitCirclePoint(const GameObject* circle, const Vec2* p);
static BOOL GameObject_IsHitLinePoint(const GameObject* line, const Vec2* p);
static BOOL GameObject_IsHitBox(const GameObject* obj1, const GameObject* obj2);
static BOOL GameObject_IsHitBoxCircle(const GameObject* box, const GameObject* circle);
static BOOL GameObject_IsHitCircle(const GameObject* obj1, const GameObject* obj2);
static BOOL GameObject_IsHitLineCircle(const GameObject* line, const GameObject* circle);
static BOOL GameObject_IsHitLineBox(const GameObject* line, const GameObject* box);
static BOOL GameObject_IsHitLine(const GameObject* line1, const GameObject* line2);

// 関数の定義 ==============================================================

// <<テクスチャ>> ------------------------------------------------------

// <テクスチャ作成>
GameTexture GameTexture_Create(HGRP texture, Vec2 anchor, Vec2 size)
{
	return{ texture, anchor, size, Vec2_Create(size.x / 2, size.y / 2) };
}

// <テクスチャなし>
GameTexture GameTexture_CreateNone()
{
	return GameTexture_Create(TEXTURE_NONE, Vec2_Create(), Vec2_Create());
}

// <<スプライトアニメーション>> ----------------------------------------

// <スプライトアニメーション作成>
GameSpriteAnimation GameSpriteAnimation_Create(int frames_start, int frames_end, int frame_duration)
{
	return{ frames_start, frames_end, frame_duration, 0, TRUE, ANIMATION_RUNNING };
}

// <スプライトアニメーションなし>
GameSpriteAnimation GameSpriteAnimation_CreateNone()
{
	return GameSpriteAnimation_Create(0, 0, 1);
}

// <スプライトアニメーション更新>
AnimationState GameSpriteAnimation_Update(GameSprite* animate_sprite)
{
	// アニメーションしているか
	animate_sprite->animation.result = ANIMATION_RUNNING;
	// 経過時間
	animate_sprite->animation.elapsed_time++;
	// 最初のフレーム以上
	animate_sprite->frame_index = GetMax(animate_sprite->frame_index, animate_sprite->animation.frame_start);

	// フレーム経過
	if (animate_sprite->animation.elapsed_time > animate_sprite->animation.frame_duration)
	{
		// 経過時間
		animate_sprite->animation.elapsed_time = 0;
		// フレーム番号
		animate_sprite->frame_index++;

		// 最初に戻る
		if (animate_sprite->frame_index > animate_sprite->animation.frame_end)
		{
			// ループするなら
			if (animate_sprite->animation.loop_flag)
				// 最初に戻る
				animate_sprite->frame_index = animate_sprite->animation.frame_start;
			// アニメーション完了
			animate_sprite->animation.result = ANIMATION_FINISHED;
		}
	}

	return animate_sprite->animation.result;
}

// <<スプライト>> ------------------------------------------------------

// <スプライト作成>
GameSprite GameSprite_Create(GameTexture texture, float scale, float angle)
{
	return{ COLOR_WHITE, texture, texture.size, 1, 0, Vec2_Create(), scale, angle, GameSpriteAnimation_CreateNone() };
}

// <スプライトなし>
GameSprite GameSprite_CreateNone()
{
	return GameSprite_Create(GameTexture_CreateNone(), 0, 0);
}

// <スプライト更新>
void GameSprite_SetFrame(GameSprite* sprite, int frame)
{
	// フレーム番号
	sprite->frame_index = frame;
}

// <スプライト描画>
void GameSprite_Render(const GameSprite* sprite, const Vec2* pos)
{
	int column = sprite->frame_index%sprite->num_columns;
	int row = sprite->frame_index / sprite->num_columns;

	Vec2 anchor = Vec2_Add(&sprite->texture.anchor, &Vec2_Create(sprite->size.x * column, sprite->size.y * row));

	// スプライト描画
	DrawRectRotaGraph2F(
		pos->x + sprite->offset.x, pos->y + sprite->offset.y,
		(int)anchor.x, (int)anchor.y,
		(int)sprite->texture.size.x, (int)sprite->texture.size.y,
		sprite->texture.center.x, sprite->texture.center.y,
		(double)sprite->scale,
		(double)sprite->angle,
		sprite->texture.texture,
		TRUE
	);
}

// <<ティック>> --------------------------------------------------------

// <オブジェクト作成>
void GameTick_Update(void)
{
	int now = GetNowCount();
	g_deltamilliseconds = GetMin(100, now - g_lastcount);
	g_lastcount = now;
}

// <<オブジェクト>> ----------------------------------------------------

// <オブジェクト作成>
GameObject GameObject_Create(Vec2 pos, Vec2 vel, Vec2 size)
{
	return{ pos, vel, size, SHAPE_BOX, GameSprite_CreateNone(), FALSE, 0, CONNECTION_NONE, TRUE, 1, 0, GameTimer_Create() };
}

// <オブジェクト削除>
void GameObject_Dispose(GameObject* obj)
{
	obj->alive = FALSE;
}

// <オブジェクト確認>
BOOL GameObject_IsAlive(const GameObject* obj)
{
	return obj->alive;
}

// <オブジェクト座標更新>
void GameObject_UpdatePosition(GameObject* obj)
{
	obj->pos.x += obj->vel.x;// *(g_deltamilliseconds / 17.f);
	obj->pos.y += obj->vel.y;// *(g_deltamilliseconds / 17.f);
}

// <オブジェクトXオフセット>
float GameObject_OffsetX(const GameObject* obj, ObjectSide side, float pos, float padding)
{
	float offset = 0;
	switch (side)
	{
	case LEFT:
		offset = -(padding + obj->size.x / 2.f);
		break;
	case RIGHT:
		offset = (padding + obj->size.x / 2.f);
		break;
	}
	return pos + offset;
}

// <オブジェクトXオフセット>
float GameObject_OffsetY(const GameObject* obj, ObjectSide side, float pos, float padding)
{
	float offset = 0;
	switch (side)
	{
	case TOP:
		offset = -(padding + obj->size.y / 2.f);
		break;
	case BOTTOM:
		offset = (padding + obj->size.y / 2.f);
		break;
	}
	return pos + offset;
}

// <オブジェクトX位置ゲット>
float GameObject_GetX(const GameObject* obj, ObjectSide side, float padding)
{
	return GameObject_OffsetX(obj, side, obj->pos.x, padding);
}

// <オブジェクトY位置ゲット>
float GameObject_GetY(const GameObject* obj, ObjectSide side, float padding)
{
	return GameObject_OffsetY(obj, side, obj->pos.y, padding);
}

// <矩形オブジェクト×点当たり判定>
static BOOL GameObject_IsHitBoxPoint(const GameObject* obj, const Vec2* p)
{
	return (
		p->x < GameObject_GetX(obj, RIGHT) &&
		GameObject_GetX(obj, LEFT) < p->x &&
		p->y < GameObject_GetY(obj, BOTTOM) &&
		GameObject_GetY(obj, TOP) < p->y
		);
}

// <円オブジェクト×点当たり判定>
static BOOL GameObject_IsHitCirclePoint(const GameObject* circle, const Vec2* p)
{
	float r1 = GetMinF(circle->size.x, circle->size.y) / 2;

	return Vec2_LengthSquaredTo(&circle->pos, p) < r1*r1;
}

// <線オブジェクト×点当たり判定>
static BOOL GameObject_IsHitLinePoint(const GameObject* line, const Vec2* p)
{
	GameObject circle = GameObject_Create(*p);
	return GameObject_IsHitLineCircle(line, &circle);
}

// <オブジェクト×点当たり判定>
BOOL GameObject_IsHitPoint(const GameObject* obj, const Vec2* p)
{
	if (obj->shape == SHAPE_BOX)
		return GameObject_IsHitBoxPoint(obj, p);
	else if (obj->shape == SHAPE_CIRCLE)
		return GameObject_IsHitCirclePoint(obj, p);
	else if (obj->shape == SHAPE_LINE)
		return GameObject_IsHitLinePoint(obj, p);
	return FALSE;
}

// <矩形オブジェクト×矩形オブジェクト当たり判定>
static BOOL GameObject_IsHitBox(const GameObject* obj1, const GameObject* obj2)
{
	return (
		GameObject_GetX(obj2, LEFT) < GameObject_GetX(obj1, RIGHT) &&
		GameObject_GetX(obj1, LEFT) < GameObject_GetX(obj2, RIGHT) &&
		GameObject_GetY(obj2, TOP) < GameObject_GetY(obj1, BOTTOM) &&
		GameObject_GetY(obj1, TOP) < GameObject_GetY(obj2, BOTTOM)
		);
}

// <矩形オブジェクト×円当たり判定>
static BOOL GameObject_IsHitBoxCircle(const GameObject* box, const GameObject* circle)
{
	if (GameObject_GetX(box, LEFT) <= circle->pos.x && circle->pos.x <= GameObject_GetX(box, RIGHT))
		return box->size.y / 2 + circle->size.y / 2 > GetAbsF(circle->pos.y - box->pos.y);
	else if (GameObject_GetY(box, TOP) <= circle->pos.y && circle->pos.y <= GameObject_GetY(box, BOTTOM))
		return box->size.x / 2 + circle->size.x / 2 > GetAbsF(circle->pos.x - box->pos.x);
	else
	{
		float diagonal = Vec2_Length(&box->size);
		GameObject boxcircle = GameObject_Create(box->pos, Vec2_Create(), Vec2_Create(diagonal, diagonal));
		return GameObject_IsHitCircle(circle, &boxcircle);
	}
}

// <円オブジェクト×円オブジェクト当たり判定>
static BOOL GameObject_IsHitCircle(const GameObject* obj1, const GameObject* obj2)
{
	float r1 = GetMinF(obj1->size.x, obj1->size.y) / 2;
	float r2 = GetMinF(obj2->size.x, obj2->size.y) / 2;

	return Vec2_LengthSquaredTo(&obj1->pos, &obj2->pos) < (r1 + r2)*(r1 + r2);
}

// <線オブジェクト×円オブジェクト当たり判定>
static BOOL GameObject_IsHitLineCircle(const GameObject* line, const GameObject* circle)
{
	Vec2 pointA = Vec2_Create(GameObject_GetX(line, LEFT), GameObject_GetY(line, TOP));
	Vec2 pointB = Vec2_Create(GameObject_GetX(line, RIGHT), GameObject_GetY(line, BOTTOM));

	//ベクトルを生成
	Vec2 vecAB = Vec2_Sub(&pointB, &pointA);
	Vec2 vecAP = Vec2_Sub(&circle->pos, &pointA);
	Vec2 vecBP = Vec2_Sub(&circle->pos, &pointB);

	//ABの単位ベクトルを計算
	Vec2 normalAB = Vec2_Normalized(&vecAB);

	//AからXまでの距離を
	//単位ベクトルABとベクトルAPの内積で求める
	float lenAX = Vec2_Dot(&normalAB, &vecAP);

	float shortestDistance;  //線分APとPの最短距離
	if (lenAX < 0) {
		//AXが負なら APが円の中心までの最短距離
		shortestDistance = Vec2_Length(&vecAP);
	}
	else if (lenAX > Vec2_Length(&vecAB)) {
		//AXがAPよりも長い場合は、BPが円の中心
		//までの最短距離
		shortestDistance = Vec2_Length(&vecBP);
	}
	else {
		//PがAB上にあるので、PXが最短距離
		//単位ベクトルABとベクトルAPの外積で求める
		shortestDistance = GetAbsF(Vec2_Cross(&normalAB, &vecAP));
	}

	{
		//Xの座標を求める(AXの長さより計算）
		Vec2 pointX = Vec2_Create(pointA.x + (normalAB.x * lenAX),
			pointA.y + (normalAB.y * lenAX));

		float r1 = GetMinF(circle->size.x, circle->size.y) / 2;
		float r2 = line->edge / 2;

		BOOL hit = FALSE;
		if (shortestDistance < r1 + r2) {
			//最短距離が円の半径よりも小さい場合は、当たり
			hit = TRUE;
		}
		return hit;
	}
}

// 点と線分の関係
// @return	1	点p2は線分p0-p1の反時計方向
//			-1	点p2は線分p0-p1の時計方向
//			2	点p2は線分p0-p1の手前
//			-2	点p2は線分p0-p1の先
//			0	点p2は線分p0-p1の上
static int ccw(const Vec2* p0, const Vec2* p1, const Vec2* p2)
{
	int sw = 0;
	float EPS = 1.0e-8f;

	Vec2 a = Vec2_Create(p1->x - p0->x, p1->y - p0->y);
	Vec2 b = Vec2_Create(p2->x - p0->x, p2->y - p0->y);

	float cross = Vec2_Cross(&a, &b);
	if (cross > EPS)
		sw = 1;
	else if (cross < -EPS)
		sw = -1;
	else if (Vec2_Dot(&a, &b) < -EPS)
		sw = 2;
	else if (Vec2_LengthSquared(&a) < Vec2_LengthSquared(&b))
		sw = -2;
	return sw;
}

// <線オブジェクト×矩形オブジェクト当たり判定>
static BOOL GameObject_IsHitLineBox(const GameObject* line, const GameObject* box)
{
	Vec2 p1 = Vec2_Create(GameObject_GetX(box, LEFT), GameObject_GetY(box, TOP));
	Vec2 p2 = Vec2_Create(GameObject_GetX(box, LEFT), GameObject_GetY(box, BOTTOM));
	Vec2 p3 = Vec2_Create(GameObject_GetX(box, RIGHT), GameObject_GetY(box, BOTTOM));
	Vec2 p4 = Vec2_Create(GameObject_GetX(box, RIGHT), GameObject_GetY(box, TOP));
	Vec2 p5 = Vec2_Create(GameObject_GetX(line, LEFT), GameObject_GetY(line, TOP));
	Vec2 p6 = Vec2_Create(GameObject_GetX(line, RIGHT), GameObject_GetY(line, BOTTOM));

	int l2p0 = ccw(&p5, &p6, &p1);
	int l2p1 = ccw(&p5, &p6, &p2);
	int l2p2 = ccw(&p5, &p6, &p3);
	int l2p3 = ccw(&p5, &p6, &p4);

	if (ccw(&p1, &p2, &p5) * ccw(&p1, &p2, &p6) <= 0 && l2p0 * l2p1 <= 0 ||
		ccw(&p2, &p3, &p5) * ccw(&p2, &p3, &p6) <= 0 && l2p1 * l2p2 <= 0 ||
		ccw(&p3, &p4, &p5) * ccw(&p3, &p4, &p6) <= 0 && l2p2 * l2p3 <= 0 ||
		ccw(&p4, &p1, &p5) * ccw(&p4, &p1, &p6) <= 0 && l2p3 * l2p0 <= 0)
		TRUE;

	return FALSE;
}

// <線分オブジェクト当たり判定>
static BOOL GameObject_IsHitLine(const GameObject* line1, const GameObject* line2)
{
	float EPS = 1.0e-8f;

	Vec2 a1 = Vec2_Create(GameObject_GetX(line1, LEFT), GameObject_GetY(line1, TOP));
	Vec2 a2 = Vec2_Create(GameObject_GetX(line1, RIGHT), GameObject_GetY(line1, BOTTOM));
	Vec2 b1 = Vec2_Create(GameObject_GetX(line2, LEFT), GameObject_GetY(line2, TOP));
	Vec2 b2 = Vec2_Create(GameObject_GetX(line2, RIGHT), GameObject_GetY(line2, BOTTOM));

	Vec2 v = Vec2_Sub(&a1, &b1);
	Vec2 v1 = Vec2_Sub(&a2, &a1);
	Vec2 v2= Vec2_Sub(&b2, &b1);

	float Crs_v1_v2 = Vec2_Cross(&v1, &v2);

	if (Crs_v1_v2 != 0.0f) {
		float Crs_v_v1 = Vec2_Cross(&v, &v1);
		float Crs_v_v2 = Vec2_Cross(&v, &v2);

		float t1 = Crs_v_v2 / Crs_v1_v2;
		float t2 = Crs_v_v1 / Crs_v1_v2;

		return !(t1 + EPS < 0 || t1 - EPS > 1 || t2 + EPS < 0 || t2 - EPS > 1);
	}

	return TRUE;
}

// <オブジェクト当たり判定>
BOOL GameObject_IsHit(const GameObject* obj1, const GameObject* obj2)
{
	if (obj1->shape == SHAPE_BOX && obj2->shape == SHAPE_BOX)
		return GameObject_IsHitBox(obj1, obj2);
	else if (obj1->shape == SHAPE_CIRCLE && obj2->shape == SHAPE_CIRCLE)
		return GameObject_IsHitCircle(obj1, obj2);
	else if (obj1->shape == SHAPE_LINE && obj2->shape == SHAPE_LINE)
		return GameObject_IsHitBox(obj1, obj2) && GameObject_IsHitLine(obj1, obj2);
	else
	{
		if (GameObject_IsHitBox(obj1, obj2))
		{
			switch (obj1->shape)
			{
			case SHAPE_BOX:
				switch (obj2->shape)
				{
				case SHAPE_CIRCLE:
					return GameObject_IsHitBoxCircle(obj1, obj2);
				case SHAPE_LINE:
					return GameObject_IsHitLineBox(obj2, obj1);
				}
				break;
			case SHAPE_CIRCLE:
				switch (obj2->shape)
				{
				case SHAPE_BOX:
					return GameObject_IsHitBoxCircle(obj2, obj1);
				case SHAPE_LINE:
					return GameObject_IsHitLineCircle(obj2, obj1);
				}
				break;
			case SHAPE_LINE:
				switch (obj2->shape)
				{
				case SHAPE_BOX:
					return GameObject_IsHitLineBox(obj1, obj2);
				case SHAPE_CIRCLE:
					return GameObject_IsHitLineCircle(obj1, obj2);
				}
				break;
			}
		}

		return FALSE;
	}
}

// <オブジェクト描画>
void GameObject_Render(const GameObject* obj, const Vec2* translate)
{
	float box_xl = GameObject_GetX(obj, LEFT) + translate->x;
	float box_xc = GameObject_GetX(obj, CENTER_X) + translate->x;
	float box_xr = GameObject_GetX(obj, RIGHT) + translate->x;
	float box_yt = GameObject_GetY(obj, TOP) + translate->y;
	float box_ym = GameObject_GetY(obj, CENTER_Y) + translate->y;
	float box_yb = GameObject_GetY(obj, BOTTOM) + translate->y;
	Vec2 box_t = Vec2_Create(box_xc, box_ym);

	// テクスチャを確認
	if (obj->sprite.texture.texture != TEXTURE_NONE)
	{
		if (obj->sprite.texture.texture != TEXTURE_MISSING)
		{
			switch (obj->sprite_connection)
			{
			case CONNECTION_LOOP:
			case CONNECTION_BARRIER:
			{
				// リピートタイル (回転、テクスチャ中心座標 には未対応)
				Vec2 center_offset = Vec2_Scale(&obj->sprite.texture.center, obj->sprite.scale);
				Vec2 sp_pos = Vec2_Add(&box_t, &obj->sprite.offset);
				Vec2 sp_size = Vec2_Scale(&obj->sprite.texture.size, obj->sprite.scale);

				float go_left = box_xl;
				float go_right = box_xr;
				float go_top = box_yt;
				float go_bottom = box_yb;

				float sp_left = sp_pos.x - sp_size.x / 2;
				float sp_right = sp_pos.x + sp_size.x / 2;
				float sp_top = sp_pos.y - sp_size.y / 2;
				float sp_bottom = sp_pos.y + sp_size.y / 2;

				switch (obj->sprite_connection)
				{
				case CONNECTION_BARRIER:
					if (sp_left < go_right && go_left < sp_right && sp_top < go_bottom && go_top < sp_bottom)
						GameSprite_Render(&obj->sprite, &box_t);
					break;
				case CONNECTION_LOOP:
					float offset_x = GetLoopRangeF(go_left, sp_left, sp_right) - sp_left;
					float offset_y = GetLoopRangeF(go_top, sp_top, sp_bottom) - sp_top;

					if (sp_size.x >= 1.f && sp_size.y >= 1.f)
					{
						for (float iy = go_top + sp_size.y / 2 - offset_y - center_offset.y; iy < go_bottom; iy += sp_size.y)
						{
							for (float ix = go_left + sp_size.x / 2 - offset_x - center_offset.x; ix < go_right; ix += sp_size.x)
							{
								GameSprite_Render(&obj->sprite, &Vec2_Create(ix + sp_size.x / 2 - obj->sprite.offset.x, iy + sp_size.y / 2 - obj->sprite.offset.y));

								if (DEBUG_HITBOX)
									DrawBoxAA(ix, iy, ix + sp_size.x, iy + sp_size.y, obj->sprite.color, FALSE, .5f);
							}
						}
					}

					break;
				}

				break;
			}
			default:
				GameSprite_Render(&obj->sprite, &box_t);
				break;
			}
		}
		else
		{
			// NULLテクスチャを表示
			DrawBoxAA(box_xl, box_yt, box_xr, box_yb, COLOR_BLACK, TRUE);
			DrawBoxAA(box_xl, box_yt, box_xc, box_ym, COLOR_FUCHSIA, TRUE);
			DrawBoxAA(box_xc, box_ym, box_xr, box_yb, COLOR_FUCHSIA, TRUE);
			//DrawBoxAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, FALSE, .5f);
		}
	}

	// 図形描画
	switch (obj->shape)
	{
	default:
	case SHAPE_BOX:
		// 矩形描画
		if (obj->fill)
			DrawBoxAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, TRUE);
		if (obj->edge > 0)
			DrawBoxAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, FALSE, obj->edge);
		if (DEBUG_HITBOX)
		{
			DrawBoxAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, FALSE, .5f);
			DrawLineAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, .5f);
			DrawLineAA(box_xr, box_yt, box_xl, box_yb, obj->sprite.color, .5f);
		}
		break;
	case SHAPE_CIRCLE:
	{
		float r1 = GetMinF(obj->size.x, obj->size.y) / 2;
		// 円
		if (obj->fill)
			DrawCircleAA(box_xc, box_ym, r1, 120, obj->sprite.color, TRUE);
		if (obj->edge > 0)
			DrawCircleAA(box_xc, box_ym, r1, 120, obj->sprite.color, FALSE, obj->edge);
		if (DEBUG_HITBOX)
		{
			DrawCircleAA(box_xc, box_ym, r1, 120, obj->sprite.color, FALSE, .5f);
			DrawBoxAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, FALSE, .5f);
			DrawLineAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, .5f);
			DrawLineAA(box_xr, box_yt, box_xl, box_yb, obj->sprite.color, .5f);
		}
		break;
	}
	case SHAPE_LINE:
	{
		float r1 = obj->edge;
		// 線
		if (DEBUG_HITBOX)
			Vec2_Render(&obj->size, &obj->pos, obj->sprite.color, .5f);
		DrawLineAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, r1);
		break;
	}
	}
}

// <オブジェクトサイズ変更>
void GameObject_SetSize(GameObject* obj, float scale, float size)
{
	obj->sprite.scale = scale;
	obj->size = Vec2_Create(size * scale, size * scale);
}

// <<フィールドオブジェクト>> ------------------------------------------

// <フィールドオブジェクト作成>
GameObject GameObject_Field_Create(void)
{
	return GameObject_Create(Vec2_Create(SCREEN_CENTER_X, SCREEN_CENTER_Y), Vec2_Create(), Vec2_Create(SCREEN_WIDTH, SCREEN_HEIGHT));
}

// <フィールド上下衝突処理>
ObjectSide GameObject_Field_CollisionVertical(const GameObject* field, GameObject* obj, ObjectConnection connection, ObjectEdgeSide edge)
{
	// ヒットサイド
	ObjectSide side_hit = NONE;

	// コウラ・上下壁当たり判定
	{
		// 縁に応じてパディングを調整
		float padding_top = GameObject_GetY(field, TOP);
		float padding_bottom = GameObject_GetY(field, BOTTOM);
		switch (edge)
		{
		case EDGESIDE_INNER:
			padding_top = GameObject_OffsetY(obj, BOTTOM, padding_top);
			padding_bottom = GameObject_OffsetY(obj, TOP, padding_bottom);
			break;
		case EDGESIDE_OUTER:
			padding_top = GameObject_OffsetY(obj, TOP, padding_top);
			padding_bottom = GameObject_OffsetY(obj, BOTTOM, padding_bottom);
			break;
		}

		// 当たり判定
		if (obj->pos.y < padding_top)
			side_hit = TOP;
		else if (padding_bottom <= obj->pos.y)
			side_hit = BOTTOM;

		// フィールドのつながり
		switch (connection)
		{
		case CONNECTION_BARRIER:
			// 壁にあたったら調整
			obj->pos.y = ClampF(obj->pos.y, padding_top, padding_bottom);
			break;
		case CONNECTION_LOOP:
			// 壁にあたったらループ
			obj->pos.y = GetLoopRangeF(obj->pos.y, padding_top, padding_bottom);
			break;
		}
	}

	return side_hit;
}

// <フィールド左右衝突処理>
ObjectSide GameObject_Field_CollisionHorizontal(const GameObject* field, GameObject* obj, ObjectConnection connection, ObjectEdgeSide edge)
{
	// ヒットサイド
	ObjectSide side_hit = NONE;

	// コウラ・左右壁当たり判定
	{
		// 縁に応じてパディングを調整
		float padding_left = GameObject_GetX(field, LEFT);
		float padding_right = GameObject_GetX(field, RIGHT);
		switch (edge)
		{
		case EDGESIDE_INNER:
			padding_left = GameObject_OffsetX(obj, RIGHT, padding_left);
			padding_right = GameObject_OffsetX(obj, LEFT, padding_right);
			break;
		case EDGESIDE_OUTER:
			padding_left = GameObject_OffsetX(obj, LEFT, padding_left);
			padding_right = GameObject_OffsetX(obj, RIGHT, padding_right);
			break;
		}

		// 当たり判定
		if (obj->pos.x < padding_left)
			side_hit = LEFT;
		else if (padding_right <= obj->pos.x)
			side_hit = RIGHT;

		// フィールドのつながり
		switch (connection)
		{
		case CONNECTION_BARRIER:
			// 壁にあたったら調整
			obj->pos.x = ClampF(obj->pos.x, padding_left, padding_right);
			break;
		case CONNECTION_LOOP:
			// 壁にあたったらループ
			obj->pos.x = GetLoopRangeF(obj->pos.x, padding_left, padding_right);
			break;
		}
	}

	return side_hit;
}
