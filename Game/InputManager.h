#pragma once
#include "Vec2.h"

// �֐��̒�` ==============================================================

// �L�[�X�V
void UpdateInputManager(void);

// �L�[��������Ă��邩
BOOL IsJoypadDown(int key);

// �L�[��������Ă��邩
BOOL IsJoypadUp(int key);

// �L�[�����������ォ
BOOL IsJoypadPressed(int key);

// �L�[�𗣂������ォ
BOOL IsJoypadReleased(int key);

// �L�[��������Ă��邩
BOOL IsKeyDown(int key);

// �L�[��������Ă��邩
BOOL IsKeyUp(int key);

// �L�[�����������ォ
BOOL IsKeyPressed(int key);

// �L�[�𗣂������ォ
BOOL IsKeyReleased(int key);

// �}�E�X��������Ă��邩
BOOL IsMouseDown(int mouse);

// �}�E�X��������Ă��邩
BOOL IsMouseUp(int mouse);

// �}�E�X�����������ォ
BOOL IsMousePressed(int mouse);

// �}�E�X�𗣂������ォ
BOOL IsMouseReleased(int mouse);

// �}�E�X�����W���擾
Vec2 GetMousePosition(void);

// �}�E�X�����W��ݒ�
void SetMousePosition(const Vec2* pos);
