#include "GameResource.h"
#include "GameObject.h"

// 定数の定義 ==============================================================

// <フォント> ----------------------------------------------------------
#define FONT_FILE_MAIN_CUSTOM "Resources\\Fonts\\uzura.ttf"						// カスタムフォントファイル
#define FONT_NAME_MAIN_CUSTOM "うずらフォント"									// カスタムフォントネーム
#define FONT_NAME "HGP創英角ｺﾞｼｯｸUB"											// フォールバック用フォントネーム

// 関数の定義 ==============================================================

// <<リソース>> --------------------------------------------------

// <リソース作成読み込み>
GameResource GameResource_Create(void)
{
	GameResource res;

	// ロゴ動画
	res.movie_logo = LoadGraph("Resources/Movies/ydeagames.mp4");

	// フォント
	// カスタムフォントファイルが読み込めたら使う
	if (AddFontResourceEx(FONT_FILE_MAIN_CUSTOM, FR_PRIVATE, NULL) > 0)
		res.font_main = CreateFontToHandle(FONT_NAME_MAIN_CUSTOM, FONT_SIZE_MAIN, 1, DX_FONTTYPE_ANTIALIASING_4X4);
	else
		res.font_main = CreateFontToHandle(FONT_NAME, FONT_SIZE_MAIN, 1, DX_FONTTYPE_ANTIALIASING_4X4);

	// サウンド
	res.sound_bgm[0] = LoadSoundMem("Resources/Audio/Protected/bab_music_01.mp3");
	res.sound_bgm[1] = LoadSoundMem("Resources/Audio/Protected/drb_music_catacomb.wav");
	res.sound_bgm[2] = LoadSoundMem("Resources/Audio/Protected/drb_music_brick_dungeon.wav");
	res.sound_bgm[3] = LoadSoundMem("Resources/Audio/Protected/dru_music_ishtar_theme.wav");
	res.sound_se[0] = LoadSoundMem("Resources/Audio/Protected/xev_se_Zakato_DethoutSound.wav");
	ChangeVolumeSoundMem(100, res.sound_se[0]);
	res.sound_se[1] = LoadSoundMem("Resources/Audio/button45.mp3");
	res.sound_se[2] = LoadSoundMem("Resources/Audio/activate.mp3");
	ChangeVolumeSoundMem(150, res.sound_se[2]);
	res.sound_se[3] = LoadSoundMem("Resources/Audio/short01a.wav");
	ChangeVolumeSoundMem(180, res.sound_se[3]);
	res.sound_se[4] = LoadSoundMem("Resources/Audio/button55.mp3");
	res.sound_se[5] = LoadSoundMem("Resources/Audio/cancel1.mp3");
	ChangeVolumeSoundMem(180, res.sound_se[5]);
	res.sound_se[6] = LoadSoundMem("Resources/Audio/bomb1.mp3");
	ChangeVolumeSoundMem(150, res.sound_se[6]);
	res.sound_se[7] = LoadSoundMem("Resources/Audio/clear.wav");
	ChangeVolumeSoundMem(150, res.sound_se[7]);
	res.sound_se[8] = LoadSoundMem("Resources/Audio/gun-fire05.mp3");
	ChangeVolumeSoundMem(150, res.sound_se[8]);
	res.sound_se[9] = LoadSoundMem("Resources/Audio/teleport01.mp3");
	ChangeVolumeSoundMem(150, res.sound_se[9]);
	res.sound_se[10] = LoadSoundMem("Resources/Audio/beam-gun03.mp3");
	ChangeVolumeSoundMem(170, res.sound_se[10]);
	res.sound_se[11] = LoadSoundMem("Resources/Audio/system31.mp3");
	ChangeVolumeSoundMem(140, res.sound_se[11]);
	res.sound_se[12] = LoadSoundMem("Resources/Audio/gun-fire04.mp3");
	res.sound_se[13] = LoadSoundMem("Resources/Audio/spo_ge_mbat02.mp3");
	ChangeVolumeSoundMem(210, res.sound_se[13]);
	res.sound_se[14] = LoadSoundMem("Resources/Audio/beam-gun05.mp3");
	ChangeVolumeSoundMem(120, res.sound_se[14]);
	res.sound_se[15] = LoadSoundMem("Resources/Audio/deactivate.mp3");
	ChangeVolumeSoundMem(150, res.sound_se[15]);

	// テクスチャ
	res.texture[0] = LoadGraph("Resources/Textures/bomb.png");
	res.texture[1] = LoadGraph("Resources/Textures/goal.png");
	res.texture[2] = LoadGraph("Resources/Textures/planets.png");
	res.texture[3] = LoadGraph("Resources/Textures/start.png");
	res.texture[4] = LoadGraph("Resources/Textures/warp.png");
	res.texture[5] = LoadGraph("Resources/Textures/launcher.png");
	res.texture[6] = LoadGraph("Resources/Textures/doom.png");
	res.texture[7] = LoadGraph("Resources/Textures/goal_doom.png");
	res.texture[8] = LoadGraph("Resources/Textures/gravity_switch.png");
	res.texture[9] = LoadGraph("Resources/Textures/cursor.png");

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

	// テクスチャ
	{
		int i;
		for (i = 0; i < NUM_TEXTURE; i++)
			DeleteGraph(res->texture[i]);
	}
}
