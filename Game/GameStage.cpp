#include "GameStage.h"
#include "GameObject.h"
#include "GameObjects.h"
#include <assert.h>
#include <direct.h>
#include <io.h>
#include <ctype.h>

// 変数の定義 ==============================================================

// <<ステージ情報>>

// <ステージ情報作成>
StageInfo StageInfo_Create(const char* dirpath, const char* name)
{
	StageInfo stage;
	strcpy(stage.title, name);
	snprintf(stage.filepath, MAX_PATH, "%s/%s.dat", dirpath, name);
	snprintf(stage.screenshot_filepath, MAX_PATH, "%s/%s.png", dirpath, name);
	return stage;
}

// <ステージ情報作成>
StageInfo StageInfo_CreateFromFilename(const char* dirpath, const char* filename)
{
	char str[MAX_PATH];
	{
		char *lastdot;
		strcpy(str, filename);
		lastdot = strrchr(str, '.');
		if (lastdot != NULL)
			*lastdot = '\0';
	}
	return StageInfo_Create(dirpath, str);
}

// 文字列比較連番対応版
static int strcmp_ex(const char* s1, const char* s2)
{
	const unsigned char *ss1, *ss2;
	for (ss1 = (const unsigned char*)s1, ss2 = (const unsigned char*)s2;;)
	{
		if (*ss1 != '\0' && *ss2 != '\0' && isdigit(*ss1) && isdigit(*ss2))
		{
			int si1, si2;
			for (si1 = 0; *ss1 != '\0' && isdigit(*ss1); ss1++)
				si1 = si1 * 10 + (*ss1 - '0');
			for (si2 = 0; *ss2 != '\0' && isdigit(*ss2); ss2++)
				si2 = si2 * 10 + (*ss2 - '0');
			if (si1 != si2)
				return si1 - si2;
		}
		if (*ss1 != *ss2 || *ss1 == '\0')
			return *ss1 - *ss2;
		ss1++, ss2++;
	}
}

// ステージ比較
static int stagecmp(const StageInfo* a, const StageInfo* b)
{
	return strcmp_ex(a->title, b->title);
}

// クイックソート実装
static void QuickSortImpl(Vector* array, int left, int right) {
	if (left >= right) return;

	int l = left;
	int r = right;
	// 中央からピボットを選択
	StageInfo pivot = *(StageInfo*)Vector_Get(array, (left + right) / 2);
	StageInfo buf;

	while (1) {
		while (stagecmp((StageInfo*)Vector_Get(array, l), &pivot) < 0) l++;
		while (stagecmp((StageInfo*)Vector_Get(array, r), &pivot) > 0) r--;

		if (l > r) break;

		// pivotより小さい値を左、大きい値を右に入れ替え
		buf = *(StageInfo*)Vector_Get(array, l);
		Vector_Set(array, l, Vector_Get(array, r));
		Vector_Set(array, r, &buf);

		l++, r--;
	}

	// pivotより小さい領域と大きい領域をそれぞれ再帰的にソート
	QuickSortImpl(array, left, r);
	QuickSortImpl(array, l, right);
}

// クイックソート
static void QuickSort(Vector* array) {
	QuickSortImpl(array, 1, Vector_GetSize(array) - 1);
}

// <ステージ情報検索>
void StageInfo_SearchFiles(Vector* stageinfos, char* path, char* filter)
{
	struct _finddata_t fdata;

	intptr_t fh = _findfirst(filter, &fdata);

	Vector_Clear(stageinfos);
	if (fh != -1)
	{
		do {
			if ((fdata.attrib & _A_SUBDIR) == 0)
			{
				StageInfo stage = StageInfo_CreateFromFilename(path, fdata.name);
				Vector_AddLast(stageinfos, &stage);
			}
		} while (_findnext(fh, &fdata) == 0);
		_findclose(fh);
	}

	QuickSort(stageinfos);
}

// <ステージフォルダ作成>
void StageInfo_InitDirectory(char* path)
{
	_mkdir(path);
}

// <ステージ情報検索>
void StageInfo_InitAndSearchFiles(Vector* stageinfos, char* path, char* filter)
{
	StageInfo_InitDirectory(path);
	StageInfo_SearchFiles(stageinfos, path, filter);
}

// <<ステージオブジェクト>>

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
