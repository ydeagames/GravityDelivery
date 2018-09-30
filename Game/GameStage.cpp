#include "GameStage.h"
#include "GameObject.h"
#include "GameObjects.h"
#include <assert.h>

// 変数の定義 ==============================================================

// <ステージ作成>
GameStage GameStage_Create(void)
{
	GameStage gamestage = {};

	gamestage.edited = FALSE;

	gamestage.balls = Vector_Create(sizeof(GameObject));
	gamestage.planets = Vector_Create(sizeof(GameObject));

	gamestage.score = 0;

	gamestage.bgm = 0;

	return gamestage;
}

// <ステージ破棄>
void GameStage_Dispose(GameStage* gamestage)
{
	Vector_Delete(&gamestage->balls);
	Vector_Delete(&gamestage->planets);
}

// <ステージ読み込み>
void GameStage_Load(GameStage* gamestage, const StageInfo* stageinfo)
{
	FILE* fp;
	const char* fname = stageinfo->filepath;

	errno_t err = fopen_s(&fp, fname, "r");

	//assert(err == 0 && "file not opened!");
	if (err == 0)
	{
		int type;
		float base_x, base_y;
		float next_x, next_y;

		Vector_Clear(&gamestage->balls);
		Vector_Clear(&gamestage->planets);

		fscanf_s(fp, "%d", &gamestage->bgm);
		while (fscanf_s(fp, "%d %f %f %f %f", &type, &base_x, &base_y, &next_x, &next_y) != EOF) {
			Vec2 base = Vec2_Create(base_x, base_y);
			Vec2 next = Vec2_Create(next_x, next_y);
			GameObject obj = GameObject_Planets_Create(type, &base, &next);

			Vector_AddLast(&gamestage->planets, &obj);
		}

		fclose(fp);

		gamestage->edited = FALSE;
		gamestage->score = 0;
	}
}

// <ステージ保存>
void GameStage_Save(GameStage* gamestage, const StageInfo* stageinfo)
{
	FILE* fp;
	const char* fname = stageinfo->filepath;

	errno_t err = fopen_s(&fp, fname, "w");
	assert(err == 0 && "file not opened!");

	{
		fprintf_s(fp, "%d\n", gamestage->bgm);
		foreach_start(&gamestage->planets, GameObject, obj)
		{
			Vec2 base;
			Vec2 next;
			GameObject_Planets_Serialize(obj, &base, &next);

			fprintf_s(fp, "%d %f %f %f %f\n", obj->type, base.x, base.y, next.x, next.y);
		} foreach_end;

		fclose(fp);

		gamestage->edited = FALSE;
		gamestage->score = 0;
	}
}
