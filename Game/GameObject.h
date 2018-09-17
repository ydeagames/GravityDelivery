#pragma once
#include "Vec2.h"
#include "GameTimer.h"

// �萔�̒�` ==============================================================

// <�f�o�b�O�p�����蔻��\��>
#define DEBUG_HITBOX (g_debug_mode)

// �O���[�o���ϐ��̐錾 ====================================================

extern BOOL g_debug_mode;

// �񋓌^�̒�` ============================================================

// <�ʒu�֌W> ----------------------------------------------------------
typedef enum
{
	NONE = 0,
	CENTER_X = 1,				// X����
	LEFT = -2,					// ��
	RIGHT = 2,					// �E
	CENTER_Y = -1,				// Y����
	TOP = -3,					// ��
	BOTTOM = 3					// ��
} ObjectSide;

// <���̈ʒu> ----------------------------------------------------------
typedef enum
{
	EDGESIDE_CENTER = 0,		// ���̏�
	EDGESIDE_OUTER = -1,		// ���̊O��
	EDGESIDE_INNER = 1			// ���̓���
} ObjectEdgeSide;

// <���[���h�̂Ȃ���> ------------------------------------------------
typedef enum
{
	CONNECTION_NONE = 0,		// �q����Ȃ��A�����Ȃ���ԂɈړ�
	CONNECTION_BARRIER,			// �ǂ�����A�i�߂Ȃ�
	CONNECTION_LOOP				// ���Α�����o�Ă���
} ObjectConnection;

// <�I�u�W�F�N�g�̌`> --------------------------------------------------
typedef enum
{
	SHAPE_BOX,					// �����`
	SHAPE_CIRCLE,				// �~
} ObjectShape;

// <�A�j���[�V�����̌���> ----------------------------------------------
typedef enum
{
	ANIMATION_RUNNING,			// �A�j���[�V���������s���̏��
	ANIMATION_FINISHED,			// �A�j���[�V�����Đ��������������
} AnimationState;

// �\���̂̐錾 ============================================================

// <�e�N�X�`��>
typedef struct
{
	HGRP texture;				// <�e�N�X�`��>
	Vec2 anchor;				// <�e�N�X�`����_>
	Vec2 size;					// <�e�N�X�`���T�C�Y>
	Vec2 center;				// <�e�N�X�`�����S>
} GameTexture;

// <�X�v���C�g�A�j���[�V����>
typedef struct
{
	int frame_start;			// �J�n�t���[���̃C���f�b�N�X
	int frame_end;				// �I���t���[���̃C���f�b�N�X
	int frame_duration;			// �t���[���Ԋu
	int elapsed_time;			// ���݂̃t���[���̌o�ߎ���
	BOOL loop_flag;				// �A�j���[�V�����̃��[�v
	AnimationState result;		// �Ō�̏��
} GameSpriteAnimation;

// <�X�v���C�g�I�u�W�F�N�g>
typedef struct
{
	unsigned int color;			// <�F>
	GameTexture texture;		// <�e�N�X�`��>
	Vec2 size;					// <�T�C�Y>
	int num_columns;			// <1�s������̃t���[����>
	int frame_index;			// <���݂̃t���[���̃C���f�b�N�X>
	Vec2 offset;				// <�I�t�Z�b�g>
	float scale;				// <�X�P�[��>
	float angle;				// <��]>
	GameSpriteAnimation animation;	// <�X�v���C�g�A�j���[�V����>
} GameSprite;

// <�Q�[���I�u�W�F�N�g>
typedef struct
{
	Vec2 pos;					// <�ʒu>
	Vec2 vel;					// <���x>
	Vec2 size;					// <�傫��>
	ObjectShape shape;			// <�`>
	GameSprite sprite;			// <�X�v���C�g>
	BOOL fill;					// <�h��Ԃ�>
	float edge;					// <��>
	ObjectConnection sprite_connection;	// <�X�v���C�g�̂Ȃ���>
	BOOL alive;					// <�\�����>
	int state;					// <���>
	int type;					// <�^�C�v>
	GameTimer count;			// <�J�E���^>
} GameObject;

