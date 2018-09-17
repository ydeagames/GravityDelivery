#include "GameResource.h"
#include "GameObject.h"

// 定数の定義 ==============================================================

// <フォント> ----------------------------------------------------------
#define FONT_FILE_MAIN_CUSTOM "Resources\\Fonts\\TheSlavicFont-Regular.ttf"		// カスタムフォントファイル
#define FONT_NAME_MAIN_CUSTOM "The Slavic Font"									// カスタムフォントネーム
#define FONT_NAME "HGP創英角ｺﾞｼｯｸUB"										// フォールバック用フォントネーム
#define FONT_NAME_MENU "HGP創英角ｺﾞｼｯｸUB"

// 関数の定義 ==============================================================

// <<リソース>> --------------------------------------------------

// <リソース作成読み込み>
GameResource GameResource_Create(void)
{
	GameResource res;

	// ロゴ動画
	res.movie_logo = LoadGraph("Resources/Movies/ydeagames.avi");

	// フォント
	// カスタムフォントファイルが読み込めたら使う
	if (AddFontResourceEx(FONT_FILE_MAIN_CUSTOM, FR_PRIVATE, NULL) > 0)
		res.font_main = CreateFontToHandle(FONT_NAME_MAIN_CUSTOM, FONT_SIZE_MAIN, 3, DX_FONTTYPE_ANTIALIASING_4X4);
	else
		res.font_main = CreateFontToHandle(FONT_NAME, FONT_SIZE_MAIN, 3, DX_FONTTYPE_ANTIALIASING_4X4);
	res.font_menu = CreateFontToHandle(FONT_NAME_MENU, FONT_SIZE_MENU, 16, DX_FONTTYPE_ANTIALIASING_4X4);

	// サウンド
	res.sound_bgm[0] = LoadSoundMem("Resources/Audio/Protected/bab_music_01.mp3");
	res.sound_bgm[1] = LoadSoundMem("Resources/Audio/Protected/drb_music_catacomb.wav");
	res.sound_bgm[2] = LoadSoundMem("Resources/Audio/Protected/drb_music_brick_dungeon.wav");
	res.sound_bgm[3] = LoadSoundMem("Resources/Audio/Protected/dru_music_ishtar_theme.wav");
	res.sound_se[0] = LoadSoundMem("Resources/Audio/Protected/xev_se_Zakato_DethoutSound.wav");
	res.sound_se[1] = LoadSoundMem("Resources/Audio/Protected/bc_se_17.wav");
	res.sound_se[2] = LoadSoundMem("Resources/Audio/Protected/genpei_se_06.wav");
	res.sound_se[3] = LoadSoundMem("Resources/Audio/Protected/wgl_se_08.wav");
	res.sound_se[4] = LoadSoundMem("Resources/Audio/Protected/bc_se_16.wav");
	res.sound_se[5] = LoadSoundMem("Resources/Audio/Protected/wonder_se_13.wav");
	res.sound_se[6] = LoadSoundMem("Resources/Audio/Protected/xev_se_Miss.wav");
	res.sound_se[7] = LoadSoundMem("Resources/Audio/Protected/dig_music_StageClear.wav");

	// テクスチャ
	// タイトル
	res.texture_title1 = LoadGraph("Resources/Textures/gravity_switch.png");

	// プレイ
	res.texture_planet1 = LoadGraph("Resources/Textures/Protected/StarLuster_OBJ_other.png");
	res.texture_planet2 = LoadGraph("Resources/Textures/Protected/StarLuster_OBJ_enemy.png");
	res.texture_planet3 = LoadGraph("Resources/Textures/Protected/StarLuster_OBJ_enemy_big.png");
	res.texture_planet4 = LoadGraph("Resources/Textures/Protected/StarLuster_OBJ_photonTorpedo.png");
	res.texture_planet5 = LoadGraph("Resources/Textures/Protected/StarLuster_OBJ_supplyBase.png");
	res.texture_cursor1 = LoadGraph("Resources/Textures/Protected/Xevious_OBJ_solvalou.png");

	return res;
}

// <リソース開放>
void GameResource_Delete(GameResource* res)
{
	// フォント
	DeleteFontToHandle(res->font_main);
	RemoveFontResourceEx(FONT_FILE_MAIN_CUSTOM, FR_PRIVATE, NULL);

	// サウンド
	{
		int i;
		for (i = 0; i < NUM_BGM; i++)
			DeleteSoundMem(res->sound_bgm[i]);
		for (i = 0; i < NUM_SE; i++)
			DeleteSoundMem(res->sound_se[i]);
	}

	// テクスチャ
	// ロゴ動画
	DeleteGraph(res->movie_logo);
	// タイトル
	DeleteGraph(res->texture_title1);

	// プレイ
	DeleteGraph(res->texture_planet1);
	DeleteGraph(res->texture_planet2);
	DeleteGraph(res->texture_planet3);
	DeleteGraph(res->texture_planet4);
	DeleteGraph(res->texture_planet5);
	DeleteGraph(res->texture_cursor1);

	// タイトル

}
