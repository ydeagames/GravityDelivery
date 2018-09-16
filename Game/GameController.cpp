#include "GameController.h"
#include "InputManager.h"
#include "GameMain.h"

// �񋓌^�̒�` ============================================================

// <�L�[�Z�b�g> --------------------------------------------------------
enum
{
	PLAYER_KEY_UP = PAD_INPUT_UP | PAD_INPUT_8,
	PLAYER_KEY_DOWN = PAD_INPUT_DOWN | PAD_INPUT_5,
	PLAYER_KEY_LEFT = PAD_INPUT_LEFT | PAD_INPUT_4,
	PLAYER_KEY_RIGHT = PAD_INPUT_RIGHT | PAD_INPUT_6
};

// �֐��̐錾 ==============================================================

static void GameController_Default_Update(GameController* ctrl);
static void GameController_Default_UpdateControl(GameController* ctrl);
static void GameController_Player_UpdateControl(GameController* ctrl);

// �֐��̒�` ==============================================================

// <<�L�[�Z�b�g> -------------------------------------------------------

// <�L�[�Z�b�g�쐬>
PlayerKeySet PlayerKeySet_Create(int key_up, int key_down, int key_left, int key_right)
{
	return{ key_up, key_down, key_left, key_right };
}

// <<�f�t�H���g�L�[�Z�b�g> ---------------------------------------------

// <�L�[�Z�b�g�쐬>
PlayerKeySet PlayerKeySet_Default_Create(void)
{
	return PlayerKeySet_Create(PLAYER_KEY_UP, PLAYER_KEY_DOWN, PLAYER_KEY_LEFT, PLAYER_KEY_RIGHT);
}

// <<�R���g���[���[>> --------------------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Create(const GameObject* field, GameObject* object, void(*updateFunc)(GameController*), void(*updateCtrlFunc)(GameController*))
{
	return{ field, object, updateFunc, updateCtrlFunc };
}

// <�R���g���[���[�X�V>
void GameController_Update(GameController* ctrl)
{
	ctrl->Update(ctrl);
}

// <�R���g���[���[����X�V>
void GameController_UpdateControl(GameController* ctrl)
{
	ctrl->UpdateControl(ctrl);
}

// <<�f�t�H���g�R���g���[���[>> ----------------------------------------

// <�f�t�H���g�R���g���[���[�쐬>
GameController GameController_Default_Create(GameObject* field, GameObject* object)
{
	return GameController_Create(field, object, GameController_Default_Update, GameController_Default_UpdateControl);
}

void GameController_Default_Update(GameController* ctrl)
{
	ctrl->object->vel = Vec2_Create();
}

void GameController_Default_UpdateControl(GameController* ctrl)
{
}
