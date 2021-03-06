#pragma once

#include "CGameObject.h"
#include "CTexture.h"
#include "CAnimation.h"

#define ITEM_WIDTH 16
#define ITEM_HEIGHT 16

//syntax
#define TYPE_ITEM_MUSHROOM_RED "mushroom_red"
#define TYPE_ITEM_MUSHROOM_GREEN "mushroom_green"
#define TYPE_ITEM_COIN_BRICK "coin_brick"
#define TYPE_ITEM_P "p"
#define TYPE_ITEM_LEAF "leaf"

#define STATE_ITEM_IDLE 3900
#define STATE_ITEM_MOVE 3901
#define STATE_ITEM_JUMP 3902
#define STATE_ITEM_DISAPPEAR 3903
#define STATE_ITEM_UP 3904
#define STATE_ITEM_P_PUSHED 3905
#define STATE_ITEM_FALL 3906

#define ID_ANI_ITEM_MUSHROOM_RED 3000
#define ID_ANI_ITEM_MUSHROOM_GREEN 3001
#define ID_ANI_ITEM_COIN_BRICK 3003
#define ID_ANI_ITEM_LEAF 3004
#define ID_ANI_ITEM_P 11001
#define ID_ANI_ITEM_P_PUSHED 11002

class CItem : public CGameObject
{
	string _type;

	int _GetAnimationId();

	int _step;

	vector<LPGAMEOBJECT> _coObjects;

public:

	CItem(float x, float y, string type);

	int IsCollidable()
	{
		return 1;
	}

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnBlockingOn(bool isHorizontal, float z);

	string GetType() { return this->_type; }
	void SetType(string type);

	virtual int IsBlocking(LPCOLLISIONEVENT e);

	~CItem();
};