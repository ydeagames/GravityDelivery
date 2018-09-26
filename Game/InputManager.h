#pragma once
#include "Vec2.h"

// 関数の定義 ==============================================================

// キー更新
void UpdateInputManager(void);

// キーが押されているか
BOOL IsJoypadDown(int key);

// キーが離されているか
BOOL IsJoypadUp(int key);

// キーを押した直後か
BOOL IsJoypadPressed(int key);

// キーを離した直後か
BOOL IsJoypadReleased(int key);

// キーが押されているか
BOOL IsKeyDown(int key);

// キーが離されているか
BOOL IsKeyUp(int key);

// キーを押した直後か
BOOL IsKeyPressed(int key);

// キーを離した直後か
BOOL IsKeyReleased(int key);

// マウスが押されているか
BOOL IsMouseDown(int mouse);

// マウスが離されているか
BOOL IsMouseUp(int mouse);

// マウスを押した直後か
BOOL IsMousePressed(int mouse);

// マウスを離した直後か
BOOL IsMouseReleased(int mouse);

// マウスを座標を取得
Vec2 GetMousePosition(void);

// マウスを座標を設定
void SetMousePosition(const Vec2* pos);
