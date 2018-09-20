#include "GameObjectHit.h"
#include "GameObject.h"
#include "GameUtils.h"
#include <math.h>

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
	Vec2 v2 = Vec2_Sub(&b2, &b1);

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
