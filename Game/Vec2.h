#pragma once

// 構造体の宣言 ============================================================

// <ベクトル>
typedef struct
{
	float x;	// X座標
	float y;	// Y座標
} Vec2;

// 関数の宣言 ==============================================================

// <ベクトル作成>
Vec2 Vec2_Create(float x = 0.f, float y = 0.f);

// <ベクトルの長さ>
float Vec2_Length(const Vec2* vec);

// <ベクトルの長さの二乗>
float Vec2_LengthSquared(const Vec2* vec);

// <もう一方のベクトルとの内積>
float Vec2_Dot(const Vec2* vec, const Vec2* other);

// <もう一方のベクトルとの距離>
float Vec2_LengthTo(const Vec2* vec, const Vec2* other);

// <もう一方のベクトルとの距離の二乗>
float Vec2_LengthSquaredTo(const Vec2* vec, const Vec2* other);

// <正規化（長さを1にした）ベクトル>
Vec2 Vec2_Normalized(const Vec2* vec);

// <同値のベクトルか>
BOOL Vec2_EqualsEpsilon(const Vec2* vec, const Vec2* other, float epsilon);

// <同値のベクトルか>
BOOL Vec2_Equals(const Vec2* vec, const Vec2* other);

// <0ベクトルか>
BOOL Vec2_IsZero(const Vec2* vec);

// <ベクトルの角度>
float Vec2_Angle(const Vec2* vec);

// <ベクトルの角度加算>
Vec2 Vec2_Rotate(const Vec2* vec, float rot);

// <ベクトルを分解>
void Vec2_Decompose(const Vec2* vec, const Vec2* angle, Vec2* vec_a, Vec2* vec_b);

// <ベクトルを加算>
Vec2 Vec2_Add(const Vec2* vec_a, const Vec2* vec_b);

// <ベクトルを減算>
Vec2 Vec2_Sub(const Vec2* vec_a, const Vec2* vec_b);

// <ベクトルを減算>
Vec2 Vec2_Scale(const Vec2* vec, float scale);

// <ベクトルを反転>
Vec2 Vec2_Negate(const Vec2* vec);

// <ベクトルを描画>
void Vec2_Render(const Vec2* vec, const Vec2* base, unsigned int color, float Thickness = 1.f);
