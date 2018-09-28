#include "ScreenShot.h"

#include <direct.h>
#include <string>
#include <shlwapi.h>
#pragma comment( lib , "shlwapi.lib" )

static BOOL CheckExistenceOfFolder(const std::string folder_name);	// ���̖��O�̃t�H���_�����݂��邩�ǂ��� / ���݂����Ȃ�FALSE��Ԃ�

static BOOL CheckExistenceOfFolder(const std::string folder_name)
{
	return (_mkdir(folder_name.c_str()) == 0);
}

BOOL SaveScreenShotToPNG(const GameObject* area, char* name)
{
	int i;
	// ScreenShot �Ƃ����t�H���_�����邩�m�F
	if (CheckExistenceOfFolder("ScreenShot"))
		// ������΍��
		_mkdir("ScreenShot");
	for (i = 1; i < 1000; i++)
	{
		char str[64];
		sprintf(str, "Screenshots/shot%03d.png", i);
		if (PathFileExists(str) == 0)
		{
			if (name != NULL)
				strcpy(name, str);
			return (SaveDrawScreenToPNG((int)GameObject_GetX(area, LEFT), (int)GameObject_GetY(area, TOP), (int)GameObject_GetX(area, RIGHT), (int)GameObject_GetY(area, BOTTOM), str) == 0);
		}
	}
	return FALSE;
}
