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
	TYPE_GOAL,
	TYPE_GOAL_DOOM,
	TYPE_BEAM,
	TYPE_WARP,
	TYPE_VEL
} ObjectType;

// �֐��̐錾 ==============================================================

// <�e�I�u�W�F�N�g>
GameObject GameObject_Ball_Create(const Vec2* mouse, const Vec2* vec);

// <�S�[���I�u�W�F�N�g>
GameObject GameObject_Goal_Create(const Vec2* mouse);

// <�X�^�[�g�I�u�W�F�N�g>
GameObject GameObject_Start_Create(const Vec2* mouse, const Vec2* vec);

// <�f���I�u�W�F�N�g>
GameObject GameObject_Planet_Create(const Vec2* mouse);

// <�r�[���I�u�W�F�N�g>
GameObject GameObject_Beam_Create(const Vec2* mouse, const Vec2* next);

// <�I�u�W�F�N�g>
GameObject GameObject_Type_Create(int type, const Vec2* mouse, const Vec2* vec);

// <�I�u�W�F�N�g���[�v�`��>
void GameObject_RenderLoop(const GameObject* obj, const GameObject* tile_area, GameObject* tile_obj, const Vec2* translate);

// <�t�B�[���h�`��>
void GameObject_Field_Render(const GameObject* field, const Vec2* translate, int fadeout);

// <���b�Z�[�W�`��>
void GameObject_Msg_Render(const Vec2* pos, const Vec2* translate, const char* msg);
