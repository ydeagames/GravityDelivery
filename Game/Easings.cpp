#include "Easings.h"
#include "GameUtils.h"

#define _USE_MATH_DEFINES
#include <math.h>

// 定数の定義 ==============================================================

#define M_PI_F ((float)M_PI)

// 内部関数の定義 ==========================================================

static float easeLinear(float t, float b, float c, float d) {
	return c * t / d + b;
}

static float easeInQuad(float t, float b, float c, float d) {
	t /= d;
	return c * t*t + b;
}

static float easeOutQuad(float t, float b, float c, float d) {
	t /= d;
	return -c * t*(t - 2) + b;
}

static float easeInOutQuad(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1)
		return c / 2 * t*t + b;
	t--;
	return -c / 2 * (t*(t - 2) - 1) + b;
}

static float easeInCubic(float t, float b, float c, float d) {
	t /= d;
	return c * t*t*t + b;
}

static float easeOutCubic(float t, float b, float c, float d) {
	t /= d;
	t--;
	return c * (t*t*t + 1) + b;
}

static float easeInOutCubic(float t, float b, float c, float d) {
	if ((t /= d / 2) < 1)
		return c / 2 * t*t*t + b;
	return c / 2 * ((t -= 2)*t*t + 2) + b;
}

static float easeInQuart(float t, float b, float c, float d) {
	return c * (t /= d)*t*t*t + b;
}

static float easeOutQuart(float t, float b, float c, float d) {
	t /= d;
	t--;
	return -c * (t*t*t*t - 1) + b;
}

static float easeInOutQuart(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1)
		return c / 2 * t*t*t*t + b;
	t -= 2;
	return -c / 2 * (t*t*t*t - 2) + b;
}

static float easeInQuint(float t, float b, float c, float d) {
	t /= d;
	return c * t*t*t*t*t + b;
}

static float easeOutQuint(float t, float b, float c, float d) {
	t /= d;
	t--;
	return c * (t*t*t*t*t + 1) + b;
}

static float easeInOutQuint(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1)
		return c / 2 * t*t*t*t*t + b;
	t -= 2;
	return c / 2 * (t*t*t*t*t + 2) + b;
}

static float easeInSine(float t, float b, float c, float d) {
	return -c * cosf(t / d * (M_PI_F / 2)) + c + b;
}

static float easeOutSine(float t, float b, float c, float d) {
	return c * sinf(t / d * (M_PI_F / 2)) + b;
}

static float easeInOutSine(float t, float b, float c, float d) {
	return -c / 2 * (cosf(M_PI_F*t / d) - 1) + b;
}

static float easeInExpo(float t, float b, float c, float d) {
	return c * powf(2, 10 * (t / d - 1)) + b;
}

static float easeOutExpo(float t, float b, float c, float d) {
	return c * (-powf(2, -10 * t / d) + 1) + b;
}

static float easeInOutExpo(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1)
		return c / 2 * powf(2, 10 * (t - 1)) + b;
	t--;
	return c / 2 * (-powf(2, -10 * t) + 2) + b;
}

static float easeInCirc(float t, float b, float c, float d) {
	t /= d;
	return -c * (sqrtf(1 - t * t) - 1) + b;
}

static float easeOutCirc(float t, float b, float c, float d) {
	t /= d;
	t--;
	return c * sqrtf(1 - t * t) + b;
}

static float easeInOutCirc(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1)
		return -c / 2 * (sqrtf(1 - t * t) - 1) + b;
	t -= 2;
	return c / 2 * (sqrtf(1 - t * t) + 1) + b;
}

static float easeInElastic(float t, float b, float c, float d) {
	float s = 1.70158f;
	float p = 0;
	float a = c;
	if (t == 0)
		return b;
	if ((t /= d) == 1)
		return b + c;
	p = d * .3f;
	if (a < fabsf(c)) {
		a = c;
		s = p / 4;
	}
	else
		s = p / (2 * M_PI_F)*asinf(c / a);
	return -(a*powf(2, 10 * (t -= 1))*sinf((t*d - s)*(2 * M_PI_F) / p)) + b;
}

static float easeOutElastic(float t, float b, float c, float d) {
	float s = 1.70158f;
	float p = 0;
	float a = c;
	if (t == 0)
		return b;
	if ((t /= d) == 1)
		return b + c;
	p = d * .3f;
	if (a < fabsf(c)) {
		a = c;
		s = p / 4;
	}
	else
		s = p / (2 * M_PI_F)*asinf(c / a);
	return a * powf(2, -10 * t)*sinf((t*d - s)*(2 * M_PI_F) / p) + c + b;
}

