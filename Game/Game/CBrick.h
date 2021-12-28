#pragma once

#include "CGameObject.h"
#include "CSprite.h"
#include "CAnimation.h"
#include "CItem.h"

class CBrick : public CGameObject
{
private:
	int _width;
	int _height;
public:
	CBrick(float x, float y, int w, int h);

	int IsBlocking(LPCOLLISIONEVENT e) {
		if (dynamic_cast<CItem*>(e->src_obj)) {
			CItem* item = dynamic_cast<CItem*>(e->src_obj);
			if (item->GetType() == TYPE_ITEM_LEAF) {
				return 0;
			}
		}
		return 1;
	}

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};