#pragma once
#include "Vector.h"

// �\���̂̒�` ============================================================

typedef struct {
	char filename[MAX_PATH];
	char filepath[MAX_PATH];
	char title[MAX_PATH];
} StageInfo;

typedef struct {
	Vector balls;
	Vector planets;

	int score;
	BOOL edited;
	HSND bgm;
} GameStage;

// �ϐ��̐錾 ==============================================================

// <�X�e�[�W�쐬>
GameStage GameStage_Create(void);

// <�X�e�[�W�j��>
void GameStage_Dispose(GameStage* gamestage);

// <�X�e�[�W�ǂݍ���>
void GameStage_Load(GameStage* gamestage, const StageInfo* stageinfo);

// <�X�e�[�W�ۑ�>
void GameStage_Save(GameStage* gamestage, const StageInfo* stageinfo);
