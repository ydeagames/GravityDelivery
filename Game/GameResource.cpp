#include "GameResource.h"
#include "GameObject.h"

// 定数の定義 ==============================================================

// <フォント> ----------------------------------------------------------
#define FONT_FILE_MAIN_CUSTOM "Resources\\Fonts\\TheSlavicFont-Regular.ttf"		// カスタムフォントファイル
#define FONT_NAME_MAIN_CUSTOM "The Slavic Font"									// カスタムフォントネーム
#define FONT_NAME "HGP創英角ｺﾞｼｯｸUB"										// フォールバック用フォントネーム
#define FONT_NAME_MENU "HGP創英角ｺﾞｼｯｸUB"

// <サウンド> ----------------------------------------------------------
#define SOUND_BGM "Resources\\Audio\\nv_01.mp3"
#define SOUND_SE_HIT "Resources\\Audio\\Protected\\xev_se_EnemyStriken_Zapper.wav"
#define SOUND_SE_HIT_BOSS "Resources\\Audio\\Protected\\xev_se_Zakato_DethoutSound.wav"
#define SOUND_SE_HIT_BOSS_DEAD "Resources\\Audio\\Protected\\xev_se_EnemyStriken_Blaster.wav"
#define SOUND_SE_CHARGE "Resources\\Audio\\Protected\\xev_se_Andorgenesis_loop.wav"
#define SOUND_SE_SHOOT "Resources\\Audio\\Protected\\xev_se_ZapperShoot.wav"
#define SOUND_SE_SHOOTBIG "Resources\\Audio\\Protected\\xev_se_EnemyStriken_Blaster.wav"
#define SOUND_SE_DEAD "Resources\\Audio\\Protected\\xev_se_Miss.wav"

// <テクスチャ> --------------------------------------------------------
// タイトル
#define TEXTURE_TITLE "Resources\\Textures\\Title\\title.png"
#define TEXTURE_TITLE_TEXT "Resources\\Textures\\Title\\press_button.png"
// プレイ
#define TEXTURE_PLAYER_PROTECTED "Resources\\Textures\\Protected\\Solvalou.png"		// リソース保護テクスチャ
#define TEXTURE_PLAYER "Resources\\Textures\\Solvalou.png"							// オープンテクスチャ
#define TEXTURE_BULLET_PROTECTED "Resources\\Textures\\Protected\\Bullet.png"		// リソース保護テクスチャ
#define TEXTURE_BULLET "Resources\\Textures\\Bullet.png"							// オープンテクスチャ
#define TEXTURE_ENEMY_PROTECTED "Resources\\Textures\\Protected\\Enemies.png"		// リソース保護テクスチャ
#define TEXTURE_ENEMY "Resources\\Textures\\Enemies.png"							// オープンテクスチャ
#define TEXTURE_EXPLOSION_PROTECTED "Resources\\Textures\\Protected\\Explosion.png"	// リソース保護テクスチャ
#define TEXTURE_EXPLOSION "Resources\\Textures\\Explosion.png"						// オープンテクスチャ
#define TEXTURE_MAP_PROTECTED "Resources\\Textures\\Protected\\Map.png"				// リソース保護テクスチャ
#define TEXTURE_MAP "Resources\\Textures\\Map.png"									// オープンテクスチャ
#define TEXTURE_CLOUD_PROTECTED "Resources\\Textures\\Protected\\Cloud.png"			// リソース保護テクスチャ
#define TEXTURE_CLOUD "Resources\\Textures\\Cloud.png"								// オープンテクスチャ
// リザルト
#define TEXTURE_RESULT "Resources\\Textures\\Result\\result.png"
#define TEXTURE_RESULT_TEXT "Resources\\Textures\\Result\\press_button_back.png"

