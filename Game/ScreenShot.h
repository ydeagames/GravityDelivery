#pragma once
#include "GameObject.h"

// <�X�N���[���V���b�g>
BOOL SaveScreenShotToPNG(const GameObject* area, char* name = NULL);

// <�X�N���[���V���b�g���O>
void SaveScreenShotToPNG_Log(const GameObject* area);

// <�X�N���[���V���b�g�t�H���_���J��>
void OpenScreenShotDir(void);