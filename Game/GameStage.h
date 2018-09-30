#pragma once
#include "Vector.h"

// �\���̂̒�` ============================================================

// <�X�e�[�W���>
typedef struct {
	char filepath[MAX_PATH];
	char screenshot_filepath[MAX_PATH];
	char title[MAX_PATH];
} StageInfo;

// <�X�e�[�W�I�u�W�F�N�g>
typedef struct {
	Vector balls;
	Vector planets;

	int score;
	BOOL edited;
	HSND bgm;
} GameStage;

// �ϐ��̐錾 ==============================================================

// <<�X�e�[�W���>>

// <�X�e�[�W���쐬>
StageInfo StageInfo_Create(const char* dirpath, const char* name);

// <�X�e�[�W���쐬>
StageInfo StageInfo_CreateFromFilename(const char* dirpath, const char* name);

// <�X�e�[�W��񌟍�>
void StageInfo_SearchFiles(Vector* stageinfos, char* path, char* filter);

// <�X�e�[�W�t�H���_�쐬>
void StageInfo_InitDirectory(char* path);

// <�X�e�[�W��񌟍�>
void StageInfo_InitAndSearchFiles(Vector* stageinfos, char* path, char* filter);

// <<�X�e�[�W�I�u�W�F�N�g>>

// <�X�e�[�W�쐬>
GameStage GameStage_Create(void);

// <�X�e�[�W�j��>
void GameStage_Dispose(GameStage* gamestage);

// <�X�e�[�W�ǂݍ���>
void GameStage_Load(GameStage* gamestage, const StageInfo* stageinfo);

// <�X�e�[�W�ۑ�>
void GameStage_Save(GameStage* gamestage, const StageInfo* stageinfo);
