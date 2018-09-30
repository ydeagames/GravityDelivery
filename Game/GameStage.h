#pragma once
#include "Vector.h"

// 構造体の定義 ============================================================

// <ステージ情報>
typedef struct {
	char filepath[MAX_PATH];
	char screenshot_filepath[MAX_PATH];
	char title[MAX_PATH];
} StageInfo;

// <ステージオブジェクト>
typedef struct {
	Vector balls;
	Vector planets;

	int score;
	BOOL edited;
	HSND bgm;
} GameStage;

// 変数の宣言 ==============================================================

// <<ステージ情報>>

// <ステージ情報作成>
StageInfo StageInfo_Create(const char* dirpath, const char* name);

// <ステージ情報作成>
StageInfo StageInfo_CreateFromFilename(const char* dirpath, const char* name);

// <ステージ情報検索>
void StageInfo_SearchFiles(Vector* stageinfos, char* path, char* filter);

// <ステージフォルダ作成>
void StageInfo_InitDirectory(char* path);

// <ステージ情報検索>
void StageInfo_InitAndSearchFiles(Vector* stageinfos, char* path, char* filter);

// <<ステージオブジェクト>>

// <ステージ作成>
GameStage GameStage_Create(void);

// <ステージ破棄>
void GameStage_Dispose(GameStage* gamestage);

// <ステージ読み込み>
void GameStage_Load(GameStage* gamestage, const StageInfo* stageinfo);

// <ステージ保存>
void GameStage_Save(GameStage* gamestage, const StageInfo* stageinfo);
