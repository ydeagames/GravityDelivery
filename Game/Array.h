//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Array.h
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

// 多重インクルードの防止 ==================================================
#pragma once

// ヘッダファイルの読み込み ================================================
#include "GameObject.h"

// 型の定義 ================================================================

// このリストが扱うことのできる型
typedef GameObject Object;

// 構造体の宣言 ============================================================

// 配列型
typedef struct {
	Object* base;
	size_t capacity;
	size_t size;
} Array;

// 関数の宣言 ==============================================================

// <<配列>> ------------------------------------------------------------

// 拡張
void Array_ReserveRequired(Array* list, size_t min_capacity);

// 自動拡張
void Array_Reserve(Array* list, size_t min_capacity);

// サイズを設定
void Array_SetSizeT(Array* list, size_t size);

// サイズを取得
size_t Array_GetSizeT(const Array* list);

// サイズを設定
void Array_SetSize(Array* list, int size);

// サイズを取得
int Array_GetSize(const Array* list);

// コンストラクタ
Array Array_Create(size_t new_capacity);

// デストラクタ
void Array_Delete(Array* list);

// 要素番号から取得
Object* Array_Get(const Array* list, int index);

// 要素番号の位置の要素を置き換え
void Array_Set(Array* list, int index, const Object* element);
