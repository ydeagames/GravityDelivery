#include "GameObject.h"
#include "GameUtils.h"
#include "GameMain.h"
#include "Vector.h"

// �O���[�o���ϐ��̒�` ====================================================

BOOL g_debug_mode = FALSE;

// <<�e�B�b�N>> --------------------------------------------------------

// <�ŏI����>
static int g_lastcount = -1;
// <�f���^�~���b>
static int g_deltamilliseconds = 0;

// �֐��̒�` ==============================================================

// <<�e�N�X�`��>> ------------------------------------------------------

// <�e�N�X�`���쐬>
GameTexture GameTexture_Create(HGRP texture, Vec2 anchor, Vec2 size)
{
	return{ texture, anchor, size, Vec2_Create(size.x / 2, size.y / 2) };
}

// <�e�N�X�`���Ȃ�>
GameTexture GameTexture_CreateNone()
{
	return GameTexture_Create(TEXTURE_NONE, Vec2_Create(), Vec2_Create());
}

// <<�X�v���C�g�A�j���[�V����>> ----------------------------------------

// <�X�v���C�g�A�j���[�V�����쐬>
GameSpriteAnimation GameSpriteAnimation_Create(int frames_start, int frames_end, int frame_duration, BOOL pause)
{
	return{ frames_start, frames_end, frame_duration, 0, TRUE, pause, ANIMATION_RUNNING };
}

// <�X�v���C�g�A�j���[�V�����Ȃ�>
GameSpriteAnimation GameSpriteAnimation_CreateNone()
{
	return GameSpriteAnimation_Create(-1, -1, 1, TRUE);
}

// <�X�v���C�g�A�j���[�V�����X�V>
AnimationState GameSpriteAnimation_Update(GameSprite* animate_sprite)
{
	if (!animate_sprite->animation.pause_flag && animate_sprite->animation.frame_start >= 0 && animate_sprite->animation.frame_end >= 0)
	{
		// �A�j���[�V�������Ă��邩
		animate_sprite->animation.result = ANIMATION_RUNNING;
		// �o�ߎ���
		animate_sprite->animation.elapsed_time++;
		// �ŏ��̃t���[���ȏ�
		animate_sprite->frame_index = GetMax(animate_sprite->frame_index, animate_sprite->animation.frame_start);

		// �t���[���o��
		if (animate_sprite->animation.elapsed_time > animate_sprite->animation.frame_duration)
		{
			// �o�ߎ���
			animate_sprite->animation.elapsed_time = 0;
			// �t���[���ԍ�
			animate_sprite->frame_index++;

			// �ŏ��ɖ߂�
			if (animate_sprite->frame_index > animate_sprite->animation.frame_end)
			{
				// ���[�v����Ȃ�
				if (animate_sprite->animation.loop_flag)
					// �ŏ��ɖ߂�
					animate_sprite->frame_index = animate_sprite->animation.frame_start;
				else
					// �Ō�̃t���[���Œ�~
					animate_sprite->frame_index = animate_sprite->animation.frame_end;
				// �A�j���[�V��������
				animate_sprite->animation.result = ANIMATION_FINISHED;
			}
		}
	}
	else
		// �A�j���[�V�������Ă��邩
		animate_sprite->animation.result = ANIMATION_IDLE;

	return animate_sprite->animation.result;
}

// <<�X�v���C�g>> ------------------------------------------------------

// <�X�v���C�g�쐬>
GameSprite GameSprite_Create(GameTexture texture, float scale, float angle)
{
	return{ COLOR_WHITE, texture, texture.size, 1, 0, Vec2_Create(), scale, angle, GameSpriteAnimation_CreateNone() };
}

// <�X�v���C�g�Ȃ�>
GameSprite GameSprite_CreateNone()
{
	return GameSprite_Create(GameTexture_CreateNone(), 0, 0);
}

// <�X�v���C�g�X�V>
void GameSprite_SetFrame(GameSprite* sprite, int frame)
{
	// �t���[���ԍ�
	sprite->frame_index = frame;
}

// <�X�v���C�g�`��>
void GameSprite_Render(const GameSprite* sprite, const Vec2* pos)
{
	int column = sprite->frame_index%sprite->num_columns;
	int row = sprite->frame_index / sprite->num_columns;

	Vec2 anchor = Vec2_Add(&sprite->texture.anchor, &Vec2_Create(sprite->size.x * column, sprite->size.y * row));

	// �X�v���C�g�`��
	DrawRectRotaGraph2F(
		pos->x + sprite->offset.x, pos->y + sprite->offset.y,
		(int)anchor.x, (int)anchor.y,
		(int)sprite->texture.size.x, (int)sprite->texture.size.y,
		sprite->texture.center.x, sprite->texture.center.y,
		(double)sprite->scale,
		(double)sprite->angle,
		sprite->texture.texture,
		TRUE
	);
}

