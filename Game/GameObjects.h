#pragma once
#include "GameObject.h"
#include "Vector.h"

// 定数の定義 ==============================================================

// 列挙型の定義 ============================================================

// <種類> --------------------------------------------------------------

typedef enum
{
	TYPE_UNKNOWN = 0,
	TYPE_PLANET,
	TYPE_START,
	TYPE_GOAL
} ObjectType;

// 関数の宣言 ==============================================================

// <弾オブジェクトサイズ変更>
void GameObject_SetSize(GameObject* obj, float scale);

// <オブジェクトループ描画>
void GameObject_RenderLoop(const GameObject* obj, const GameObject* tile_area, GameObject* tile_obj, const Vec2* translate);
