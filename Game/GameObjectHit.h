#pragma once
#include "GameObject.h"

// 関数の宣言 ==============================================================

// <オブジェクト点当たり判定>
BOOL GameObject_IsHitPoint(const GameObject* obj, const Vec2* p);

// <オブジェクト当たり判定>
BOOL GameObject_IsHit(const GameObject* obj1, const GameObject* obj2);
