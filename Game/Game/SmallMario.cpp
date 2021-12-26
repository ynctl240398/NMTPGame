#include "SmallMario.h"
#include "CMario.h"

SmallMario::SmallMario(CMario* mario) : BaseMario(mario, MarioState::Small)
{
}

void SmallMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	mario->GetPosition(left, top);
	left -= MARIO_SMALL_BBOX_WIDTH / 2;
	top -= MARIO_SMALL_BBOX_HEIGHT / 2;
	right = left + MARIO_SMALL_BBOX_WIDTH;
	bottom = top + MARIO_SMALL_BBOX_HEIGHT;
}
