//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Vector.h
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

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once

// �萔�̒�` ==============================================================

// �f�o�b�O
#define VECTOR_DEBUG TRUE

// �}�N���̒�` ============================================================

// �g��for��
#define foreach_start(vec, type, var) \
{ \
	VectorIterator itr_##var; \
	BOOL break_##var = FALSE; \
	for (itr_##var = Vector_NextIterator(vec); !break_##var && VectorIterator_HasNext(&itr_##var);) \
	{ \
		int i_##var = VectorIterator_NextIndex(&itr_##var); \
		type* var = (type*)VectorIterator_Next(&itr_##var); \
		{
#define foreach_end \
		} \
	} \
}

// �^�̒�` ================================================================

// �\���̂̐錾 ============================================================

// ���X�g�^
typedef struct {
	size_t element_size;
	void* first_capacity;
	void* first;
	void* last;
	void* last_capacity;
} Vector;

// ���X�g�����q�^
typedef struct {
	Vector* list;
	int current;
	int next;
#ifdef VECTOR_DEBUG
	BOOL current_exists;
	size_t current_size;
#endif
} VectorIterator;

// �֐��̐錾 ==============================================================

// �R���X�g���N�^
Vector Vector_Create(size_t element_size);

// �f�X�g���N�^
void Vector_Delete(Vector* list);

// �I�[���擾
void* Vector_GetLast(const Vector* list);

// �擪���擾
void* Vector_GetFirst(const Vector* list);

// �v�f�ԍ�����擾
void* Vector_Get(const Vector* list, int index);

// �I�[�ɒǉ�
void Vector_AddLast(Vector* list, const void* element);

// �擪�ɒǉ�
void Vector_AddFirst(Vector* list, const void* element);

// �v�f�ԍ��֒ǉ�
void Vector_Add(Vector* list, int index, const void* element);

// �v�f�ԍ��̈ʒu�̗v�f��u������
void Vector_Set(Vector* list, int index, const void* element);

// �I�[�̗v�f���폜
void Vector_RemoveLast(Vector* list);

// �擪�̗v�f���폜
void Vector_RemoveFirst(Vector* list);

// �v�f�ԍ��̗v�f���폜
void Vector_Remove(Vector* list, int index);

// ���ׂĂ̗v�f���폜
void Vector_Clear(Vector* list);

// �T�C�Y���擾
size_t Vector_GetSizeT(const Vector* list);

// �T�C�Y���擾
int Vector_GetSize(const Vector* list);

// �e�ʂ��擾
size_t Vector_GetCapacityT(const Vector* list);

// �擪�̋󂫗e�ʂ��擾
size_t Vector_RemainingFirstT(const Vector* list);

// �I�[�̋󂫗e�ʂ��擾
size_t Vector_RemainingLastT(const Vector* list);

// �����̔����q���쐬
VectorIterator Vector_NextIterator(Vector* list);

// �~���̔����q���쐬
VectorIterator Vector_PrevIterator(Vector* list);

// �����q���쐬
VectorIterator VectorIterator_Create(Vector* list, int current, int next);

// ���̗v�f�����݂��邩
BOOL VectorIterator_HasNext(const VectorIterator* itr);

// ���֐i��
void* VectorIterator_Next(VectorIterator* itr);

// ���̗v�f�̗v�f�ԍ�
int VectorIterator_NextIndex(const VectorIterator* itr);

// ���݂̗v�f�̑O�ɒǉ� (����x��Next�ɂ��A��x�����Ăяo�����Ƃ͂ł��܂���)
void VectorIterator_Add(VectorIterator* itr, const void* element);

// ���݂̗v�f��u������
void VectorIterator_Set(VectorIterator* itr, const void* element);

// ���݂̗v�f���폜���� (����x��Next�ɂ��A��x�����Ăяo�����Ƃ͂ł��܂���)
void VectorIterator_Remove(VectorIterator* itr);
