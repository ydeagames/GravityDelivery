//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Vector.cpp
//!
//! @brief  C言語版 動的リストクラス
//!
//! @date   2018/07/26
//!
//! @author GF1 26 山口寛雅
//!
//!
//! Copyright (c) 2018 山口寛雅
//!
//! This file is released under the MIT License.
//! http://opensource.org/licenses/mit-license.php
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================
#include <assert.h>
#include "Vector.h"

// 定数の定義 ==============================================================

// 初期容量
#define DEFAULT_CAPACITY 8

// 関数の定義 ==============================================================

// 終端拡張
static void Vector_ReserveLastRequired(Vector* list, size_t min_capacity)
{
	// 新規容量計算
	size_t capacity = Vector_GetCapacityT(list) + min_capacity;
	// 元のリストの情報をコピー
	Vector src = *list;

	// 保存領域確保
	list->first_capacity = malloc(capacity * list->element_size);
	list->last_capacity = (char*)list->first_capacity + (capacity * list->element_size);

	// 初期化済みか確認
	if (src.first_capacity == NULL)
	{
		// 先頭にカーソルを移動
		list->first = list->first_capacity;
		list->last = list->first_capacity;
	}
	else
	{
		// 元のリストのサイズを取得
		size_t size = Vector_GetSize(&src);
		// 先頭は元のリストの先頭と同じ位置
		list->first = (char*)list->first_capacity + (Vector_RemainingFirstT(&src) * list->element_size);
		// 終端はこの先端からサイズ分後ろに移動
		list->last = (char*)list->first + (size * list->element_size);
		// 元のリストから新たなリストへ内容をコピー
		memcpy(Vector_GetFirst(list), Vector_GetFirst(&src), size * list->element_size);
		// 元のリストのメモリを開放
		free(src.first_capacity);
	}
}

// 終端自動拡張
static void Vector_ReserveLast(Vector* list, size_t min_capacity)
{
	// 終端に容量があるか確認
	if (Vector_RemainingLastT(list) <= 1)
	{
		// 元のリストの容量を取得
		size_t old_capacity = Vector_GetCapacityT(list);
		// 新たなリストの容量を計算
		size_t new_capacity = max(old_capacity + (old_capacity >> 1), max(DEFAULT_CAPACITY, min_capacity));

		// 拡張
		Vector_ReserveLastRequired(list, new_capacity);
	}
}

// 先頭拡張
static void Vector_ReserveFirstRequired(Vector* list, size_t min_capacity)
{
	// 新規容量計算
	size_t capacity = Vector_GetCapacityT(list) + min_capacity;
	// 元のリストの情報をコピー
	Vector src = *list;

	// 保存領域確保
	list->first_capacity = malloc(capacity * list->element_size);
	list->last_capacity = (char*)list->first_capacity + (capacity * list->element_size);

	// 初期化済みか確認
	if (src.first_capacity == NULL)
	{
		// 終端にカーソルを移動
		list->first = (char*)list->last_capacity - (1 * list->element_size);
		list->last = (char*)list->last_capacity - (1 * list->element_size);
	}
	else
	{
		// 元のリストのサイズを取得
		size_t size = Vector_GetSize(&src);
		// 終端は元のリストの終端と同じ位置
		list->last = (char*)list->last_capacity - (Vector_RemainingLastT(&src) * list->element_size);
		// 先頭はこの終端からサイズ分前に移動
		list->first = (char*)list->last - (size * list->element_size);
		// 元のリストから新たなリストへ内容をコピー
		memcpy(Vector_GetFirst(list), Vector_GetFirst(&src), size * list->element_size);
		// 元のリストのメモリを開放
		free(src.first_capacity);
	}
}

// 先頭自動拡張
static void Vector_ReserveFirst(Vector* list, size_t min_capacity)
{
	// 先頭に容量があるか確認
	if (Vector_RemainingFirstT(list) <= 1)
	{
		// 元のリストの容量を取得
		size_t old_capacity = Vector_GetCapacityT(list);
		// 新たなリストの容量を計算
		size_t new_capacity = max(old_capacity + (old_capacity >> 1), max(DEFAULT_CAPACITY, min_capacity));

		// 拡張
		Vector_ReserveFirstRequired(list, new_capacity);
	}
}

// コンストラクタ
Vector Vector_Create(size_t element_size)
{
	// NULLで初期化
	return{ element_size, NULL, NULL, NULL, NULL };
}