static float easeInOutElastic(float t, float b, float c, float d) {
	float s = 1.70158f;
	float p = 0;
	float a = c;
	if (t == 0)
		return b;
	if ((t /= d / 2) == 2)
		return b + c;
	p = d * (.3f*1.5f);
	if (a < fabsf(c)) {
		a = c;
		s = p / 4;
	}
	else
		s = p / (2 * M_PI_F)*asinf(c / a);
	if (t < 1)
		return -.5f*(a*powf(2, 10 * (t -= 1))*sinf((t*d - s)*(2 * M_PI_F) / p)) + b;
	return a * powf(2, -10 * (t -= 1))*sinf((t*d - s)*(2 * M_PI_F) / p)*.5f + c + b;
}

static float easeInBack(float t, float b, float c, float d) {
	float s = 1.70158f;
	return c * (t /= d)*t*((s + 1)*t - s) + b;
}

static float easeOutBack(float t, float b, float c, float d) {
	float s = 1.70158f;
	return c * ((t = t / d - 1)*t*((s + 1)*t + s) + 1) + b;
}

static float easeInOutBack(float t, float b, float c, float d) {
	float s = 1.70158f;
	if ((t /= d / 2) < 1)
		return c / 2 * (t*t*(((s *= 1.525f) + 1)*t - s)) + b;
	return c / 2 * ((t -= 2)*t*(((s *= 1.525f) + 1)*t + s) + 2) + b;
}

static float easeOutBounce(float t, float b, float c, float d) {
	if ((t /= d) < 1 / 2.75f)
		return c * (7.5625f*t*t) + b;
	else if (t < 2 / 2.75)
		return c * (7.5625f*(t -= 1.5f / 2.75f)*t + .75f) + b;
	else if (t < 2.5 / 2.75)
		return c * (7.5625f*(t -= 2.25f / 2.75f)*t + .9375f) + b;
	else
		return c * (7.5625f*(t -= 2.625f / 2.75f)*t + .984375f) + b;
}

static float easeInBounce(float t, float b, float c, float d) {
	return c - easeOutBounce(d - t, 0, c, d) + b;
}

static float easeInOutBounce(float t, float b, float c, float d) {
	if (t < d / 2)
		return easeInBounce(t * 2, 0, c, d)*.5f + b;
	return easeOutBounce(t * 2 - d, 0, c, d)*.5f + c * .5f + b;
}

// 関数の定義 ==============================================================

// Easingを取得
ESG GetEasing(Easings easing)
{
	switch (easing)
	{
	default:
	case ESG_LINEAR: return easeLinear; break;
	case ESG_INQUAD: return easeInQuad; break;
	case ESG_OUTQUAD: return easeOutQuad; break;
	case ESG_INOUTQUAD: return easeInOutQuad; break;
	case ESG_INCUBIC: return easeInCubic; break;
	case ESG_OUTCUBIC: return easeOutCubic; break;
	case ESG_INOUTCUBIC: return easeInOutCubic; break;
	case ESG_INQUART: return easeInQuart; break;
	case ESG_OUTQUART: return easeOutQuart; break;
	case ESG_INOUTQUART: return easeInOutQuart; break;
	case ESG_INQUINT: return easeInQuint; break;
	case ESG_OUTQUINT: return easeOutQuint; break;
	case ESG_INOUTQUINT: return easeInOutQuint; break;
	case ESG_INSINE: return easeInSine; break;
	case ESG_OUTSINE: return easeOutSine; break;
	case ESG_INOUTSINE: return easeInOutSine; break;
	case ESG_INEXPO: return easeInExpo; break;
	case ESG_OUTEXPO: return easeOutExpo; break;
	case ESG_INOUTEXPO: return easeInOutExpo; break;
	case ESG_INCIRC: return easeInCirc; break;
	case ESG_OUTCIRC: return easeOutCirc; break;
	case ESG_INOUTCIRC: return easeInOutCirc; break;
	case ESG_INELASTIC: return easeInElastic; break;
	case ESG_OUTELASTIC: return easeOutElastic; break;
	case ESG_INOUTELASTIC: return easeInOutElastic; break;
	case ESG_INBACK: return easeInBack; break;
	case ESG_OUTBACK: return easeOutBack; break;
	case ESG_INOUTBACK: return easeInOutBack; break;
	case ESG_INBOUNCE: return easeInBounce; break;
	case ESG_OUTBOUNCE: return easeOutBounce; break;
	case ESG_INOUTBOUNCE: return easeInOutBounce; break;
	}
}

// Easingを使用し0～maxの範囲でのpercent%の位置
float GetEasingValue(Easings easing, float percent, float max)
{
	return GetPercentValue(GetEasing(easing)(percent, 0, 1, 1), max);
}

// Easingを使用しmin～maxの範囲でのpercent%の位置
float GetEasingValueRange(Easings easing, float percent, float min, float max)
{
	if (max < min)
	{
		percent = 1 - percent;
		SwapF(&min, &max);
	}
	return GetEasingValue(easing, percent, max - min) + min;
}
