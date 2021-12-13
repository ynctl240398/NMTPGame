#pragma once

#include "CTexture.h"
#include "CGameObject.h"

#define STATE_BRICK_P_BRICK					11900
#define STATE_BRICK_P_IDLE					11901
#define STATE_BRICK_P_PUSHED				11902
#define STATE_BRICK_P_BRICK_UP				11903
#define STATE_BRICK_P_BRICK_BREAK			11904

#define BRICK_P_BBOX_WIDTH					16
#define BRICK_P_BBOX_HEIGHT					16

#define BRICK_BROKEN_BBOX_WIDTH				8
#define BRICK_BROKEN_BBOX_HEIGHT			8

#define BRICK_BROKEN_GRAVITY				0.002f
#define BRICK_BROKEN_SPEED_X				0.001f
#define BRICK_BROKEN_JUMP_DEFLECT_SPEED		0.4f

class CBrickBorken : public CGameObject 
{
public:
	CBrickBorken(float x , float y) {
		_ay = -BRICK_BROKEN_GRAVITY;
		_position = { x,y };
	}

	void Render() override {

	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override {
	}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {

	}

	~CBrickBorken() {

	}
};

class CBrickP : public CGameObject
{
	void _RenderBrickBroken();
	int _GetAnimationId();

public:
	CBrickP(float x, float y) {
		_position = { x,y };
	}

	void SetState(int) override;


	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;

	~CBrickP() {

	}
};
