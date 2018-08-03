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
	list->first_capacity = (Object*)malloc(capacity * sizeof(Object));
	list->last_capacity = list->first_capacity + capacity;

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
		list->first = list->first_capacity + Vector_RemainingFirstT(&src);
		// �I�[�͂��̐�[����T�C�Y�����Ɉړ�
		list->last = list->first + size;
		// ���̃��X�g����V���ȃ��X�g�֓��e���R�s�[
		memcpy(Vector_GetFirst(list), Vector_GetFirst(&src), size * sizeof(Object));
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
	list->first_capacity = (Object*)malloc(capacity * sizeof(Object));
	list->last_capacity = list->first_capacity + capacity;

	// �������ς݂��m�F
	if (src.first_capacity == NULL)
	{
		// �I�[�ɃJ�[�\�����ړ�
		list->first = list->last_capacity - 1;
		list->last = list->last_capacity - 1;
	}
	else
	{
		// ���̃��X�g�̃T�C�Y���擾
		size_t size = Vector_GetSize(&src);
		// �I�[�͌��̃��X�g�̏I�[�Ɠ����ʒu
		list->last = list->last_capacity - Vector_RemainingLastT(&src);
		// �擪�͂��̏I�[����T�C�Y���O�Ɉړ�
		list->first = list->last - size;
		// ���̃��X�g����V���ȃ��X�g�֓��e���R�s�[
		memcpy(Vector_GetFirst(list), Vector_GetFirst(&src), size * sizeof(Object));
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
Vector Vector_Create(void)
{
	// NULL�ŏ�����
	return{ NULL, NULL, NULL, NULL };
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
Object* Vector_GetLast(const Vector* list)
{
	return list->last;
}

// �擪���擾
Object* Vector_GetFirst(const Vector* list)
{
	return list->first + 1;
}

// �v�f�ԍ�����擾
Object* Vector_Get(const Vector* list, int index)
{
	return &Vector_GetFirst(list)[index];
}

// �I�[�ɒǉ�
void Vector_AddLast(Vector* list, const Object* element)
{
	// �e�ʂ��m��
	Vector_ReserveLast(list, Vector_GetSize(list) + 1);
	// �I�[�ɗv�f���R�s�[
	list->last[1] = *element;
	// �I�[�J�[�\�����ړ�
	list->last++;
}

// �擪�ɒǉ�
void Vector_AddFirst(Vector* list, const Object* element)
{
	// �e�ʂ��m��
	Vector_ReserveFirst(list, Vector_GetSize(list) + 1);
	// �擪�ɗv�f���R�s�[
	list->first[0] = *element;
	// �擪�J�[�\�����ړ�
	list->first--;
}

// �v�f�ԍ��֒ǉ�
void Vector_Add(Vector* list, int index, const Object* element)
{
	int i;
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
		for (i = 0; i < index; i++)
			Vector_GetFirst(list)[i - 1] = Vector_GetFirst(list)[i];
		// �擪�J�[�\�����ړ�
		list->first--;
		// �擪����̗v�f�ԍ��ɗv�f���R�s�[
		Vector_GetFirst(list)[index] = *element;
	}
	else
	{
		// ��납��̃C���f�b�N�X���v�Z
		index = size - index;
		// �e�ʂ��m��
		Vector_ReserveLast(list, size + 1);
		// �I�[����v�f�ԍ��܂ł̗v�f�����炷
		for (i = 0; i < index; i++)
			Vector_GetLast(list)[-(i - 1)] = Vector_GetLast(list)[-i];
		// �I�[�J�[�\�����ړ�
		list->last++;
		// �I�[����̗v�f�ԍ��ɗv�f���R�s�[
		Vector_GetLast(list)[-index] = *element;
	}
}

// �v�f�ԍ��̈ʒu�̗v�f��u������
void Vector_Set(Vector* list, int index, const Object* element)
{
	Vector_GetFirst(list)[index] = *element;
}

// �I�[�̗v�f���폜
void Vector_RemoveLast(Vector* list)
{
	// �I�[�J�[�\�����ړ�
	list->last--;
}

// �擪�̗v�f���폜
void Vector_RemoveFirst(Vector* list)
{
	// �擪�J�[�\�����ړ�
	list->first++;
}

// �v�f�ԍ��̗v�f���폜
void Vector_Remove(Vector* list, int index)
{
	int i;
	// �T�C�Y�擾
	int size = Vector_GetSize(list);

	// �擪�A�I�[�̂ǂ��炪�߂���
	if (index < size / 2)
	{
		// �擪����v�f�ԍ��܂ł̗v�f�����炷
		for (i = index; i > 0; i--)
			Vector_GetFirst(list)[i] = Vector_GetFirst(list)[i - 1];
		// �擪�J�[�\�����ړ�
		list->first++;
	}
	else
	{
		// ��납��̃C���f�b�N�X���v�Z
		index = size - index;
		// �I�[����v�f�ԍ��܂ł̗v�f�����炷
		for (i = index; i > 0; i--)
			Vector_GetLast(list)[1 - i] = Vector_GetLast(list)[1 - (i - 1)];
		// �I�[�J�[�\�����ړ�
		list->last--;
	}
}

// �T�C�Y���擾
size_t Vector_GetSizeT(const Vector* list)
{
	// ������̏ꍇ�̃T�C�Y��0
	if (list->first_capacity == NULL)
		return 0;
	return list->last - list->first;
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
	return list->last_capacity - list->first_capacity;
}

// �擪�̋󂫗e�ʂ��擾
size_t Vector_RemainingFirstT(const Vector* list)
{
	if (list->first_capacity == NULL)
		return 0;
	return list->first - list->first_capacity;
}

// �I�[�̋󂫗e�ʂ��擾
size_t Vector_RemainingLastT(const Vector* list)
{
	if (list->first_capacity == NULL)
		return 0;
	return list->last_capacity - list->last;
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
Object* VectorIterator_Next(VectorIterator* itr)
{
	Object* obj = Vector_Get(itr->list, itr->current);
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
void VectorIterator_Add(VectorIterator* itr, const Object* element)
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
void VectorIterator_Set(VectorIterator* itr, const Object* element)
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
