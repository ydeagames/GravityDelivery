#include "DebugConsole.h"
#include "GameUtils.h"

// �萔�̒�` ==============================================================

#define LINE_TIME 5.f
#define LINE_TIME_FADE 2.f

// �\���̂̐錾 ============================================================

// <�f�o�b�O�R���\�[��>
typedef struct
{
	char msg[64];
	GameTimer timer;
} DebugConsoleLine;

// �֐��̒�` ==============================================================

// <<�f�o�b�O�R���\�[���s> ------------------------------------------------

// <�f�o�b�O�R���\�[���s�쐬>
static DebugConsoleLine DebugConsoleLine_Create(void)
{
	GameTimer timer = GameTimer_Create();
	GameTimer_SetRemaining(&timer, LINE_TIME);
	GameTimer_Resume(&timer);
	return{ "", timer };
}

// <<�f�o�b�O�R���\�[��> --------------------------------------------------

// <�f�o�b�O�R���\�[���쐬>
DebugConsole DebugConsole_Create(GameObject area, HFNT font, float line_height)
{
	return{ area, font, line_height, Vector_Create(sizeof(DebugConsoleLine)) };
}

// <�f�o�b�O�R���\�[�����O>
void DebugConsole_Log(DebugConsole* console, char* log)
{
	DebugConsoleLine line = DebugConsoleLine_Create();
	snprintf(line.msg, 64, "%.*s", 64, log);
	Vector_AddFirst(&console->lines, &line);
}

// <�f�o�b�O�R���\�[�����O�t�H�[�}�b�g>
void DebugConsole_LogF(DebugConsole* console, char* format, ...)
{
	DebugConsoleLine line = DebugConsoleLine_Create();
	va_list ap;
	va_start(ap, format);
	vsnprintf(line.msg, 64, format, ap);
	va_end(ap);
	Vector_AddFirst(&console->lines, &line);
}

// <�f�o�b�O�R���\�[���`��>
void DebugConsole_Render(DebugConsole* console)
{
	float y = 0;
	foreach_start(&console->lines, DebugConsoleLine, line)
	{
		if (console->area.size.y < y + console->line_height)
			break;

		{
			float opacity = GetMinF(LINE_TIME_FADE, GameTimer_GetRemaining(&line->timer)) / LINE_TIME_FADE;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(opacity * 255));
			DrawFormatStringFToHandle(GameObject_GetX(&console->area, LEFT), GameObject_GetY(&console->area, BOTTOM, -console->line_height - y), console->area.sprite.color, console->font, line->msg);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}

		y += console->line_height;
	} foreach_end;
}
