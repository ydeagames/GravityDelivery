#pragma once
#include "GameObject.h"

// �֐��̐錾 ==============================================================

// <�I�u�W�F�N�g�_�����蔻��>
BOOL GameObject_IsHitPoint(const GameObject* obj, const Vec2* p);

// <�I�u�W�F�N�g�����蔻��>
BOOL GameObject_IsHit(const GameObject* obj1, const GameObject* obj2);
