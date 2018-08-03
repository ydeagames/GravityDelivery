//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Array.cpp
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

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "Array.h"

// �֐��̒�` ==============================================================

// �g��
void Array_ReserveRequired(Array* list, size_t min_capacity)
{
	// �e�ʂ������Ă��邩
	if (list->capacity < min_capacity)
	{
		// ���̔z��̏����R�s�[
		Array src = *list;
		// �V�K�e�ʌv�Z
		list->capacity = min_capacity;
		// ���̔z��̏����R�s�[
		list->base = (Object*)malloc(list->capacity * sizeof(Object));
		// �������ς݂��m�F
		if (src.base != NULL)
			// ���̔z�񂩂�V���Ȕz��֓��e���R�s�[
			memcpy(list->base, src.base, src.capacity);
		// ���̔z��̃��������J��
		free(src.base);
	}
}

// �����g��
void Array_Reserve(Array* list, size_t min_capacity)
{
	// �e�ʂ������Ă��邩
	if (list->capacity < min_capacity)
	{
		// ���̔z��̗e�ʂ��擾
		size_t old_capacity = list->capacity;
		// �V���Ȕz��̗e�ʂ��v�Z
		size_t new_capacity = max(old_capacity + (old_capacity >> 1), min_capacity);

		Array_ReserveRequired(list, new_capacity);
	}
}

// �T�C�Y��ݒ�
void Array_SetSizeT(Array* list, size_t size)
{
	// �e�ʂ��m��
	Array_Reserve(list, size);
	// �T�C�Y��ύX
	list->size = size;
}

// �T�C�Y���擾
size_t Array_GetSizeT(const Array* list)
{
	return list->size;
}

// �T�C�Y��ݒ�
void Array_SetSize(Array* list, int size)
{
	Array_SetSizeT(list, (size_t)size);
}

// �T�C�Y���擾
int Array_GetSize(const Array* list)
{
	return (int)Array_GetSizeT(list);
}

// �R���X�g���N�^
Array Array_Create(size_t new_capacity)
{
	// NULL�ŏ�����
	Array obj = { NULL, 0, 0 };
	// �T�C�Y��ݒ�
	Array_SetSize(&obj, new_capacity);
	return obj;
}

// �f�X�g���N�^
void Array_Delete(Array* list)
{
	// �z��̃��������J��
	free(list->base);
	// NULL�ŏ�����
	*list = { NULL, 0, 0 };
}

// �v�f�ԍ�����擾
Object* Array_Get(const Array* list, int index)
{
	return &list->base[index];
}

// �v�f�ԍ��̈ʒu�̗v�f��u������
void Array_Set(Array* list, int index, const Object* element)
{
	list->base[index] = *element;
}
