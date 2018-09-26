#pragma once

// 定数の定義 ==============================================================

// <フォント> ----------------------------------------------------------

#define FONT_SIZE_MAIN 20			// フォントサイズ

// <リソース> ----------------------------------------------------------

#define NUM_BGM 4
#define NUM_SE 10

#define NUM_TEXTURE 7

// 構造体の宣言 ============================================================

// <ゲームリソース>
typedef struct
{
	// <フォント>
	HFNT font_main;

	// <サウンド>
	HSND sound_bgm[NUM_BGM];
	HSND sound_se[NUM_SE];

	// <テクスチャ>
	HGRP texture[NUM_TEXTURE];

	// <動画>
	HGRP movie_logo;
} GameResource;

// 関数の宣言 ==============================================================

// <<リソース>> --------------------------------------------------

// <リソース作成読み込み>
GameResource GameResource_Create(void);

// <リソース開放>
void GameResource_Delete(GameResource* res);
