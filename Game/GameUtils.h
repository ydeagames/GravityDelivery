#pragma once

// 整数aとbを入れ替え
void Swap(int* a, int* b);

// 小数aとbを入れ替え
void SwapF(float* a, float* b);

// 0.0≦x≦1.0 の小数ランダム生成
float GetRandomF(void);

// 0.0≦x≦max の小数ランダム生成
float GetRandF(float max);

// min≦x≦max の整数ランダム生成
int GetRandRange(int min, int max);

// min≦x≦max の小数ランダム生成
float GetRandRangeF(float min, float max);

// 0～w の整数ループ
int GetLoop(int x, int w);

// 0～w の小数ループ
float GetLoopF(float x, float w);

// min～max の整数ループ
int GetLoopRange(int x, int min, int max);

// min～max の小数ループ
float GetLoopRangeF(float x, float min, float max);

// 整数絶対値
int GetAbs(int a);

// 小数絶対値
float GetAbsF(float a);

// 整数最小値
int GetMin(int a, int b);

// 小数最小値
float GetMinF(float a, float b);

// 整数最大値
int GetMax(int a, int b);

// 整数最大値
float GetMaxF(float a, float b);

// 0～maxの範囲でxは何%の位置にいるのか
// ※ 範囲外は範囲内に収められます
float GetPercentage(float x, float max);

// min～maxの範囲でxは何%の位置にいるのか
// ※ 範囲外は範囲内に収められます
float GetPercentageRange(float x, float min, float max);

// 0～maxの範囲でのpercent%の位置
float GetPercentValue(float percent, float max);

// min～maxの範囲でのpercent%の位置
float GetPercentValueRange(float percent, float min, float max);

// H : 色相　有効範囲 0.0f ～ 360.0f
// S : 彩度　有効範囲 0.0f ～ 1.0f
// V : 明度　有効範囲 0.0f ～ 1.0f
int GetColorHSV(float H, float S, float V);
