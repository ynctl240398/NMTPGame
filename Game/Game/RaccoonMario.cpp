#include "RaccoonMario.h"
#include "CMario.h"

RaccoonMario::RaccoonMario(CMario* mario) : BaseMario(mario, MarioState::Raccoon)
{
}

void RaccoonMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	float width = mario->walkState == MarioWalkState::Sit ? MARIO_SUPPER_SITTING_BBOX_WIDTH : MARIO_SUPPER_BBOX_WIDTH;
	float height = mario->walkState == MarioWalkState::Sit ? MARIO_SUPPER_SITTING_BBOX_HEIGHT : MARIO_SUPPER_BBOX_HEIGHT;

	mario->GetPosition(left, top);
	left -= width / 2;
	top -= height / 2;
	right = left + width;
	bottom = top + height;
}
