//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Array.cpp
//!
//! @brief  C言語版 動的配列クラス
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
#include "Array.h"

// 関数の定義 ==============================================================

// 拡張
void Array_ReserveRequired(Array* list, size_t min_capacity)
{
	// 容量が増えているか
	if (list->capacity < min_capacity)
	{
		// 元の配列の情報をコピー
		Array src = *list;
		// 新規容量計算
		list->capacity = min_capacity;
		// 元の配列の情報をコピー
		list->base = (Object*)malloc(list->capacity * sizeof(Object));
		// 初期化済みか確認
		if (src.base != NULL)
			// 元の配列から新たな配列へ内容をコピー
			memcpy(list->base, src.base, src.capacity);
		// 元の配列のメモリを開放
		free(src.base);
	}
}

// 自動拡張
void Array_Reserve(Array* list, size_t min_capacity)
{
	// 容量が増えているか
	if (list->capacity < min_capacity)
	{
		// 元の配列の容量を取得
		size_t old_capacity = list->capacity;
		// 新たな配列の容量を計算
		size_t new_capacity = max(old_capacity + (old_capacity >> 1), min_capacity);

		Array_ReserveRequired(list, new_capacity);
	}
}

// サイズを設定
void Array_SetSizeT(Array* list, size_t size)
{
	// 容量を確保
	Array_Reserve(list, size);
	// サイズを変更
	list->size = size;
}

// サイズを取得
size_t Array_GetSizeT(const Array* list)
{
	return list->size;
}

// サイズを設定
void Array_SetSize(Array* list, int size)
{
	Array_SetSizeT(list, (size_t)size);
}

// サイズを取得
int Array_GetSize(const Array* list)
{
	return (int)Array_GetSizeT(list);
}

// コンストラクタ
Array Array_Create(size_t new_capacity)
{
	// NULLで初期化
	Array obj = { NULL, 0, 0 };
	// サイズを設定
	Array_SetSize(&obj, new_capacity);
	return obj;
}

// デストラクタ
void Array_Delete(Array* list)
{
	// 配列のメモリを開放
	free(list->base);
	// NULLで初期化
	*list = { NULL, 0, 0 };
}

// 要素番号から取得
Object* Array_Get(const Array* list, int index)
{
	return &list->base[index];
}

// 要素番号の位置の要素を置き換え
void Array_Set(Array* list, int index, const Object* element)
{
	list->base[index] = *element;
}