// <<�e�B�b�N>> --------------------------------------------------------

// <�I�u�W�F�N�g�쐬>
void GameTick_Update(void)
{
	int now = GetNowCount();
	g_deltamilliseconds = GetMin(100, now - g_lastcount);
	g_lastcount = now;
}

// <<�I�u�W�F�N�g>> ----------------------------------------------------

// <�I�u�W�F�N�g�쐬>
GameObject GameObject_Create(Vec2 pos, Vec2 vel, Vec2 size)
{
	return{ pos, vel, size, SHAPE_BOX, GameSprite_CreateNone(), FALSE, 0, CONNECTION_NONE, TRUE, 1, 0, GameTimer_Create() };
}

// <���I�u�W�F�N�g�쐬>
GameObject GameObject_CreateLine(Vec2 pos1, Vec2 pos2, Vec2 vel)
{
	Vec2 pos = Vec2_Scale(&Vec2_Add(&pos1, &pos2), .5f);
	Vec2 size = Vec2_Sub(&pos2, &pos1);
	GameObject obj = GameObject_Create(pos, vel, size);
	obj.shape = SHAPE_LINE;
	return obj;
}

// <�I�u�W�F�N�g�폜>
void GameObject_Dispose(GameObject* obj)
{
	obj->alive = FALSE;
}

// <�I�u�W�F�N�g�m�F>
BOOL GameObject_IsAlive(const GameObject* obj)
{
	return obj->alive;
}

// <�I�u�W�F�N�g���W�X�V>
void GameObject_UpdatePosition(GameObject* obj)
{
	obj->pos.x += obj->vel.x;// *(g_deltamilliseconds / 17.f);
	obj->pos.y += obj->vel.y;// *(g_deltamilliseconds / 17.f);
}

// <�I�u�W�F�N�gX�I�t�Z�b�g>
float GameObject_OffsetX(const GameObject* obj, ObjectSide side, float pos, float padding)
{
	float offset = 0;
	switch (side)
	{
	case LEFT:
		offset = -(padding + GetAbsF(obj->size.x) / 2.f);
		break;
	case RIGHT:
		offset = (padding + GetAbsF(obj->size.x) / 2.f);
		break;
	}
	return pos + offset;
}

// <�I�u�W�F�N�gX�I�t�Z�b�g>
float GameObject_OffsetY(const GameObject* obj, ObjectSide side, float pos, float padding)
{
	float offset = 0;
	switch (side)
	{
	case TOP:
		offset = -(padding + GetAbsF(obj->size.y) / 2.f);
		break;
	case BOTTOM:
		offset = (padding + GetAbsF(obj->size.y) / 2.f);
		break;
	}
	return pos + offset;
}

// <�I�u�W�F�N�gX�ʒu�Q�b�g>
float GameObject_GetX(const GameObject* obj, ObjectSide side, float padding)
{
	return GameObject_OffsetX(obj, side, obj->pos.x, padding);
}

// <�I�u�W�F�N�gY�ʒu�Q�b�g>
float GameObject_GetY(const GameObject* obj, ObjectSide side, float padding)
{
	return GameObject_OffsetY(obj, side, obj->pos.y, padding);
}

// <�I�u�W�F�N�gX�I�t�Z�b�g>
float GameObject_OffsetRawX(const GameObject* obj, ObjectSide side, float pos, float padding)
{
	float offset = 0;
	switch (side)
	{
	case LEFT:
		offset = -(padding + obj->size.x / 2.f);
		break;
	case RIGHT:
		offset = (padding + obj->size.x / 2.f);
		break;
	}
	return pos + offset;
}

// <�I�u�W�F�N�gX�I�t�Z�b�g>
float GameObject_OffsetRawY(const GameObject* obj, ObjectSide side, float pos, float padding)
{
	float offset = 0;
	switch (side)
	{
	case TOP:
		offset = -(padding + obj->size.y / 2.f);
		break;
	case BOTTOM:
		offset = (padding + obj->size.y / 2.f);
		break;
	}
	return pos + offset;
}

// <�I�u�W�F�N�gX�ʒu�Q�b�g>
float GameObject_GetRawX(const GameObject* obj, ObjectSide side, float padding)
{
	return GameObject_OffsetRawX(obj, side, obj->pos.x, padding);
}

// <�I�u�W�F�N�gY�ʒu�Q�b�g>
float GameObject_GetRawY(const GameObject* obj, ObjectSide side, float padding)
{
	return GameObject_OffsetRawY(obj, side, obj->pos.y, padding);
}

