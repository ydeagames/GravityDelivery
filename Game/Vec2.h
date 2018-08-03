#pragma once

// �\���̂̐錾 ============================================================

// <�x�N�g��>
typedef struct
{
	float x;	// X���W
	float y;	// Y���W
} Vec2;

// �֐��̐錾 ==============================================================

// <�x�N�g���쐬>
Vec2 Vec2_Create(float x = 0.f, float y = 0.f);

// <�x�N�g���̒���>
float Vec2_Length(const Vec2* vec);

// <�x�N�g���̒����̓��>
float Vec2_LengthSquared(const Vec2* vec);

// <��������̃x�N�g���Ƃ̓���>
float Vec2_Dot(const Vec2* vec, const Vec2* other);

// <��������̃x�N�g���Ƃ̋���>
float Vec2_LengthTo(const Vec2* vec, const Vec2* other);

// <��������̃x�N�g���Ƃ̋����̓��>
float Vec2_LengthSquaredTo(const Vec2* vec, const Vec2* other);

// <���K���i������1�ɂ����j�x�N�g��>
Vec2 Vec2_Normalized(const Vec2* vec);

// <���l�̃x�N�g����>
BOOL Vec2_EqualsEpsilon(const Vec2* vec, const Vec2* other, float epsilon);

// <���l�̃x�N�g����>
BOOL Vec2_Equals(const Vec2* vec, const Vec2* other);

// <0�x�N�g����>
BOOL Vec2_IsZero(const Vec2* vec);

// <�x�N�g���̊p�x>
float Vec2_Angle(const Vec2* vec);

// <�x�N�g���̊p�x���Z>
Vec2 Vec2_Rotate(const Vec2* vec, float rot);

// <�x�N�g���𕪉�>
void Vec2_Decompose(const Vec2* vec, const Vec2* angle, Vec2* vec_a, Vec2* vec_b);

// <�x�N�g�������Z>
Vec2 Vec2_Add(const Vec2* vec_a, const Vec2* vec_b);

// <�x�N�g�������Z>
Vec2 Vec2_Sub(const Vec2* vec_a, const Vec2* vec_b);

// <�x�N�g�������Z>
Vec2 Vec2_Scale(const Vec2* vec, float scale);

// <�x�N�g���𔽓]>
Vec2 Vec2_Negate(const Vec2* vec);

// <�x�N�g����`��>
void Vec2_Render(const Vec2* vec, const Vec2* base, unsigned int color, float Thickness = 1.f);
