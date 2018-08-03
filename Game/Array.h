//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Array.h
//!
//! @brief  C����� ���I�z��N���X
//!
//! @date   2018/07/26
//!
//! @author GF1 26 �R������
//!
//!
//! Copyright (c) 2018 �R������
//!
//! This file is released under the MIT License.
//! http://opensource.org/licenses/mit-license.php
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "GameObject.h"

// �^�̒�` ================================================================

// ���̃��X�g���������Ƃ̂ł���^
typedef GameObject Object;

// �\���̂̐錾 ============================================================

// �z��^
typedef struct {
	Object* base;
	size_t capacity;
	size_t size;
} Array;

// �֐��̐錾 ==============================================================

// <<�z��>> ------------------------------------------------------------

// �g��
void Array_ReserveRequired(Array* list, size_t min_capacity);

// �����g��
void Array_Reserve(Array* list, size_t min_capacity);

// �T�C�Y��ݒ�
void Array_SetSizeT(Array* list, size_t size);

// �T�C�Y���擾
size_t Array_GetSizeT(const Array* list);

// �T�C�Y��ݒ�
void Array_SetSize(Array* list, int size);

// �T�C�Y���擾
int Array_GetSize(const Array* list);

// �R���X�g���N�^
Array Array_Create(size_t new_capacity);

// �f�X�g���N�^
void Array_Delete(Array* list);

// �v�f�ԍ�����擾
Object* Array_Get(const Array* list, int index);

// �v�f�ԍ��̈ʒu�̗v�f��u������
void Array_Set(Array* list, int index, const Object* element);
