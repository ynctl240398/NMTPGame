#include "CBrickP.h"

#define STATE_BRICK_P_BRICK			11900
#define STATE_BRICK_P_IDLE			11901
#define STATE_BRICK_P_PUSHED		11902
#define STATE_BRICK_P_BRICK_UP		11903
#define STATE_BRICK_P_BRICK_BREAK	11904

void CBrickP::_RenderBrickBroken()
{
}

int CBrickP::_GetAnimationId()
{
	return 0;
}

void CBrickP::SetState(int)
{
}

void CBrickP::Render()
{
}

void CBrickP::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CBrickP::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