// <�I�u�W�F�N�g�`��>
void GameObject_Render(const GameObject* obj, const Vec2* translate)
{
	float box_xl = GameObject_GetRawX(obj, LEFT) + translate->x;
	float box_xc = GameObject_GetRawX(obj, CENTER_X) + translate->x;
	float box_xr = GameObject_GetRawX(obj, RIGHT) + translate->x;
	float box_yt = GameObject_GetRawY(obj, TOP) + translate->y;
	float box_ym = GameObject_GetRawY(obj, CENTER_Y) + translate->y;
	float box_yb = GameObject_GetRawY(obj, BOTTOM) + translate->y;
	Vec2 box_t = Vec2_Create(box_xc, box_ym);

	// �e�N�X�`�����m�F
	if (obj->sprite.texture.texture != TEXTURE_NONE)
	{
		if (obj->sprite.texture.texture != TEXTURE_MISSING)
		{
			switch (obj->sprite_connection)
			{
			case CONNECTION_LOOP:
			case CONNECTION_BARRIER:
			{
				// ���s�[�g�^�C�� (��]�A�e�N�X�`�����S���W �ɂ͖��Ή�)
				Vec2 center_offset = Vec2_Scale(&obj->sprite.texture.center, obj->sprite.scale);
				Vec2 sp_pos = Vec2_Add(&box_t, &obj->sprite.offset);
				Vec2 sp_size = Vec2_Scale(&obj->sprite.texture.size, obj->sprite.scale);

				float go_left = box_xl;
				float go_right = box_xr;
				float go_top = box_yt;
				float go_bottom = box_yb;

				float sp_left = sp_pos.x - sp_size.x / 2;
				float sp_right = sp_pos.x + sp_size.x / 2;
				float sp_top = sp_pos.y - sp_size.y / 2;
				float sp_bottom = sp_pos.y + sp_size.y / 2;

				switch (obj->sprite_connection)
				{
				case CONNECTION_BARRIER:
					if (sp_left < go_right && go_left < sp_right && sp_top < go_bottom && go_top < sp_bottom)
						GameSprite_Render(&obj->sprite, &box_t);
					break;
				case CONNECTION_LOOP:
					float offset_x = GetLoopRangeF(go_left, sp_left, sp_right) - sp_left;
					float offset_y = GetLoopRangeF(go_top, sp_top, sp_bottom) - sp_top;

					if (sp_size.x >= 1.f && sp_size.y >= 1.f)
					{
						for (float iy = go_top + sp_size.y / 2 - offset_y - center_offset.y; iy < go_bottom; iy += sp_size.y)
						{
							for (float ix = go_left + sp_size.x / 2 - offset_x - center_offset.x; ix < go_right; ix += sp_size.x)
							{
								GameSprite_Render(&obj->sprite, &Vec2_Create(ix + sp_size.x / 2 - obj->sprite.offset.x, iy + sp_size.y / 2 - obj->sprite.offset.y));

								if (DEBUG_HITBOX)
									DrawBoxAA(ix, iy, ix + sp_size.x, iy + sp_size.y, obj->sprite.color, FALSE, .5f);
							}
						}
					}

					break;
				}

				break;
			}
			default:
				GameSprite_Render(&obj->sprite, &box_t);
				break;
			}
		}
		else
		{
			// NULL�e�N�X�`����\��
			DrawBoxAA(box_xl, box_yt, box_xr, box_yb, COLOR_BLACK, TRUE);
			DrawBoxAA(box_xl, box_yt, box_xc, box_ym, COLOR_FUCHSIA, TRUE);
			DrawBoxAA(box_xc, box_ym, box_xr, box_yb, COLOR_FUCHSIA, TRUE);
			//DrawBoxAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, FALSE, .5f);
		}
	}

	// �}�`�`��
	switch (obj->shape)
	{
	default:
	case SHAPE_BOX:
		// ��`�`��
		if (obj->fill)
			DrawBoxAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, TRUE);
		if (obj->edge > 0)
			DrawBoxAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, FALSE, obj->edge);
		if (DEBUG_HITBOX)
		{
			DrawBoxAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, FALSE, .5f);
			DrawLineAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, .5f);
			DrawLineAA(box_xr, box_yt, box_xl, box_yb, obj->sprite.color, .5f);
		}
		break;
	case SHAPE_CIRCLE:
	{
		float r1 = GetMinF(GetAbsF(obj->size.x), GetAbsF(obj->size.y)) / 2;
		// �~
		if (obj->fill)
			DrawCircleAA(box_xc, box_ym, r1, 120, obj->sprite.color, TRUE);
		if (obj->edge > 0)
			DrawCircleAA(box_xc, box_ym, r1, 120, obj->sprite.color, FALSE, obj->edge);
		if (DEBUG_HITBOX)
		{
			DrawCircleAA(box_xc, box_ym, r1, 120, obj->sprite.color, FALSE, .5f);
			DrawBoxAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, FALSE, .5f);
			DrawLineAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, .5f);
			DrawLineAA(box_xr, box_yt, box_xl, box_yb, obj->sprite.color, .5f);
		}
		break;
	}
	case SHAPE_LINE:
	{
		float r1 = obj->edge;
		// ��
		if (DEBUG_HITBOX)
			Vec2_Render(&obj->size, &Vec2_Create(box_xl, box_yt), obj->sprite.color, .5f);
		DrawLineAA(box_xl, box_yt, box_xr, box_yb, obj->sprite.color, r1);
		break;
	}
	}
}

