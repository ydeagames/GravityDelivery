#include "GameUtils.h"
#include <math.h>

// 整数aとbを入れ替え
void Swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

// 小数aとbを入れ替え
void SwapF(float* a, float* b)
{
	float tmp = *a;
	*a = *b;
	*b = tmp;
}

// 0.0≦x≦1.0 の小数ランダム生成
float GetRandomF(void)
{
	return (float)GetRand(RAND_MAX) / RAND_MAX;
}

// 0.0≦x≦max の小数ランダム生成
float GetRandF(float max)
{
	return GetRandomF() * max;
}

// min≦x≦max の整数ランダム生成
int GetRandRange(int min, int max)
{
	if (max < min)
		Swap(&min, &max);
	return GetRand(max - min) + min;
}

// min≦x≦max の小数ランダム生成
float GetRandRangeF(float min, float max)
{
	if (max < min)
		SwapF(&min, &max);
	return GetRandF(max - min) + min;
}

// 0～w の整数ループ
int GetLoop(int x, int w)
{
	return ((x % w) + w) % w;
}

// 0～w の小数ループ
float GetLoopF(float x, float w)
{
	return fmodf((fmodf(x, w) + w), w);
}

// min～max の整数ループ
int GetLoopRange(int x, int min, int max)
{
	if (max < min)
		Swap(&min, &max);
	return GetLoop(x - min, max - min) + min;
}

// min～max の小数ループ
float GetLoopRangeF(float x, float min, float max)
{
	if (max < min)
		SwapF(&min, &max);
	return GetLoopF(x - min, max - min) + min;
}

// 整数絶対値
int GetAbs(int a)
{
	return a > 0 ? a : -a;
}

// 小数絶対値
float GetAbsF(float a)
{
	return a > 0 ? a : -a;
}

// 整数最小値
int GetMin(int a, int b)
{
	return a < b ? a : b;
}

// 小数最小値
float GetMinF(float a, float b)
{
	return a < b ? a : b;
}

// 整数最大値
int GetMax(int a, int b)
{
	return a > b ? a : b;
}

// 整数最大値
float GetMaxF(float a, float b)
{
	return a > b ? a : b;
}

// 0～maxの範囲でxは何%の位置にいるのか
// ※ 範囲外は範囲内に収められます
float GetPercentage(float x, float max)
{
	float n = ClampF(x, 0, max);
	return n / max;
}

// min～maxの範囲でxは何%の位置にいるのか
// ※ 範囲外は範囲内に収められます
float GetPercentageRange(float x, float min, float max)
{
	if (max < min)
	{
		x = max - x + min;
		SwapF(&min, &max);
	}
	return GetPercentage(x - min, max - min);
}

// 0～maxの範囲でのpercent%の位置
float GetPercentValue(float percent, float max)
{
	return max * percent;
}

// min～maxの範囲でのpercent%の位置
float GetPercentValueRange(float percent, float min, float max)
{
	if (max < min)
	{
		percent = 1 - percent;
		SwapF(&min, &max);
	}
	return GetPercentValue(percent, max - min) + min;
}

// H : 色相　有効範囲 0.0f ～ 360.0f
// S : 彩度　有効範囲 0.0f ～ 1.0f
// V : 明度　有効範囲 0.0f ～ 1.0f
int GetColorHSV(float H, float S, float V)
{
	int hi;
	float f, p, q, t;
	float r, g, b;
	int ir, ig, ib;

	hi = (int)(H / 60.0f);
	hi = hi == 6 ? 5 : hi %= 6;
	f = H / 60.0f - (float)hi;
	p = V * (1.0f - S);
	q = V * (1.0f - f * S);
	t = V * (1.0f - (1.0f - f) * S);
	switch (hi)
	{
	case 0: r = V; g = t; b = p; break;
	case 1: r = q; g = V; b = p; break;
	case 2: r = p; g = V; b = t; break;
	case 3: r = p; g = q; b = V; break;
	case 4: r = t; g = p; b = V; break;
	case 5: r = V; g = p; b = q; break;
	}

	ir = (int)(r * 255.0f);
	if (ir > 255) ir = 255;
	else if (ir < 0) ir = 0;

	ig = (int)(g * 255.0f);
	if (ig > 255) ig = 255;
	else if (ig < 0) ig = 0;

	ib = (int)(b * 255.0f);
	if (ib > 255) ib = 255;
	else if (ib < 0) ib = 0;

	return GetColor(ir, ig, ib);
}
