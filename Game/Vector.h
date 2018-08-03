//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Vector.h
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

// 多重インクルードの防止 ==================================================
#pragma once

// ヘッダファイルの読み込み ================================================
#include "GameObject.h"

// 定数の定義 ==============================================================

// デバッグ
#define VECTOR_DEBUG TRUE

// マクロの定義 ============================================================

// 拡張for文
#define foreach_start(vec, var) \
{ \
	VectorIterator itr_##var; \
	for (itr_##var = Vector_NextIterator(vec); VectorIterator_HasNext(&itr_##var);) \
	{ \
		int i_##var = VectorIterator_NextIndex(&itr_##var); \
		Object* var = VectorIterator_Next(&itr_##var); \
		{
#define foreach_end \
		} \
	} \
}

// 型の定義 ================================================================

// このリストが扱うことのできる型
typedef GameObject Object;

// 構造体の宣言 ============================================================

// リスト型
typedef struct {
	Object* first_capacity;
	Object* first;
	Object* last;
	Object* last_capacity;
} Vector;

// リスト反復子型
typedef struct {
	Vector* list;
	int current;
	int next;
#ifdef VECTOR_DEBUG
	BOOL current_exists;
	size_t current_size;
#endif
} VectorIterator;

// 関数の宣言 ==============================================================

// コンストラクタ
Vector Vector_Create(void);

// デストラクタ
void Vector_Delete(Vector* list);

// 終端を取得
Object* Vector_GetLast(const Vector* list);

// 先頭を取得
Object* Vector_GetFirst(const Vector* list);

// 要素番号から取得
Object* Vector_Get(const Vector* list, int index);

// 終端に追加
void Vector_AddLast(Vector* list, const Object* element);

// 先頭に追加
void Vector_AddFirst(Vector* list, const Object* element);

// 要素番号へ追加
void Vector_Add(Vector* list, int index, const Object* element);

// 要素番号の位置の要素を置き換え
void Vector_Set(Vector* list, int index, const Object* element);

// 終端の要素を削除
void Vector_RemoveLast(Vector* list);

// 先頭の要素を削除
void Vector_RemoveFirst(Vector* list);

// 要素番号の要素を削除
void Vector_Remove(Vector* list, int index);

// サイズを取得
size_t Vector_GetSizeT(const Vector* list);

// サイズを取得
int Vector_GetSize(const Vector* list);

// 容量を取得
size_t Vector_GetCapacityT(const Vector* list);

// 先頭の空き容量を取得
size_t Vector_RemainingFirstT(const Vector* list);

// 終端の空き容量を取得
size_t Vector_RemainingLastT(const Vector* list);

// 昇順の反復子を作成
VectorIterator Vector_NextIterator(Vector* list);

// 降順の反復子を作成
VectorIterator Vector_PrevIterator(Vector* list);

// 反復子を作成
VectorIterator VectorIterator_Create(Vector* list, int current, int next);

// 次の要素が存在するか
BOOL VectorIterator_HasNext(const VectorIterator* itr);

// 次へ進む
Object* VectorIterator_Next(VectorIterator* itr);

// 次の要素の要素番号
int VectorIterator_NextIndex(const VectorIterator* itr);

// 現在の要素の前に追加 (※一度のNextにつき、一度しか呼び出すことはできません)
void VectorIterator_Add(VectorIterator* itr, const Object* element);

// 現在の要素を置き換え
void VectorIterator_Set(VectorIterator* itr, const Object* element);

// 現在の要素を削除する (※一度のNextにつき、一度しか呼び出すことはできません)
void VectorIterator_Remove(VectorIterator* itr);
