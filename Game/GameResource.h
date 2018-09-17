#pragma once

// 定数の定義 ==============================================================

// <フォント> ----------------------------------------------------------

#define FONT_SIZE_MAIN 80			// フォントサイズ
#define FONT_SIZE_MENU 20

// <リソース> ----------------------------------------------------------

#define NUM_BGM 4
#define NUM_SE 7

// 構造体の宣言 ============================================================

// <ゲームリソース>
typedef struct
{
	// <フォント>
	HFNT font_main;
	HFNT font_menu;

	// <サウンド>
	HSND sound_bgm[NUM_BGM];
	HSND sound_se[NUM_SE];

	// <テクスチャ>
	// タイトル
	
	// プレイ
	HGRP texture_planet1;
	HGRP texture_planet2;
	HGRP texture_planet3;
	HGRP texture_planet4;
	HGRP texture_planet5;
	HGRP texture_cursor1;

	// リザルト

	// <動画>
	HGRP movie_logo;
} GameResource;

// 関数の宣言 ==============================================================

// <<リソース>> --------------------------------------------------

// <リソース作成読み込み>
GameResource GameResource_Create(void);

// <リソース開放>
void GameResource_Delete(GameResource* res);
