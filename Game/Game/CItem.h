#pragma once

#include "CGameObject.h"
#include "CTexture.h"
#include "CAnimation.h"

#define ITEM_WIDTH 16
#define ITEM_HEIGHT 16

//syntax
#define TYPE_ITEM_MUSHROOM_RED "mushroom_red"
#define TYPE_ITEM_MUSHROOM_GREEN "mushroom_green"
#define TYPE_ITEM_COIN "coin"
#define TYPE_ITEM_COIN_BRICK "coin_brick"

#define STATE_ITEM_IDLE 3900
#define STATE_ITEM_MOVE 3901
#define STATE_ITEM_JUMP 3902
#define STATE_ITEM_DISAPPEAR 3903
#define STATE_ITEM_UP 3904

#define ID_ITEM_MUSHROOM_RED_ANI 3000
#define ID_ITEM_MUSHROOM_GREEN_ANI 3001
#define ID_ITEM_COIN_ANI 3002
#define ID_ITEM_COIN_BRICK_ANI 3003

class CItem : public CGameObject
{
	float _ay;
	float _ax;
	float _startY;
	string _type;

	int _GetAnimationId();

public:

	CItem(float x, float y, string type);

	int IsCollidable()
	{
		return 1;
	}

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Release() override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	string GetType() { return this->_type; }
	void SetType(string type);

	void SetState(int state);


	~CItem();
};