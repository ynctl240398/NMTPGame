#include "SmallMario.h"
#include "CMario.h"
#include "CItem.h"

void SmallMario::_OnCollisionWithItem(LPCOLLISIONEVENT e)
{
	CItem* item = dynamic_cast<CItem*>(e->obj);
	if (item->GetType() == TYPE_ITEM_MUSHROOM_RED) {
		mario->marioState = MarioState::Big;
	}
}

SmallMario::SmallMario(CMario* mario) : BaseMario(mario, MarioState::Small)
{
	_level = LEVEL_SMALL;
}

void SmallMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	mario->GetPosition(left, top);
	left -= MARIO_SMALL_BBOX_WIDTH / 2;
	top -= MARIO_SMALL_BBOX_HEIGHT / 2;
	right = left + MARIO_SMALL_BBOX_WIDTH;
	bottom = top + MARIO_SMALL_BBOX_HEIGHT;
}
