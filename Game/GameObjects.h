#pragma once
#include "GameObject.h"
#include "Vector.h"

// 定数の定義 ==============================================================

// <プレイヤー>
#define PLAYER_VEL 4.f		// プレイヤーの速度
// <敵>
#define ENEMY_VEL 2			// 敵の速度
// <弾>
#define BULLET_VEL 3.f		// 敵の速度

#define BULLET_GROW_SPEED .1f

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

// <<敵オブジェクト>> ----------------------------------------------

// <敵オブジェクト作成>
GameObject GameObject_Enemy_Create(int texture);

// <敵オブジェクト座標デフォルト>
void GameObject_Enemy_SetPosDefault(GameObject* obj, const GameObject* field);

// <敵オブジェクト移動デフォルト>
void GameObject_Enemy_SetVelDefault(GameObject* obj);

// <敵オブジェクト更新>
void GameObject_Enemy_Update(GameObject* obj);

// <<プレイヤーオブジェクト>> ----------------------------------------------

// <プレイヤーオブジェクト作成>
GameObject GameObject_Player_Create(void);

// <プレイヤーオブジェクト座標Yデフォルト>
void GameObject_Player_SetPosYDefault(GameObject* obj, const GameObject* field);

// <<弾オブジェクト>> ----------------------------------------------

// <弾オブジェクト作成>
GameObject GameObject_Bullet_Create(void);

// <弾オブジェクト座標デフォルト>
void GameObject_Bullet_SetPosDefault(GameObject* obj, const GameObject* player);

// <弾オブジェクト移動デフォルト>
void GameObject_Bullet_SetVelDefault(GameObject* obj, float offset_angle, int num = 0, int total = 1);

// <弾オブジェクトサイズ変更>
void GameObject_Bullet_SetSize(GameObject* obj, float scale);

// <弾オブジェクト成長>
void GameObject_Bullet_Grow(GameObject* obj, float grow = BULLET_GROW_SPEED);

// <オブジェクトループ描画>
void GameObject_RenderLoop(const GameObject* obj, const GameObject* tile_area, GameObject* tile_obj, const Vec2* translate);
