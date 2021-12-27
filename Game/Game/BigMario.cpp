#include "BigMario.h"
#include "CMario.h"
#include "CItem.h"

void BigMario::_OnCollisionWithItem(LPCOLLISIONEVENT e)
{
	CItem* item = dynamic_cast<CItem*>(e->obj);
	if (item->GetType() == TYPE_ITEM_MUSHROOM_RED) {
		mario->marioState = MarioState::Raccoon;
	}
}

void BigMario::_OnDamaged(LPCOLLISIONEVENT e, float damage)
{
	if (damage <= 0 || mario->_untouchable) return;
	if (damage <= 1) {
		mario->marioState = MarioState::Small;
	}
	else {
		mario->_Die();
	}
}

BigMario::BigMario(CMario* mario) : BaseMario(mario, MarioState::Big)
{
	_level = LEVEL_BIG;
}

void BigMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	int width = mario->walkState == MarioWalkState::Sit ? MARIO_BIG_SITTING_BBOX_WIDTH : MARIO_BIG_BBOX_WIDTH;
	int height = mario->walkState == MarioWalkState::Sit ? MARIO_BIG_SITTING_BBOX_HEIGHT : MARIO_BIG_BBOX_HEIGHT;

	mario->GetPosition(left, top);
	left -= width / 2;
	top -= height / 2;
	right = left + width;
	bottom = top + height;


}
