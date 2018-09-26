#include "InputManager.h"

// グローバル変数の定義 ====================================================

static int g_input_state;
static int g_input_state_last;

static char g_key_input_state[256];
static char g_key_input_state_last[256];

static int g_mouse_state;
static int g_mouse_state_last;

// 関数の定義 ==============================================================

// キー更新
void UpdateInputManager(void)
{
	g_input_state_last = g_input_state;
	g_input_state = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	memcpy(g_key_input_state_last, g_key_input_state, sizeof(g_key_input_state));
	GetHitKeyStateAll(g_key_input_state);

	g_mouse_state_last = g_mouse_state;
	g_mouse_state = GetMouseInput();
}

// ジョイパッドが押されているか
BOOL IsJoypadDown(int key)
{
	return g_input_state & key;
}

// ジョイパッドが離されているか
BOOL IsJoypadUp(int key)
{
	return !IsJoypadDown(key);
}

// ジョイパッドを押した直後か
BOOL IsJoypadPressed(int key)
{
	return !(g_input_state_last & key) && (g_input_state & key);
}

// ジョイパッドを離した直後か
BOOL IsJoypadReleased(int key)
{
	return (g_input_state_last & key) && !(g_input_state & key);
}

// キーが押されているか
BOOL IsKeyDown(int key)
{
	return g_key_input_state[key];
}

// キーが離されているか
BOOL IsKeyUp(int key)
{
	return !IsKeyDown(key);
}

// キーを押した直後か
BOOL IsKeyPressed(int key)
{
	return !g_key_input_state_last[key] && g_key_input_state[key];
}

// キーを離した直後か
BOOL IsKeyReleased(int key)
{
	return g_key_input_state_last[key] && !g_key_input_state[key];
}

// マウスが押されているか
BOOL IsMouseDown(int mouse)
{
	return g_mouse_state & mouse;
}

// マウスが離されているか
BOOL IsMouseUp(int mouse)
{
	return !IsMouseDown(mouse);
}

// マウスを押した直後か
BOOL IsMousePressed(int mouse)
{
	return !(g_mouse_state_last & mouse) && (g_mouse_state & mouse);
}

// マウスを離した直後か
BOOL IsMouseReleased(int mouse)
{
	return (g_mouse_state_last & mouse) && !(g_mouse_state & mouse);
}

// マウスを座標を取得
Vec2 GetMousePosition(void)
{
	int mouse_x, mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return Vec2_Create((float)mouse_x, (float)mouse_y);
}

// マウスを座標を設定
void SetMousePosition(const Vec2* pos)
{
	SetMousePoint((int)pos->x, (int)pos->y);
}