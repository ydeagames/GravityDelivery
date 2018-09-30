#include "ScreenShot.h"
#include "GameMain.h"

#include <direct.h>
#include <string>
#include <shlwapi.h>
#include <time.h>
#pragma comment( lib , "shlwapi.lib" )

#define DIR_NAME "Screenshots"

static BOOL CheckExistenceOfFolder(const std::string folder_name);	// ���̖��O�̃t�H���_�����݂��邩�ǂ��� / ���݂����Ȃ�FALSE��Ԃ�

static BOOL CheckExistenceOfFolder(const std::string folder_name)
{
	return (_mkdir(folder_name.c_str()) == 0);
}

BOOL SaveScreenShotToPNG(const GameObject* area, char* name)
{
	int i;
	// ScreenShot �Ƃ����t�H���_�����邩�m�F
	if (CheckExistenceOfFolder(DIR_NAME))
		// ������΍��
		_mkdir(DIR_NAME);
	{
		char timestr[32];
		char str[64];
		time_t temp = time(NULL);
		struct tm* timeptr = localtime(&temp);
		size_t ch = strftime(timestr, sizeof(timestr) - 1, "%Y-%m-%d_%H.%M.%S", timeptr);
		for (i = 1; i < 1000; i++)
		{
			if (i<=1)
				sprintf(str, DIR_NAME"/%s.png", timestr);
			else
				sprintf(str, DIR_NAME"/%s_%d.png", timestr, i);
			if (PathFileExists(str) == 0)
			{
				if (name != NULL)
					strcpy(name, str);
				return (SaveDrawScreenToPNG((int)GameObject_GetX(area, LEFT), (int)GameObject_GetY(area, TOP), (int)GameObject_GetX(area, RIGHT), (int)GameObject_GetY(area, BOTTOM), str) == 0);
			}
		}
	}
	return FALSE;
}

void SaveScreenShotToPNG_Log(const GameObject* area)
{
	char name[64] = "[Error]";
	if (SaveScreenShotToPNG(&g_field, name))
		DebugConsole_LogF(&g_console, "Saved screenshot as %s", name);
	else
		DebugConsole_LogF(&g_console, "Couldn't save screenshot: %s", name);
}

void OpenScreenShotDir(void)
{
	ShellExecute(NULL, "open", DIR_NAME, NULL, NULL, SW_SHOWNORMAL);
}