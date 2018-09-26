#include "InputManager.h"

// �O���[�o���ϐ��̒�` ====================================================

static int g_input_state;
static int g_input_state_last;

static char g_key_input_state[256];
static char g_key_input_state_last[256];

static int g_mouse_state;
static int g_mouse_state_last;

// �֐��̒�` ==============================================================

// �L�[�X�V
void UpdateInputManager(void)
{
	g_input_state_last = g_input_state;
	g_input_state = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	memcpy(g_key_input_state_last, g_key_input_state, sizeof(g_key_input_state));
	GetHitKeyStateAll(g_key_input_state);

	g_mouse_state_last = g_mouse_state;
	g_mouse_state = GetMouseInput();
}

// �W���C�p�b�h��������Ă��邩
BOOL IsJoypadDown(int key)
{
	return g_input_state & key;
}

// �W���C�p�b�h��������Ă��邩
BOOL IsJoypadUp(int key)
{
	return !IsJoypadDown(key);
}

// �W���C�p�b�h�����������ォ
BOOL IsJoypadPressed(int key)
{
	return !(g_input_state_last & key) && (g_input_state & key);
}

// �W���C�p�b�h�𗣂������ォ
BOOL IsJoypadReleased(int key)
{
	return (g_input_state_last & key) && !(g_input_state & key);
}

// �L�[��������Ă��邩
BOOL IsKeyDown(int key)
{
	return g_key_input_state[key];
}

// �L�[��������Ă��邩
BOOL IsKeyUp(int key)
{
	return !IsKeyDown(key);
}

// �L�[�����������ォ
BOOL IsKeyPressed(int key)
{
	return !g_key_input_state_last[key] && g_key_input_state[key];
}

// �L�[�𗣂������ォ
BOOL IsKeyReleased(int key)
{
	return g_key_input_state_last[key] && !g_key_input_state[key];
}

// �}�E�X��������Ă��邩
BOOL IsMouseDown(int mouse)
{
	return g_mouse_state & mouse;
}

// �}�E�X��������Ă��邩
BOOL IsMouseUp(int mouse)
{
	return !IsMouseDown(mouse);
}

// �}�E�X�����������ォ
BOOL IsMousePressed(int mouse)
{
	return !(g_mouse_state_last & mouse) && (g_mouse_state & mouse);
}

// �}�E�X�𗣂������ォ
BOOL IsMouseReleased(int mouse)
{
	return (g_mouse_state_last & mouse) && !(g_mouse_state & mouse);
}

// �}�E�X�����W���擾
Vec2 GetMousePosition(void)
{
	int mouse_x, mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return Vec2_Create((float)mouse_x, (float)mouse_y);
}

// �}�E�X�����W��ݒ�
void SetMousePosition(const Vec2* pos)
{
	SetMousePoint((int)pos->x, (int)pos->y);
}