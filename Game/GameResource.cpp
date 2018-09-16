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

	// フォント
	// カスタムフォントファイルが読み込めたら使う
	if (AddFontResourceEx(FONT_FILE_MAIN_CUSTOM, FR_PRIVATE, NULL) > 0)
		res.font_main = CreateFontToHandle(FONT_NAME_MAIN_CUSTOM, FONT_SIZE_MAIN, 3, DX_FONTTYPE_ANTIALIASING_4X4);
	else
		res.font_main = CreateFontToHandle(FONT_NAME, FONT_SIZE_MAIN, 3, DX_FONTTYPE_ANTIALIASING_4X4);
	res.font_menu = CreateFontToHandle(FONT_NAME_MENU, FONT_SIZE_MENU, 16, DX_FONTTYPE_ANTIALIASING_4X4);

	// サウンド


	// テクスチャ
	// ロゴ動画
	res.movie_logo = LoadGraph("Resources\\Movies\\ydeagames.avi");
	// タイトル

	// プレイ
	res.texture_planet1 = LoadGraph("Resources\\Textures\\Protected\\StarLuster_OBJ_other.png");

	return res;
}

// <リソース開放>
void GameResource_Delete(GameResource* res)
{
	// フォント
	DeleteFontToHandle(res->font_main);
	RemoveFontResourceEx(FONT_FILE_MAIN_CUSTOM, FR_PRIVATE, NULL);

	// サウンド


	// テクスチャ
	// ロゴ動画
	DeleteGraph(res->movie_logo);
	// タイトル

	// プレイ
	DeleteGraph(res->texture_planet1);
	
	// タイトル

}
