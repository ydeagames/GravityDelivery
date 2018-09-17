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
void GameObject_SetSize(GameObject* obj, float scale, float size = 10);

// <ゴールオブジェクト>
GameObject GameObject_Goal_Create(const Vec2* mouse);

// <スタートオブジェクト>
GameObject GameObject_Start_Create(const Vec2* mouse, const Vec2* vec);

// <惑星オブジェクト>
GameObject GameObject_Planet_Create(const Vec2* mouse);

// <オブジェクト>
GameObject GameObject_Type_Create(int type, const Vec2* mouse, const Vec2* vec);

// <オブジェクトループ描画>
void GameObject_RenderLoop(const GameObject* obj, const GameObject* tile_area, GameObject* tile_obj, const Vec2* translate);

// <フィールド描画>
void GameObject_Field_Render(const GameObject* field, const Vec2* translate);
