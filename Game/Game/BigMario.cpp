#include "BigMario.h"
#include "CMario.h"

BigMario::BigMario(CMario* mario) : BaseMario(mario, MarioState::Big)
{
}

void BigMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	mario->GetPosition(left, top);
	left -= MARIO_BIG_BBOX_WIDTH / 2;
	top -= MARIO_BIG_BBOX_HEIGHT / 2;
	right = left + MARIO_BIG_BBOX_WIDTH;
	bottom = top + MARIO_BIG_BBOX_HEIGHT;
}
