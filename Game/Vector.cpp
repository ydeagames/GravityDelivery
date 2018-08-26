//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Vector.cpp
//!
//! @brief  C����� ���I���X�g�N���X
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
#include <assert.h>
#include "Vector.h"

// �萔�̒�` ==============================================================

// �����e��
#define DEFAULT_CAPACITY 8

// �֐��̒�` ==============================================================

// �I�[�g��
static void Vector_ReserveLastRequired(Vector* list, size_t min_capacity)
{
	// �V�K�e�ʌv�Z
	size_t capacity = Vector_GetCapacityT(list) + min_capacity;
	// ���̃��X�g�̏����R�s�[
	Vector src = *list;

	// �ۑ��̈�m��
	list->first_capacity = malloc(capacity * list->element_size);
	list->last_capacity = (char*)list->first_capacity + (capacity * list->element_size);

	// �������ς݂��m�F
	if (src.first_capacity == NULL)
	{
		// �擪�ɃJ�[�\�����ړ�
		list->first = list->first_capacity;
		list->last = list->first_capacity;
	}
	else
	{
		// ���̃��X�g�̃T�C�Y���擾
		size_t size = Vector_GetSize(&src);
		// �擪�͌��̃��X�g�̐擪�Ɠ����ʒu
		list->first = (char*)list->first_capacity + (Vector_RemainingFirstT(&src) * list->element_size);
		// �I�[�͂��̐�[����T�C�Y�����Ɉړ�
		list->last = (char*)list->first + (size * list->element_size);
		// ���̃��X�g����V���ȃ��X�g�֓��e���R�s�[
		memcpy(Vector_GetFirst(list), Vector_GetFirst(&src), size * list->element_size);
		// ���̃��X�g�̃��������J��
		free(src.first_capacity);
	}
}

// �I�[�����g��
static void Vector_ReserveLast(Vector* list, size_t min_capacity)
{
	// �I�[�ɗe�ʂ����邩�m�F
	if (Vector_RemainingLastT(list) <= 1)
	{
		// ���̃��X�g�̗e�ʂ��擾
		size_t old_capacity = Vector_GetCapacityT(list);
		// �V���ȃ��X�g�̗e�ʂ��v�Z
		size_t new_capacity = max(old_capacity + (old_capacity >> 1), max(DEFAULT_CAPACITY, min_capacity));

		// �g��
		Vector_ReserveLastRequired(list, new_capacity);
	}
}

// �擪�g��
static void Vector_ReserveFirstRequired(Vector* list, size_t min_capacity)
{
	// �V�K�e�ʌv�Z
	size_t capacity = Vector_GetCapacityT(list) + min_capacity;
	// ���̃��X�g�̏����R�s�[
	Vector src = *list;

	// �ۑ��̈�m��
	list->first_capacity = malloc(capacity * list->element_size);
	list->last_capacity = (char*)list->first_capacity + (capacity * list->element_size);

	// �������ς݂��m�F
	if (src.first_capacity == NULL)
	{
		// �I�[�ɃJ�[�\�����ړ�
		list->first = (char*)list->last_capacity - (1 * list->element_size);
		list->last = (char*)list->last_capacity - (1 * list->element_size);
	}
	else
	{
		// ���̃��X�g�̃T�C�Y���擾
		size_t size = Vector_GetSize(&src);
		// �I�[�͌��̃��X�g�̏I�[�Ɠ����ʒu
		list->last = (char*)list->last_capacity - (Vector_RemainingLastT(&src) * list->element_size);
		// �擪�͂��̏I�[����T�C�Y���O�Ɉړ�
		list->first = (char*)list->last - (size * list->element_size);
		// ���̃��X�g����V���ȃ��X�g�֓��e���R�s�[
		memcpy(Vector_GetFirst(list), Vector_GetFirst(&src), size * list->element_size);
		// ���̃��X�g�̃��������J��
		free(src.first_capacity);
	}
}

// �擪�����g��
static void Vector_ReserveFirst(Vector* list, size_t min_capacity)
{
	// �擪�ɗe�ʂ����邩�m�F
	if (Vector_RemainingFirstT(list) <= 1)
	{
		// ���̃��X�g�̗e�ʂ��擾
		size_t old_capacity = Vector_GetCapacityT(list);
		// �V���ȃ��X�g�̗e�ʂ��v�Z
		size_t new_capacity = max(old_capacity + (old_capacity >> 1), max(DEFAULT_CAPACITY, min_capacity));

		// �g��
		Vector_ReserveFirstRequired(list, new_capacity);
	}
}

// �R���X�g���N�^
Vector Vector_Create(size_t element_size)
{
	// NULL�ŏ�����
	return{ element_size, NULL, NULL, NULL, NULL };
}

