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
	TYPE_GOAL,
	TYPE_GOAL_DOOM,
	TYPE_BEAM,
	TYPE_WARP,
	TYPE_VEL
} ObjectType;

// 関数の宣言 ==============================================================

// <弾オブジェクト>
GameObject GameObject_Ball_Create(const Vec2* mouse, const Vec2* vec);

// <ゴールオブジェクト>
GameObject GameObject_Goal_Create(const Vec2* mouse);

// <スタートオブジェクト>
GameObject GameObject_Start_Create(const Vec2* mouse, const Vec2* vec);

// <惑星オブジェクト>
GameObject GameObject_Planet_Create(const Vec2* mouse);

// <ビームオブジェクト>
GameObject GameObject_Beam_Create(const Vec2* mouse, const Vec2* next);

// <オブジェクト>
GameObject GameObject_Type_Create(int type, const Vec2* mouse, const Vec2* vec);

// <オブジェクトループ描画>
void GameObject_RenderLoop(const GameObject* obj, const GameObject* tile_area, GameObject* tile_obj, const Vec2* translate);

// <フィールド描画>
void GameObject_Field_Render(const GameObject* field, const Vec2* translate, int fadeout);

// <メッセージ描画>
void GameObject_Msg_Render(const Vec2* pos, const Vec2* translate, const char* msg);
