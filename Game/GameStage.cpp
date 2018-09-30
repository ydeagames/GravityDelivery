#include "GameStage.h"
#include "GameObject.h"
#include "GameObjects.h"
#include <assert.h>
#include <direct.h>
#include <io.h>

// �ϐ��̒�` ==============================================================

// <<�X�e�[�W���>>

// <�X�e�[�W���쐬>
StageInfo StageInfo_Create(const char* dirpath, const char* name)
{
	StageInfo stage;
	strcpy_s(stage.filename, name);
	snprintf(stage.filepath, MAX_PATH, "%s/%s", dirpath, name);
	{
		char *lastdot;
		strcpy(stage.title, stage.filename);
		lastdot = strrchr(stage.title, '.');
		if (lastdot != NULL)
			*lastdot = '\0';
	}
	return stage;
}

// <�X�e�[�W��񌟍�>
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
				StageInfo stage = StageInfo_Create(path, fdata.name);
				Vector_AddLast(stageinfos, &stage);
			}
		} while (_findnext(fh, &fdata) == 0);
		_findclose(fh);
	}
}

// <�X�e�[�W�t�H���_�쐬>
void StageInfo_InitDirectory(char* path)
{
	_mkdir(path);
}

// <�X�e�[�W��񌟍�>
void StageInfo_InitAndSearchFiles(Vector* stageinfos, char* path, char* filter)
{
	StageInfo_InitDirectory(path);
	StageInfo_SearchFiles(stageinfos, path, filter);
}

// <<�X�e�[�W�I�u�W�F�N�g>>

// <�X�e�[�W�쐬>
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

// <�X�e�[�W�j��>
void GameStage_Dispose(GameStage* gamestage)
{
	Vector_Delete(&gamestage->balls);
	Vector_Delete(&gamestage->planets);
}

// <�X�e�[�W�ǂݍ���>
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

// <�X�e�[�W�ۑ�>
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
