#pragma once
#include "GameObjects.h"

// �\���̂̐錾 ============================================================

// <�L�[�Z�b�g>
typedef struct
{
	int key_up;								// ���L�[
	int key_down;							// ���L�[
	int key_left;							// ���L�[
	int key_right;							// ���L�[
} PlayerKeySet;

// <�Q�[���R���g���[���[>
typedef struct tag_GameController GameController;
struct tag_GameController
{
	const GameObject* field;					// �t�B�[���h�I�u�W�F�N�g
	GameObject* object;							// �R���g���[���I�u�W�F�N�g
	void(*Update)(GameController*);				// �R���g���[���X�V�֐�
	void(*UpdateControl)(GameController*);		// �R���g���[������֐�

	// �p�����ł��Ȃ��̂Ŏd���Ȃ�Player�p�̕ϐ�
	PlayerKeySet player_keys;
};

// �֐��̐錾 ==============================================================

// <<�L�[�Z�b�g> -------------------------------------------------------

// <�L�[�Z�b�g�쐬>
PlayerKeySet PlayerKeySet_Create(int key_up, int key_down, int key_left, int key_right);

// <<�f�t�H���g�L�[�Z�b�g> ---------------------------------------------

// <�L�[�Z�b�g�쐬>
PlayerKeySet PlayerKeySet_Default_Create(void);

// <<�R���g���[���[>> --------------------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Create(const GameObject* field, GameObject* object, void(*updateFunc)(GameController*), void(*updateCtrlFunc)(GameController*));

// <�R���g���[���[�X�V>
void GameController_Update(GameController* ctrl);

// <�R���g���[���[����X�V>
void GameController_UpdateControl(GameController* ctrl);

// <<�f�t�H���g�R���g���[���[>> ----------------------------------------

// <�f�t�H���g�R���g���[���[�쐬>
GameController GameController_Default_Create(GameObject* object);
