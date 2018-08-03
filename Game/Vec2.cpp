#include "Vec2.h"
#define _USE_MATH_DEFINES
#include <math.h>

// 定数の定義 ==============================================================

#define FLOAT_EPSILON (10e-6f)

// 関数の定義 ==============================================================

// <ベクトル作成>
Vec2 Vec2_Create(float x, float y)
{
	return{ x, y };
}

// <ベクトルの長さ>
float Vec2_Length(const Vec2* vec)
{
	return sqrtf(Vec2_LengthSquared(vec));
}

// <ベクトルの長さの二乗>
float Vec2_LengthSquared(const Vec2* vec)
{
	return vec->x * vec->x + vec->y * vec->y;
}

// <もう一方のベクトルとの内積>
float Vec2_Dot(const Vec2* vec, const Vec2* other)
{
	return vec->x * other->x + vec->y * other->y;
}

// <もう一方のベクトルとの距離>
float Vec2_LengthTo(const Vec2* vec, const Vec2* other)
{
	return sqrtf(Vec2_LengthSquaredTo(vec, other));
}

// <もう一方のベクトルとの距離の二乗>
float Vec2_LengthSquaredTo(const Vec2* vec, const Vec2* other)
{
	return (other->x - vec->x) * (other->x - vec->x) + (other->y - vec->y) * (other->y - vec->y);
}

// <正規化（長さを1にした）ベクトル>
Vec2 Vec2_Normalized(const Vec2* vec)
{
	float length = Vec2_Length(vec);
	if (length > 0)
		return Vec2_Create(vec->x / length, vec->y / length);
	return Vec2_Create();
}

// <同値のベクトルか>
BOOL Vec2_EqualsEpsilon(const Vec2* vec, const Vec2* other, float epsilon)
{
	return fabsf(vec->x - other->x) < epsilon && fabsf(vec->y - other->y) < epsilon;
}

// <同値のベクトルか>
BOOL Vec2_Equals(const Vec2* vec, const Vec2* other)
{
	return Vec2_EqualsEpsilon(vec, other, FLOAT_EPSILON);
}

// <0ベクトルか>
BOOL Vec2_IsZero(const Vec2* vec)
{
	return Vec2_Equals(vec, &Vec2_Create());
}

// <ベクトルの角度>
float Vec2_Angle(const Vec2* vec)
{
	return atan2f(vec->y, vec->x);
}

// <ベクトルの角度加算>
Vec2 Vec2_Rotate(const Vec2* vec, float rot)
{
	float scale = Vec2_Length(vec);
	float angle = Vec2_Angle(vec);
	return Vec2_Create(cosf(angle + rot)*scale, sinf(angle + rot)*scale);
}

// <ベクトルを分解>
void Vec2_Decompose(const Vec2* vec, const Vec2* angle, Vec2* vec_a, Vec2* vec_b)
{
	float angle_rota = Vec2_Angle(angle);
	float vec_rota = Vec2_Angle(vec);
	float diff_rota = vec_rota - angle_rota;
	float vec_length = Vec2_Length(vec);

	float vec_a_length = vec_length * cosf(diff_rota);
	float vec_b_length = vec_length * sinf(diff_rota);
	float vec_a_rota = angle_rota;
	float vec_b_rota = angle_rota + (float)M_PI_2;

	*vec_a = Vec2_Create(vec_a_length * cosf(vec_a_rota), vec_a_length * sinf(vec_a_rota));
	*vec_b = Vec2_Create(vec_b_length * cosf(vec_b_rota), vec_b_length * sinf(vec_b_rota));
}

// <ベクトルを加算>
Vec2 Vec2_Add(const Vec2* vec_a, const Vec2* vec_b)
{
	return Vec2_Create(vec_a->x + vec_b->x, vec_a->y + vec_b->y);
}

// <ベクトルを減算>
Vec2 Vec2_Sub(const Vec2* vec_a, const Vec2* vec_b)
{
	return Vec2_Create(vec_a->x - vec_b->x, vec_a->y - vec_b->y);
}

// <ベクトルを減算>
Vec2 Vec2_Scale(const Vec2* vec, float scale)
{
	return Vec2_Create(vec->x * scale, vec->y * scale);
}

// <ベクトルを反転>
Vec2 Vec2_Negate(const Vec2* vec)
{
	return Vec2_Scale(vec, -1);
}

// <ベクトルを描画>
void Vec2_Render(const Vec2* vec, const Vec2* base, unsigned int color, float Thickness)
{
	float arrow_length = 10 + Thickness * Vec2_Length(vec)*.125f;
	float arrow_rota = Vec2_Angle(vec);
	float arrow_rota1 = arrow_rota + ToRadians(-150);
	float arrow_rota2 = arrow_rota + ToRadians(150);
	DrawLineAA(base->x, base->y, base->x + vec->x, base->y + vec->y, color, Thickness);
	DrawLineAA(base->x + vec->x, base->y + vec->y, base->x + vec->x + (arrow_length * cosf(arrow_rota1)), base->y + vec->y + (arrow_length * sinf(arrow_rota1)), color, Thickness);
	DrawLineAA(base->x + vec->x, base->y + vec->y, base->x + vec->x + (arrow_length * cosf(arrow_rota2)), base->y + vec->y + (arrow_length * sinf(arrow_rota2)), color, Thickness);
}