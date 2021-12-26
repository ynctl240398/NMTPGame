#include "BigMario.h"
#include "CMario.h"

BigMario::BigMario(CMario* mario) : BaseMario(mario, MarioState::Big)
{
	_level = LEVEL_BIG;
}

void BigMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	int width = mario->walkState == MarioWalkState::Sit ? MARIO_BIG_SITTING_BBOX_WIDTH : MARIO_BIG_BBOX_WIDTH;
	int height = mario->walkState == MarioWalkState::Sit ? MARIO_BIG_SITTING_BBOX_WIDTH : MARIO_BIG_BBOX_HEIGHT;

	mario->GetPosition(left, top);
	left -= width / 2;
	top -= height / 2;
	right = left + width;
	bottom = top + height;


}