// �f�X�g���N�^
void Vector_Delete(Vector* list)
{
	// �������ς݂��m�F
	if (list->first_capacity != NULL)
		// ���X�g�̃��������J��
		free(list->first_capacity);
	// NULL�ŏ�����
	*list = { NULL, NULL, NULL, NULL };
}

// �I�[���擾
void* Vector_GetLast(const Vector* list)
{
	return list->last;
}

// �擪���擾
void* Vector_GetFirst(const Vector* list)
{
	return (char*)list->first + (1 * list->element_size);
}

// �v�f�ԍ�����擾
void* Vector_Get(const Vector* list, int index)
{
	return (char*)Vector_GetFirst(list) + (index * list->element_size);
}

// �I�[�ɒǉ�
void Vector_AddLast(Vector* list, const void* element)
{
	// �e�ʂ��m��
	Vector_ReserveLast(list, Vector_GetSize(list) + 1);
	// �I�[�ɗv�f���R�s�[
	memcpy((char*)list->last + (1 * list->element_size), element, list->element_size);
	// �I�[�J�[�\�����ړ�
	list->last = (char*)list->last + (1 * list->element_size);
}

// �擪�ɒǉ�
void Vector_AddFirst(Vector* list, const void* element)
{
	// �e�ʂ��m��
	Vector_ReserveFirst(list, Vector_GetSize(list) + 1);
	// �擪�ɗv�f���R�s�[
	memcpy((char*)list->first + (0 * list->element_size), element, list->element_size);
	// �擪�J�[�\�����ړ�
	list->first = (char*)list->first + (-1 * list->element_size);
}

// �v�f�ԍ��֒ǉ�
void Vector_Add(Vector* list, int index, const void* element)
{
	// �T�C�Y�擾
	int size = Vector_GetSize(list);

	// �͈͊O�ɃA�N�Z�X���ꂽ��G���[
	assert((0 <= index && index <= size) && "IndexOutOfBoundsException");

	// �擪�A�I�[�̂ǂ��炪�߂���
	if (index < size / 2)
	{
		// �e�ʂ��m��
		Vector_ReserveFirst(list, size + 1);
		// �擪����v�f�ԍ��܂ł̗v�f�����炷
		memmove((char*)Vector_GetFirst(list) + (-1 * list->element_size),
			(char*)Vector_GetFirst(list) + (0 * list->element_size),
			(index * list->element_size));
		// �擪�J�[�\�����ړ�
		list->first = (char*)list->first + (-1 * list->element_size);
		// �擪����̗v�f�ԍ��ɗv�f���R�s�[
		memcpy((char*)Vector_GetFirst(list) + (index * list->element_size), element, list->element_size);
	}
	else
	{
		// ��납��̃C���f�b�N�X���v�Z
		index = size - index;
		// �e�ʂ��m��
		Vector_ReserveLast(list, size + 1);
		// �I�[����v�f�ԍ��܂ł̗v�f�����炷
		memmove((char*)Vector_GetLast(list) + ((-index + 2) * list->element_size),
			(char*)Vector_GetLast(list) + ((-index + 1) * list->element_size),
			(index * list->element_size));
		// �I�[�J�[�\�����ړ�
		list->last = (char*)list->last + (1 * list->element_size);
		// �I�[����̗v�f�ԍ��ɗv�f���R�s�[
		memcpy((char*)Vector_GetLast(list) + (-index * list->element_size), element, list->element_size);
	}
}

// �v�f�ԍ��̈ʒu�̗v�f��u������
void Vector_Set(Vector* list, int index, const void* element)
{
	memcpy((char*)Vector_GetFirst(list) + (index * list->element_size), element, list->element_size);
}

// �I�[�̗v�f���폜
void Vector_RemoveLast(Vector* list)
{
	// �I�[�J�[�\�����ړ�
	list->last = (char*)list->last + (-1 * list->element_size);
}

// �擪�̗v�f���폜
void Vector_RemoveFirst(Vector* list)
{
	// �擪�J�[�\�����ړ�
	list->first = (char*)list->first + (1 * list->element_size);
}

// �v�f�ԍ��̗v�f���폜
void Vector_Remove(Vector* list, int index)
{
	// �T�C�Y�擾
	int size = Vector_GetSize(list);

	// �擪�A�I�[�̂ǂ��炪�߂���
	if (index < size / 2)
	{
		// �擪����v�f�ԍ��܂ł̗v�f�����炷
		memmove((char*)Vector_GetFirst(list) + (1 * list->element_size),
			(char*)Vector_GetFirst(list) + (0 * list->element_size),
			(index * list->element_size));
		// �擪�J�[�\�����ړ�
		list->first = (char*)list->first + (1 * list->element_size);
	}
	else
	{
		// ��납��̃C���f�b�N�X���v�Z
		index = size - index;
		// �I�[����v�f�ԍ��܂ł̗v�f�����炷
		memmove((char*)Vector_GetLast(list) + ((1 - index) * list->element_size),
			(char*)Vector_GetLast(list) + ((2 - index) * list->element_size),
			(index * list->element_size));
		// �I�[�J�[�\�����ړ�
		list->last = (char*)list->last + (-1 * list->element_size);
	}
}