// <動画> --------------------------------------------------------------
#define MOVIE_LOGO "Resources\\Movies\\ydeagames.avi"								// ロゴムービー

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
	res.sound_bgm = LoadSoundMem(SOUND_BGM);
	res.sound_se_hit = LoadSoundMem(SOUND_SE_HIT);
	res.sound_se_hit_boss = LoadSoundMem(SOUND_SE_HIT_BOSS);
	res.sound_se_hit_boss_dead = LoadSoundMem(SOUND_SE_HIT_BOSS_DEAD);
	res.sound_se_charge = LoadSoundMem(SOUND_SE_CHARGE);
	res.sound_se_shoot = LoadSoundMem(SOUND_SE_SHOOT);
	res.sound_se_shootbig = LoadSoundMem(SOUND_SE_SHOOTBIG);
	res.sound_se_dead = LoadSoundMem(SOUND_SE_DEAD);

	// テクスチャ
	// ロゴ動画
	res.movie_logo = LoadGraph(MOVIE_LOGO);
	// タイトル
	res.texture_title = LoadGraph(TEXTURE_TITLE);
	res.texture_title_text = LoadGraph(TEXTURE_TITLE_TEXT);
	// プレイ
	res.texture_player = LoadGraph(TEXTURE_PLAYER_PROTECTED);
	if (res.texture_player == TEXTURE_MISSING)										// リソース保護テクスチャが無かったら
		res.texture_player = LoadGraph(TEXTURE_PLAYER);								// オープンテクスチャを読み込む
	res.texture_bullet = LoadGraph(TEXTURE_BULLET_PROTECTED);
	if (res.texture_bullet == TEXTURE_MISSING)										// リソース保護テクスチャが無かったら
		res.texture_bullet = LoadGraph(TEXTURE_BULLET);								// オープンテクスチャを読み込む
	res.texture_enemy = LoadGraph(TEXTURE_ENEMY_PROTECTED);
	if (res.texture_enemy == TEXTURE_MISSING)										// リソース保護テクスチャが無かったら
		res.texture_enemy = LoadGraph(TEXTURE_ENEMY);								// オープンテクスチャを読み込む
	res.texture_explosion = LoadGraph(TEXTURE_EXPLOSION_PROTECTED);
	if (res.texture_explosion == TEXTURE_MISSING)									// リソース保護テクスチャが無かったら
		res.texture_explosion = LoadGraph(TEXTURE_EXPLOSION);						// オープンテクスチャを読み込む
	res.texture_map = LoadGraph(TEXTURE_MAP_PROTECTED);
	if (res.texture_map == TEXTURE_MISSING)											// リソース保護テクスチャが無かったら
		res.texture_map = LoadGraph(TEXTURE_MAP);									// オープンテクスチャを読み込む
	res.texture_cloud = LoadGraph(TEXTURE_CLOUD_PROTECTED);
	if (res.texture_cloud == TEXTURE_MISSING)										// リソース保護テクスチャが無かったら
		res.texture_cloud = LoadGraph(TEXTURE_CLOUD);								// オープンテクスチャを読み込む
	// リザルト
	res.texture_result = LoadGraph(TEXTURE_RESULT);
	res.texture_result_text = LoadGraph(TEXTURE_RESULT_TEXT);

	return res;
}

// <リソース開放>
void GameResource_Delete(GameResource* res)
{
	// フォント
	DeleteFontToHandle(res->font_main);
	RemoveFontResourceEx(FONT_FILE_MAIN_CUSTOM, FR_PRIVATE, NULL);

	// サウンド
	DeleteSoundMem(res->sound_bgm);
	DeleteSoundMem(res->sound_se_hit);
	DeleteSoundMem(res->sound_se_hit_boss);
	DeleteSoundMem(res->sound_se_hit_boss_dead);
	DeleteSoundMem(res->sound_se_charge);
	DeleteSoundMem(res->sound_se_shoot);
	DeleteSoundMem(res->sound_se_shootbig);
	DeleteSoundMem(res->sound_se_dead);

	// テクスチャ
	// ロゴ動画
	DeleteGraph(res->movie_logo);
	// タイトル
	DeleteGraph(res->texture_title);
	DeleteGraph(res->texture_title_text);
	// プレイ
	DeleteGraph(res->texture_player);
	DeleteGraph(res->texture_bullet);
	DeleteGraph(res->texture_enemy);
	DeleteGraph(res->texture_explosion);
	DeleteGraph(res->texture_map);
	DeleteGraph(res->texture_cloud);
	// タイトル
	DeleteGraph(res->texture_result);
	DeleteGraph(res->texture_result_text);
}