// <�I�u�W�F�N�g�T�C�Y�ύX>
void GameObject_SetSize(GameObject* obj, float scale, float size)
{
	obj->sprite.scale = scale;
	obj->size = Vec2_Create(size * scale, size * scale);
}

// <<�t�B�[���h�I�u�W�F�N�g>> ------------------------------------------

// <�t�B�[���h�I�u�W�F�N�g�쐬>
GameObject GameObject_Field_Create(void)
{
	return GameObject_Create(Vec2_Create(SCREEN_CENTER_X, SCREEN_CENTER_Y), Vec2_Create(), Vec2_Create(SCREEN_WIDTH, SCREEN_HEIGHT));
}

// <�t�B�[���h�㉺�Փˏ���>
ObjectSide GameObject_Field_CollisionVertical(const GameObject* field, GameObject* obj, ObjectConnection connection, ObjectEdgeSide edge)
{
	// �q�b�g�T�C�h
	ObjectSide side_hit = NONE;

	// �R�E���E�㉺�Ǔ����蔻��
	{
		// ���ɉ����ăp�f�B���O�𒲐�
		float padding_top = GameObject_GetY(field, TOP);
		float padding_bottom = GameObject_GetY(field, BOTTOM);
		switch (edge)
		{
		case EDGESIDE_INNER:
			padding_top = GameObject_OffsetY(obj, BOTTOM, padding_top);
			padding_bottom = GameObject_OffsetY(obj, TOP, padding_bottom);
			break;
		case EDGESIDE_OUTER:
			padding_top = GameObject_OffsetY(obj, TOP, padding_top);
			padding_bottom = GameObject_OffsetY(obj, BOTTOM, padding_bottom);
			break;
		}

		// �����蔻��
		if (obj->pos.y < padding_top)
			side_hit = TOP;
		else if (padding_bottom <= obj->pos.y)
			side_hit = BOTTOM;

		// �t�B�[���h�̂Ȃ���
		switch (connection)
		{
		case CONNECTION_BARRIER:
			// �ǂɂ��������璲��
			obj->pos.y = ClampF(obj->pos.y, padding_top, padding_bottom);
			break;
		case CONNECTION_LOOP:
			// �ǂɂ��������烋�[�v
			obj->pos.y = GetLoopRangeF(obj->pos.y, padding_top, padding_bottom);
			break;
		}
	}

	return side_hit;
}

// <�t�B�[���h���E�Փˏ���>
ObjectSide GameObject_Field_CollisionHorizontal(const GameObject* field, GameObject* obj, ObjectConnection connection, ObjectEdgeSide edge)
{
	// �q�b�g�T�C�h
	ObjectSide side_hit = NONE;

	// �R�E���E���E�Ǔ����蔻��
	{
		// ���ɉ����ăp�f�B���O�𒲐�
		float padding_left = GameObject_GetX(field, LEFT);
		float padding_right = GameObject_GetX(field, RIGHT);
		switch (edge)
		{
		case EDGESIDE_INNER:
			padding_left = GameObject_OffsetX(obj, RIGHT, padding_left);
			padding_right = GameObject_OffsetX(obj, LEFT, padding_right);
			break;
		case EDGESIDE_OUTER:
			padding_left = GameObject_OffsetX(obj, LEFT, padding_left);
			padding_right = GameObject_OffsetX(obj, RIGHT, padding_right);
			break;
		}

		// �����蔻��
		if (obj->pos.x < padding_left)
			side_hit = LEFT;
		else if (padding_right <= obj->pos.x)
			side_hit = RIGHT;

		// �t�B�[���h�̂Ȃ���
		switch (connection)
		{
		case CONNECTION_BARRIER:
			// �ǂɂ��������璲��
			obj->pos.x = ClampF(obj->pos.x, padding_left, padding_right);
			break;
		case CONNECTION_LOOP:
			// �ǂɂ��������烋�[�v
			obj->pos.x = GetLoopRangeF(obj->pos.x, padding_left, padding_right);
			break;
		}
	}

	return side_hit;
}
