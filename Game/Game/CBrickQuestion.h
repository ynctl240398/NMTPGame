#pragma once

#include "CGameObject.h"
#include "CAnimation.h"
#include "CTexture.h"
#include "CGameObject.h"
#include "CItem.h"

#define BRICK_WIDTH 16
#define BRICK_HEIGHT 16

#define STATE_BRICK_QUESTION_IDLE 2900
#define STATE_BRICK_QUESTION_RUN 2901
#define STATE_BRICK_BRICK_RUN 2902

#define ID_ANI_BRICK_QUESTION_IDLE 2000
#define ID_ANI_BRICK_QUESTION_RUN 2001
#define ID_ANI_BRICK_BRICK_RUN 11000

#define JUMP_SPEED 0.15f

class CBrickQuestion : public CGameObject
{	
	CItem* _item;

	bool _active;
	int _step;

	int _GetAnimationId();

public:
	CBrickQuestion(float x, float y, string typeItem, string skin);

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	int IsBlocking(LPCOLLISIONEVENT e) {
		if (dynamic_cast<CItem*>(e->src_obj)) {
			CItem* item = dynamic_cast<CItem*>(e->src_obj);
			if (item->GetType() == TYPE_ITEM_LEAF) {
				return 0;
			}
		}
		return 1;
	}

	CItem* GetItem() { return _item; }
	void Active();
	bool IsActivated();

	void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};