// �萔�̒�` ==============================================================

// <�e�N�X�`��>
#define TEXTURE_MISSING -1		// �e�N�X�`����������܂���
#define TEXTURE_NONE -2			// �e�N�X�`���Ȃ�

// �֐��̐錾 ==============================================================

// <<�e�N�X�`��>> ------------------------------------------------------

// <�e�N�X�`���쐬>
GameTexture GameTexture_Create(HGRP texture, Vec2 anchor, Vec2 size);

// <�e�N�X�`���Ȃ�>
GameTexture GameTexture_CreateNone();

// <<�X�v���C�g�A�j���[�V����>> ----------------------------------------

// <�X�v���C�g�A�j���[�V�����쐬>
GameSpriteAnimation GameSpriteAnimation_Create(int frames_start, int frames_end, int frame_duration);

// <�X�v���C�g�A�j���[�V�����Ȃ�>
GameSpriteAnimation GameSpriteAnimation_CreateNone();

// <�X�v���C�g�A�j���[�V�����X�V>
AnimationState GameSpriteAnimation_Update(GameSprite* obj);

// <<�X�v���C�g>> ------------------------------------------------------

// <�X�v���C�g�쐬>
GameSprite GameSprite_Create(GameTexture texture, float scale = 1, float angle = 0);

// <�X�v���C�g�Ȃ�>
GameSprite GameSprite_CreateNone();

// <�X�v���C�g�X�V>
void GameSprite_SetFrame(GameSprite* sprite, int frame);

// <�X�v���C�g�`��>
void GameSprite_Render(const GameSprite* sprite, const Vec2* pos);

// <<�e�B�b�N>> --------------------------------------------------------

// <�I�u�W�F�N�g�쐬>
void GameTick_Update(void);

// <<�I�u�W�F�N�g>> ----------------------------------------------------

// <�I�u�W�F�N�g�쐬>
GameObject GameObject_Create(Vec2 pos = Vec2_Create(), Vec2 vel = Vec2_Create(), Vec2 size = Vec2_Create());

// <�I�u�W�F�N�g�폜>
void GameObject_Dispose(GameObject* obj);

// <�I�u�W�F�N�g�m�F>
BOOL GameObject_IsAlive(const GameObject* obj);

// <�I�u�W�F�N�g���W�X�V>
void GameObject_UpdatePosition(GameObject* obj);

// <�I�u�W�F�N�gX�I�t�Z�b�g>
float GameObject_OffsetX(const GameObject* obj, ObjectSide side, float pos = 0.f, float margin = 0.f);

// <�I�u�W�F�N�gX�I�t�Z�b�g>
float GameObject_OffsetY(const GameObject* obj, ObjectSide side, float pos = 0.f, float margin = 0.f);

// <�I�u�W�F�N�gX�ʒu�Q�b�g>
float GameObject_GetX(const GameObject* obj, ObjectSide side, float margin = 0.f);

// <�I�u�W�F�N�gY�ʒu�Q�b�g>
float GameObject_GetY(const GameObject* obj, ObjectSide side, float margin = 0.f);

// <�I�u�W�F�N�g�����蔻��>
BOOL GameObject_IsHit(const GameObject* obj1, const GameObject* obj2);

// <�I�u�W�F�N�g�_�����蔻��>
BOOL GameObject_IsHitPoint(const GameObject* obj, const Vec2* p);

// <�I�u�W�F�N�g�`��>
void GameObject_Render(const GameObject* obj, const Vec2* translate = &Vec2_Create());

// <<�t�B�[���h�I�u�W�F�N�g>> ------------------------------------------

// <�t�B�[���h�I�u�W�F�N�g�쐬>
GameObject GameObject_Field_Create(void);

// <�t�B�[���h�㉺�Փˏ���>
ObjectSide GameObject_Field_CollisionVertical(const GameObject* field, GameObject* obj, ObjectConnection connection, ObjectEdgeSide edge);

// <�t�B�[���h���E�Փˏ���>
ObjectSide GameObject_Field_CollisionHorizontal(const GameObject* field, GameObject* obj, ObjectConnection connection, ObjectEdgeSide edge);
