#pragma once

#include "CGameObject.h"
#include "CAnimation.h"
#include "CTexture.h"
#include "CGameObject.h"

#define BRICK_WIDTH 16
#define BRICK_HEIGHT 16

#define STATE_BRICK_QUESTION_IDLE 2900
#define STATE_BRICK_QUESTION_RUN 2901

#define ID_BRICK_QUESTION_ANI_IDLE 2000
#define ID_BRICK_QUESTION_ANI_RUN 2001

#define JUMP_SPEED 0.015f
#define MAX_JUMP 0.6f

class CBrickQuestion : public CGameObject
{
	float _maxVy;
	float _ay;

	float _startY;

	int _GetAnimationId();
public:
	CBrickQuestion(float x, float y);

	void SetState(int) override;

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void Release() override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};