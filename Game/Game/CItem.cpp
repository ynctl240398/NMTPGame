#include "CItem.h"
#include "CUtil.h"

CItem::CItem(float x, float y, string type) {
	_position = { x,y };
	_isActive = false;
	_type = type;
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

}

void CItem::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	int aniId;
	if (_type == TYPE_MUSHROOM_RED) {
		aniId = ID_ITEM_MUSHROOM_RED_ANI;
	}
	else aniId = ID_ITEM_MUSHROOM_GREEN_ANI;

	LPANIMATION animation = animations->Get(aniId);

	if (animation == NULL) {
		return;
	}

	animations->Get(aniId)->Render(_position.x, _position.y, _scale);
	RenderBoundingBox();
}

void CItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = _position.x - ITEM_WIDTH / 2;
	top = _position.y - ITEM_HEIGHT / 2;
	right = left + ITEM_WIDTH;
	bottom = top + ITEM_HEIGHT;
}

void CItem::Release() {

}

CItem::~CItem() {

}