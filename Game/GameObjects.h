#pragma once
#include "GameObject.h"
#include "Vector.h"

// �萔�̒�` ==============================================================

// <�v���C���[>
#define PLAYER_VEL 4.f		// �v���C���[�̑��x
// <�G>
#define ENEMY_VEL 2			// �G�̑��x
// <�e>
#define BULLET_VEL 3.f		// �G�̑��x

#define BULLET_GROW_SPEED .1f

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

// <<�G�I�u�W�F�N�g>> ----------------------------------------------

// <�G�I�u�W�F�N�g�쐬>
GameObject GameObject_Enemy_Create(int texture);

// <�G�I�u�W�F�N�g���W�f�t�H���g>
void GameObject_Enemy_SetPosDefault(GameObject* obj, const GameObject* field);

// <�G�I�u�W�F�N�g�ړ��f�t�H���g>
void GameObject_Enemy_SetVelDefault(GameObject* obj);

// <�G�I�u�W�F�N�g�X�V>
void GameObject_Enemy_Update(GameObject* obj);

// <<�v���C���[�I�u�W�F�N�g>> ----------------------------------------------

// <�v���C���[�I�u�W�F�N�g�쐬>
GameObject GameObject_Player_Create(void);

// <�v���C���[�I�u�W�F�N�g���WY�f�t�H���g>
void GameObject_Player_SetPosYDefault(GameObject* obj, const GameObject* field);

// <<�e�I�u�W�F�N�g>> ----------------------------------------------

// <�e�I�u�W�F�N�g�쐬>
GameObject GameObject_Bullet_Create(void);

// <�e�I�u�W�F�N�g���W�f�t�H���g>
void GameObject_Bullet_SetPosDefault(GameObject* obj, const GameObject* player);

// <�e�I�u�W�F�N�g�ړ��f�t�H���g>
void GameObject_Bullet_SetVelDefault(GameObject* obj, float offset_angle, int num = 0, int total = 1);

// <�e�I�u�W�F�N�g�T�C�Y�ύX>
void GameObject_Bullet_SetSize(GameObject* obj, float scale);

// <�e�I�u�W�F�N�g����>
void GameObject_Bullet_Grow(GameObject* obj, float grow = BULLET_GROW_SPEED);

// <�I�u�W�F�N�g���[�v�`��>
void GameObject_RenderLoop(const GameObject* obj, const GameObject* tile_area, GameObject* tile_obj, const Vec2* translate);
