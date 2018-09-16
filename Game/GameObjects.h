#pragma once
#include "GameObject.h"
#include "Vector.h"

// �萔�̒�` ==============================================================

// �񋓌^�̒�` ============================================================

// <���> --------------------------------------------------------------

typedef enum
{
	TYPE_UNKNOWN = 0,
	TYPE_PLANET,
	TYPE_START,
	TYPE_GOAL
} ObjectType;

// �֐��̐錾 ==============================================================

// <�e�I�u�W�F�N�g�T�C�Y�ύX>
void GameObject_SetSize(GameObject* obj, float scale);

// <�S�[���I�u�W�F�N�g>
GameObject GameObject_Goal_Create(const Vec2* mouse);

// <�X�^�[�g�I�u�W�F�N�g>
GameObject GameObject_Start_Create(const Vec2* mouse, const Vec2* vec);

// <�f���I�u�W�F�N�g>
GameObject GameObject_Planet_Create(const Vec2* mouse);

// <�I�u�W�F�N�g���[�v�`��>
void GameObject_RenderLoop(const GameObject* obj, const GameObject* tile_area, GameObject* tile_obj, const Vec2* translate);
