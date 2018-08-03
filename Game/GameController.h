#pragma once
#include "GameObjects.h"

// 構造体の宣言 ============================================================

// <キーセット>
typedef struct
{
	int key_up;								// ↑キー
	int key_down;							// ↓キー
	int key_left;							// ←キー
	int key_right;							// →キー
} PlayerKeySet;

// <ゲームコントローラー>
typedef struct tag_GameController GameController;
struct tag_GameController
{
	const GameObject* field;					// フィールドオブジェクト
	GameObject* object;							// コントロールオブジェクト
	void(*Update)(GameController*);				// コントローラ更新関数
	void(*UpdateControl)(GameController*);		// コントローラ操作関数

	// 継承ができないので仕方なくPlayer用の変数
	PlayerKeySet player_keys;
};

// 関数の宣言 ==============================================================

// <<キーセット> -------------------------------------------------------

// <キーセット作成>
PlayerKeySet PlayerKeySet_Create(int key_up, int key_down, int key_left, int key_right);

// <<デフォルトキーセット> ---------------------------------------------

// <キーセット作成>
PlayerKeySet PlayerKeySet_Default_Create(void);

// <<コントローラー>> --------------------------------------------------

// <コントローラー作成>
GameController GameController_Create(const GameObject* field, GameObject* object, void(*updateFunc)(GameController*), void(*updateCtrlFunc)(GameController*));

// <コントローラー更新>
void GameController_Update(GameController* ctrl);

// <コントローラー操作更新>
void GameController_UpdateControl(GameController* ctrl);

// <<デフォルトコントローラー>> ----------------------------------------

// <デフォルトコントローラー作成>
GameController GameController_Default_Create(GameObject* object);

// <<プレイヤーコントローラー>> ----------------------------------------

// <プレイヤーコントローラー作成>
GameController GameController_Player_Create(const GameObject* field, GameObject* object, PlayerKeySet keys);
