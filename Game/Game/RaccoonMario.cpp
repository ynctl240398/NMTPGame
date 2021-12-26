#include "RaccoonMario.h"
#include "CMario.h"

RaccoonMario::RaccoonMario(CMario* mario) : BaseMario(mario, MarioState::Raccoon)
{
}

void RaccoonMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	mario->GetPosition(left, top);
	left -= MARIO_SUPPER_BBOX_WIDTH / 2;
	top -= MARIO_SUPPER_BBOX_HEIGHT / 2;
	right = left + MARIO_SUPPER_BBOX_WIDTH;
	bottom = top + MARIO_SUPPER_BBOX_HEIGHT;
}
