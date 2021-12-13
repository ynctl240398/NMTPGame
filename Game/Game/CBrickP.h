#pragma once

#include "CTexture.h"
#include "CGameObject.h"
#include "CCam.h"
#include "CGame.h"

#define STATE_BRICK_P_BRICK					11900
#define STATE_BRICK_P_IDLE					11901
#define STATE_BRICK_P_PUSHED				11902
#define STATE_BRICK_P_BRICK_UP				11903
#define STATE_BRICK_P_BRICK_BREAK			11904
#define STATE_BRICK_P_COIN					11905

#define BRICK_P_BBOX_WIDTH					16
#define BRICK_P_BBOX_HEIGHT					16

#define BRICK_BROKEN_BBOX_WIDTH				8
#define BRICK_BROKEN_BBOX_HEIGHT			8

#define BRICK_BROKEN_GRAVITY				0.002f
#define BRICK_BROKEN_SPEED_X				0.1f
#define BRICK_BROKEN_JUMP_DEFLECT_SPEED		0.5f

#define ID_ANI_PARTICLES_BRICK_BROKEN_IDLE	12000

class CBrickBorken : public CGameObject 
{

public:
	CBrickBorken(float x , float y)
	{
		_ay = BRICK_BROKEN_GRAVITY;
		_position = { x,y };
		_velocity = { 0,0 };
		_isDeleted = false;
	}

	void Render() override {
		_aniId = ID_ANI_PARTICLES_BRICK_BROKEN_IDLE;
		CGameObject::Render();
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override {
		_velocity.x += _ax * dt;
		_velocity.y += _ay * dt;

		float topCam = CCam::GetInstance()->GetPosition().y;
		float bottomCam = CGame::GetInstance()->GetBackBufferHeight() + topCam;

		if (_position.y > bottomCam) {
			Delete();
		}

		_position.x += _velocity.x * dt;
		_position.y += (_velocity.y * dt) / 2.25f;
	}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {

	}

	~CBrickBorken() {

	}
};

class CBrickP : public CGameObject
{
	vector<CBrickBorken*> _brickBrokens;

	void _HandleBrickBroken();
	int _GetAnimationId();

	bool _isShowP;
	bool _isCanBreak;


public:
	CBrickP(float x, float y, bool isCanBreak, int state) {
		_position = { x,y };
		_isShowP = false;
		_isCanBreak = isCanBreak;
		SetState(state);
	}

	void SetState(int) override;
	int IsCollidable()
	{
		int result = !(_state == STATE_BRICK_P_BRICK_BREAK || _state == STATE_BRICK_P_PUSHED || (_state == STATE_BRICK_P_IDLE && !_isShowP));
		return result;
	}

	void SetShowP(bool value) { _isShowP = value; }
	bool GetShowP() { return _isShowP; }

	bool GetBreak() { return _isCanBreak; }

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;

	~CBrickP() {

	}
};