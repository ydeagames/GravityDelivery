#include "ScenePlay.h"
#include "GameObject.h"
#include "InputManager.h"


// �萔�̒�` ==============================================================




// �O���[�o���ϐ��̒�` ====================================================

GameObject g_ball;
Vec2 g_mouse_last;



// �֐��̐錾 ==============================================================

void InitializePlay(void);  // �Q�[���̏���������
void UpdatePlay(void);      // �Q�[���̍X�V����
void RenderPlay(void);      // �Q�[���̕`�揈��
void FinalizePlay(void);    // �Q�[���̏I������




// �֐��̒�` ==============================================================

//----------------------------------------------------------------------
//! @brief �Q�[���̏���������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void InitializePlay(void)
{

}



//----------------------------------------------------------------------
//! @brief �Q�[���̍X�V����
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void UpdatePlay(void)
{
	if (IsMousePressed(MOUSE_INPUT_1))
		g_mouse_last = GetMousePosition();
	if (IsMouseReleased(MOUSE_INPUT_1))
	{
		Vec2 vel = Vec2_Scale(&Vec2_Sub(&g_mouse_last, &GetMousePosition()), .1f);
		g_ball = GameObject_Create(g_mouse_last, vel, Vec2_Create(5, 5));
		g_ball.fill = TRUE;
	}

	GameObject_UpdatePosition(&g_ball);
}



//----------------------------------------------------------------------
//! @brief �Q�[���̕`�揈��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void RenderPlay(void)
{
	if (IsMouseDown(MOUSE_INPUT_1))
	{
		Vec2 pos = GetMousePosition();
		Vec2 vel = Vec2_Sub(&g_mouse_last, &pos);
		Vec2_Render(&vel, &pos, COLOR_WHITE);
	}
	if (GameObject_IsAlive(&g_ball))
		GameObject_Render(&g_ball);
}



//----------------------------------------------------------------------
//! @brief �Q�[���̏I������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void FinalizePlay(void)
{
}