// ���ׂĂ̗v�f���폜
void Vector_Clear(Vector* list)
{
	while (Vector_GetSizeT(list) > 0)
	{
		Vector_RemoveLast(list);
	}
}

// �T�C�Y���擾
size_t Vector_GetSizeT(const Vector* list)
{
	// ������̏ꍇ�̃T�C�Y��0
	if (list->first_capacity == NULL)
		return 0;
	return ((char*)list->last - (char*)list->first) / list->element_size;
}

// �T�C�Y���擾
int Vector_GetSize(const Vector* list)
{
	return (int)Vector_GetSizeT(list);
}

// �e�ʂ��擾
size_t Vector_GetCapacityT(const Vector* list)
{
	// ������̏ꍇ�̃T�C�Y��0
	if (list->first_capacity == NULL)
		return 0;
	return ((char*)list->last_capacity - (char*)list->first_capacity) / list->element_size;
}

// �擪�̋󂫗e�ʂ��擾
size_t Vector_RemainingFirstT(const Vector* list)
{
	if (list->first_capacity == NULL)
		return 0;
	return ((char*)list->first - (char*)list->first_capacity) / list->element_size;
}

// �I�[�̋󂫗e�ʂ��擾
size_t Vector_RemainingLastT(const Vector* list)
{
	if (list->first_capacity == NULL)
		return 0;
	return ((char*)list->last_capacity - (char*)list->last) / list->element_size;
}

// �����̔����q���쐬
VectorIterator Vector_NextIterator(Vector* list)
{
	return VectorIterator_Create(list, 0, 1);
}

// �~���̔����q���쐬
VectorIterator Vector_PrevIterator(Vector* list)
{
	return VectorIterator_Create(list, Vector_GetSize(list) - 1, -1);
}

// �����q���쐬
VectorIterator VectorIterator_Create(Vector* list, int current, int next)
{
	return{
		list, current, next
#ifdef VECTOR_DEBUG
		, FALSE, Vector_GetSizeT(list)
#endif
	};
}

// ���̗v�f�����݂��邩
BOOL VectorIterator_HasNext(const VectorIterator* itr)
{
	if (itr->next > 0)
		return itr->current < Vector_GetSize(itr->list);
	else
		return 0 <= itr->current;
}

// ���֐i��
void* VectorIterator_Next(VectorIterator* itr)
{
	void* obj = Vector_Get(itr->list, itr->current);
	itr->current += itr->next;
#ifdef VECTOR_DEBUG
	itr->current_exists = TRUE;
#endif
	return obj;
}

// ���̗v�f�̗v�f�ԍ�
int VectorIterator_NextIndex(const VectorIterator* itr)
{
	return itr->current;
}

// ���݂̗v�f�̑O�ɒǉ� (����x��Next�ɂ��A��x�����Ăяo�����Ƃ͂ł��܂���)
void VectorIterator_Add(VectorIterator* itr, const void* element)
{
#ifdef VECTOR_DEBUG
	assert(itr->current_size == Vector_GetSizeT(itr->list) && "Not a fresh iterator");
	assert(itr->current_exists && "IllegalStateException: Not a fresh iterator");
#endif
	Vector_Add(itr->list, itr->current++, element);
#ifdef VECTOR_DEBUG
	itr->current_size = Vector_GetSizeT(itr->list);
	itr->current_exists = FALSE;
#endif
}

// ���݂̗v�f��u������
void VectorIterator_Set(VectorIterator* itr, const void* element)
{
	Vector_Set(itr->list, itr->current, element);
}

// ���݂̗v�f���폜���� (����x��Next�ɂ��A��x�����Ăяo�����Ƃ͂ł��܂���)
void VectorIterator_Remove(VectorIterator* itr)
{
#ifdef VECTOR_DEBUG
	assert(itr->current_size == Vector_GetSize(itr->list) && "ConcurrentModificationException");
	assert(itr->current_exists && "IllegalStateException: Not a fresh iterator");
#endif
	Vector_Remove(itr->list, itr->current -= itr->next);
#ifdef VECTOR_DEBUG
	itr->current_size = Vector_GetSizeT(itr->list);
	itr->current_exists = FALSE;
#endif
}