// デストラクタ
void Vector_Delete(Vector* list)
{
	// 初期化済みか確認
	if (list->first_capacity != NULL)
		// リストのメモリを開放
		free(list->first_capacity);
	// NULLで初期化
	*list = { NULL, NULL, NULL, NULL };
}

// 終端を取得
void* Vector_GetLast(const Vector* list)
{
	return list->last;
}

// 先頭を取得
void* Vector_GetFirst(const Vector* list)
{
	return (char*)list->first + (1 * list->element_size);
}

// 要素番号から取得
void* Vector_Get(const Vector* list, int index)
{
	return (char*)Vector_GetFirst(list) + (index * list->element_size);
}

// 終端に追加
void Vector_AddLast(Vector* list, const void* element)
{
	// 容量を確保
	Vector_ReserveLast(list, Vector_GetSize(list) + 1);
	// 終端に要素をコピー
	memcpy((char*)list->last + (1 * list->element_size), element, list->element_size);
	// 終端カーソルを移動
	list->last = (char*)list->last + (1 * list->element_size);
}

// 先頭に追加
void Vector_AddFirst(Vector* list, const void* element)
{
	// 容量を確保
	Vector_ReserveFirst(list, Vector_GetSize(list) + 1);
	// 先頭に要素をコピー
	memcpy((char*)list->first + (0 * list->element_size), element, list->element_size);
	// 先頭カーソルを移動
	list->first = (char*)list->first + (-1 * list->element_size);
}

// 要素番号へ追加
void Vector_Add(Vector* list, int index, const void* element)
{
	// サイズ取得
	int size = Vector_GetSize(list);

	// 範囲外にアクセスされたらエラー
	assert((0 <= index && index <= size) && "IndexOutOfBoundsException");

	// 先頭、終端のどちらが近いか
	if (index < size / 2)
	{
		// 容量を確保
		Vector_ReserveFirst(list, size + 1);
		// 先頭から要素番号までの要素をずらす
		memmove((char*)Vector_GetFirst(list) + (-1 * list->element_size),
			(char*)Vector_GetFirst(list) + (0 * list->element_size),
			(index * list->element_size));
		// 先頭カーソルを移動
		list->first = (char*)list->first + (-1 * list->element_size);
		// 先頭からの要素番号に要素をコピー
		memcpy((char*)Vector_GetFirst(list) + (index * list->element_size), element, list->element_size);
	}
	else
	{
		// 後ろからのインデックスを計算
		index = size - index;
		// 容量を確保
		Vector_ReserveLast(list, size + 1);
		// 終端から要素番号までの要素をずらす
		memmove((char*)Vector_GetLast(list) + ((-index + 2) * list->element_size),
			(char*)Vector_GetLast(list) + ((-index + 1) * list->element_size),
			(index * list->element_size));
		// 終端カーソルを移動
		list->last = (char*)list->last + (1 * list->element_size);
		// 終端からの要素番号に要素をコピー
		memcpy((char*)Vector_GetLast(list) + (-index * list->element_size), element, list->element_size);
	}
}

// 要素番号の位置の要素を置き換え
void Vector_Set(Vector* list, int index, const void* element)
{
	memcpy((char*)Vector_GetFirst(list) + (index * list->element_size), element, list->element_size);
}

// 終端の要素を削除
void Vector_RemoveLast(Vector* list)
{
	// 終端カーソルを移動
	list->last = (char*)list->last + (-1 * list->element_size);
}

// 先頭の要素を削除
void Vector_RemoveFirst(Vector* list)
{
	// 先頭カーソルを移動
	list->first = (char*)list->first + (1 * list->element_size);
}

// 要素番号の要素を削除
void Vector_Remove(Vector* list, int index)
{
	// サイズ取得
	int size = Vector_GetSize(list);

	// 先頭、終端のどちらが近いか
	if (index < size / 2)
	{
		// 先頭から要素番号までの要素をずらす
		memmove((char*)Vector_GetFirst(list) + (1 * list->element_size),
			(char*)Vector_GetFirst(list) + (0 * list->element_size),
			(index * list->element_size));
		// 先頭カーソルを移動
		list->first = (char*)list->first + (1 * list->element_size);
	}
	else
	{
		// 後ろからのインデックスを計算
		index = size - index;
		// 終端から要素番号までの要素をずらす
		memmove((char*)Vector_GetLast(list) + ((1 - index) * list->element_size),
			(char*)Vector_GetLast(list) + ((2 - index) * list->element_size),
			(index * list->element_size));
		// 終端カーソルを移動
		list->last = (char*)list->last + (-1 * list->element_size);
	}
}

