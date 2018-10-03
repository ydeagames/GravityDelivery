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
	TYPE_GOAL_DOOM_DEPRECATED,
	TYPE_BEAM,
	TYPE_WARP,
	TYPE_VEL,
	TYPE_BEAM_BOUNCE
} ObjectType;

typedef enum
{
	TYPE_PARTICLE_UNKNOWN = 0,
	TYPE_PARTICLE_BALL,
	TYPE_PARTICLE_DOOM,
	TYPE_PARTICLE_GOAL_DOOM,
	TYPE_PARTICLE_GOAL_DOOM_SPAWNER,
	TYPE_PARTICLE_GRAVITY,
	TYPE_PARTICLE_ACTIVE
} ParticleType;

// 関数の宣言 ==============================================================

// <弾オブジェクト>
GameObject GameObject_Ball_Create(const Vec2* pos, const Vec2* vec);

// <オブジェクト>
GameObject GameObject_Planets_Create(int type, const Vec2* base, const Vec2* next);

// <オブジェクト>
void GameObject_Planets_Serialize(const GameObject* obj, Vec2* base, Vec2* next);

// <パーティクル>
GameObject GameObject_Particles_Create(int type, const Vec2* base, const Vec2* vec);

// <オブジェクトループ描画>
void GameObject_RenderLoop(const GameObject* obj, const GameObject* tile_area, GameObject* tile_obj, const Vec2* translate);

// <フィールド描画>
void GameObject_Field_Render(const GameObject* field, const Vec2* translate, int fadeout);

// <メッセージ描画>
void GameObject_Msg_Render(const Vec2* pos, const Vec2* translate, const char* msg);
