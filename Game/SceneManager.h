#pragma once
#include "Easings.h"

// �񋓌^�̒�` ============================================================

// <�V�[��> ------------------------------------------------------------

enum tag_SceneID
{
	SCENE_NONE = -1,
	SCENE_LOGO,
	SCENE_TITLE,
	SCENE_PLAY,
	SCENE_RESULT,
	SCENE_UNKNOWN = 100
};
typedef enum tag_SceneID SceneID;

// �O���[�o���ϐ��̐錾 ====================================================

extern BOOL g_close_request;

// �֐��̐錾 ==============================================================

// �V�[���̏���������
void InitializeSceneManager(SceneID start_scene_id);

// �V�[���̍X�V����
void UpdateSceneManager(void);

// �V�[���̕`�揈��
void RenderSceneManager(void);

// �V�[���̏I������
void FinalizeSceneManager(void);

// �V�[���v��
void RequestScene(SceneID scene_id, unsigned int color = COLOR_WHITE, float time = 0.f, float endtime = -1.f, Easings easeFadeOut = ESG_OUTSINE, Easings easeFadeIn = ESG_INSINE);

// �I���v��
void RequestClose(void);

// �V�[��
SceneID GetSceneID(void);