// すべての要素を削除
void Vector_Clear(Vector* list)
{
	while (Vector_GetSizeT(list) > 0)
	{
		Vector_RemoveLast(list);
	}
}

// サイズを取得
size_t Vector_GetSizeT(const Vector* list)
{
	// 未代入の場合のサイズは0
	if (list->first_capacity == NULL)
		return 0;
	return ((char*)list->last - (char*)list->first) / list->element_size;
}

// サイズを取得
int Vector_GetSize(const Vector* list)
{
	return (int)Vector_GetSizeT(list);
}

// 容量を取得
size_t Vector_GetCapacityT(const Vector* list)
{
	// 未代入の場合のサイズは0
	if (list->first_capacity == NULL)
		return 0;
	return ((char*)list->last_capacity - (char*)list->first_capacity) / list->element_size;
}

// 先頭の空き容量を取得
size_t Vector_RemainingFirstT(const Vector* list)
{
	if (list->first_capacity == NULL)
		return 0;
	return ((char*)list->first - (char*)list->first_capacity) / list->element_size;
}

// 終端の空き容量を取得
size_t Vector_RemainingLastT(const Vector* list)
{
	if (list->first_capacity == NULL)
		return 0;
	return ((char*)list->last_capacity - (char*)list->last) / list->element_size;
}

// 昇順の反復子を作成
VectorIterator Vector_NextIterator(Vector* list)
{
	return VectorIterator_Create(list, 0, 1);
}

// 降順の反復子を作成
VectorIterator Vector_PrevIterator(Vector* list)
{
	return VectorIterator_Create(list, Vector_GetSize(list) - 1, -1);
}

// 反復子を作成
VectorIterator VectorIterator_Create(Vector* list, int current, int next)
{
	return{
		list, current, next
#ifdef VECTOR_DEBUG
		, FALSE, Vector_GetSizeT(list)
#endif
	};
}

// 次の要素が存在するか
BOOL VectorIterator_HasNext(const VectorIterator* itr)
{
	if (itr->next > 0)
		return itr->current < Vector_GetSize(itr->list);
	else
		return 0 <= itr->current;
}

// 次へ進む
void* VectorIterator_Next(VectorIterator* itr)
{
	void* obj = Vector_Get(itr->list, itr->current);
	itr->current += itr->next;
#ifdef VECTOR_DEBUG
	itr->current_exists = TRUE;
#endif
	return obj;
}

// 次の要素の要素番号
int VectorIterator_NextIndex(const VectorIterator* itr)
{
	return itr->current;
}

// 現在の要素の前に追加 (※一度のNextにつき、一度しか呼び出すことはできません)
void VectorIterator_Add(VectorIterator* itr, const void* element)
{
#ifdef VECTOR_DEBUG
	assert(itr->current_size == Vector_GetSizeT(itr->list) && "Not a fresh iterator");
	assert(itr->current_exists && "IllegalStateException: Not a fresh iterator");
#endif
	Vector_Add(itr->list, itr->current - itr->next, element);
	itr->current += itr->next;
#ifdef VECTOR_DEBUG
	itr->current_size = Vector_GetSizeT(itr->list);
	itr->current_exists = FALSE;
#endif
}

// 現在の要素を置き換え
void VectorIterator_Set(VectorIterator* itr, const void* element)
{
	Vector_Set(itr->list, itr->current - itr->next, element);
}

// 現在の要素を削除する (※一度のNextにつき、一度しか呼び出すことはできません)
void VectorIterator_Remove(VectorIterator* itr)
{
#ifdef VECTOR_DEBUG
	assert(itr->current_size == Vector_GetSize(itr->list) && "ConcurrentModificationException");
	assert(itr->current_exists && "IllegalStateException: Not a fresh iterator");
#endif
	Vector_Remove(itr->list, itr->current - itr->next);
	itr->current -= itr->next;
#ifdef VECTOR_DEBUG
	itr->current_size = Vector_GetSizeT(itr->list);
	itr->current_exists = FALSE;
#endif
}
