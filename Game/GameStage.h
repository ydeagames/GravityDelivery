#pragma once
#include "Vector.h"

// 構造体の定義 ============================================================

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

// 変数の宣言 ==============================================================

// <ステージ作成>
GameStage GameStage_Create(void);

// <ステージ破棄>
void GameStage_Dispose(GameStage* gamestage);

// <ステージ読み込み>
void GameStage_Load(GameStage* gamestage, const StageInfo* stageinfo);

// <ステージ保存>
void GameStage_Save(GameStage* gamestage, const StageInfo* stageinfo);
