#pragma once
#include "Vec2.h"

// �֐��̒�` ==============================================================

// �L�[�X�V
void UpdateInputManager(void);

// <�W���C�p�b�h>

// �W���C�p�b�h��������Ă��邩
BOOL IsJoypadDown(int key);

// �W���C�p�b�h��������Ă��邩
BOOL IsJoypadUp(int key);

// �W���C�p�b�h�����������ォ
BOOL IsJoypadPressed(int key);

// �W���C�p�b�h�𗣂������ォ
BOOL IsJoypadReleased(int key);

// �W���C�p�b�h������
void ConsumeJoypad(int key);

// <�L�[>

// �L�[��������Ă��邩
BOOL IsKeyDown(int key);

// �L�[��������Ă��邩
BOOL IsKeyUp(int key);

// �L�[�����������ォ
BOOL IsKeyPressed(int key);

// �L�[�𗣂������ォ
BOOL IsKeyReleased(int key);

// �L�[������
void ConsumeKey(int key);

// <�}�E�X>

// �}�E�X��������Ă��邩
BOOL IsMouseDown(int mouse);

// �}�E�X��������Ă��邩
BOOL IsMouseUp(int mouse);

// �}�E�X�����������ォ
BOOL IsMousePressed(int mouse);

// �}�E�X�𗣂������ォ
BOOL IsMouseReleased(int mouse);

// �}�E�X������
void ConsumeMouse(int key);

// �}�E�X�����W���擾
Vec2 GetMousePosition(void);

// �}�E�X�����W��ݒ�
void SetMousePosition(const Vec2* pos);
