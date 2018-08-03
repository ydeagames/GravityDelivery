#pragma once

// 構造体の宣言 ============================================================

// <ゲームリソース>
typedef struct
{
	// <フォント>
	HFNT font_main;
	HFNT font_menu;

	// <サウンド>
	HSND sound_bgm;
	HSND sound_se_hit;
	HSND sound_se_hit_boss;
	HSND sound_se_hit_boss_dead;
	HSND sound_se_charge;
	HSND sound_se_shoot;
	HSND sound_se_shootbig;
	HSND sound_se_dead;

	// <テクスチャ>
	// タイトル
	HGRP texture_title;
	HGRP texture_title_text;
	// プレイ
	HGRP texture_player;
	HGRP texture_bullet;
	HGRP texture_enemy;
	HGRP texture_explosion;
	HGRP texture_map;
	HGRP texture_cloud;
	// リザルト
	HGRP texture_result;
	HGRP texture_result_text;

	// <動画>
	HGRP movie_logo;
} GameResource;

// 定数の定義 ==============================================================

// <フォント> ----------------------------------------------------------

#define FONT_SIZE_MAIN 80			// フォントサイズ
#define FONT_SIZE_MENU 20

// 関数の宣言 ==============================================================

// <<リソース>> --------------------------------------------------

// <リソース作成読み込み>
GameResource GameResource_Create(void);

// <リソース開放>
void GameResource_Delete(GameResource* res);
