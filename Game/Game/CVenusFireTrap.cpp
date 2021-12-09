#include "CVenusFireTrap.h"
#include "CAnimation.h"

#define ID_ANI_VENUS_FIRE_RED_UP			8000
#define ID_ANI_VENUS_FIRE_RED_UP_FIRER		8001
#define ID_ANI_VENUS_FIRE_RED_DOWN			8002
#define ID_ANI_VENUS_FIRE_RED_DOWN_FIRER	8003
#define ID_ANI_VENUS_FIRE_GREEN_UP			8004
#define ID_ANI_VENUS_FIRE_GREEN_UP_FIRER	8005
#define ID_ANI_VENUS_FIRE_GREEN_DOWN		8006
#define ID_ANI_VENUS_FIRE_GREEN_DOWN_FIRER	8007

#define OFFSET_Y							32.0f

CVenusFireTrap::CVenusFireTrap(float x, float y, int type)
{
	_position = { x,y };
	_startPostion = _position;
	_type = type;
	_state = STATE_VENUS_FIRE_TRAP_UP;
}

void CVenusFireTrap::SetState(int)
{
}

void CVenusFireTrap::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = _GetAnimationId();
	LPANIMATION animation = animations->Get(aniId);
	if (animation == NULL) {
		return;
	}

	animations->Get(aniId)->Render(_position.x, _position.y, _scale);
	RenderBoundingBox();
}

int CVenusFireTrap::_GetAnimationId()
{
	int aniId = -1;

	switch (_state)
	{
	case STATE_VENUS_FIRE_TRAP_UP_FIRER:
		aniId = _type == TYPE_VENUS_FIRE_TRAP_RED ? ID_ANI_VENUS_FIRE_RED_UP_FIRER : ID_ANI_VENUS_FIRE_GREEN_UP_FIRER;
		break;
	case STATE_VENUS_FIRE_TRAP_DOWN_FIRER:
		aniId = _type == TYPE_VENUS_FIRE_TRAP_RED ? ID_ANI_VENUS_FIRE_RED_DOWN_FIRER : ID_ANI_VENUS_FIRE_GREEN_DOWN_FIRER;
		break;
	case STATE_VENUS_FIRE_TRAP_UP:
		aniId = _type == TYPE_VENUS_FIRE_TRAP_RED ? ID_ANI_VENUS_FIRE_RED_UP : ID_ANI_VENUS_FIRE_GREEN_UP;
		break;
	case STATE_VENUS_FIRE_TRAP_DOWN:
		aniId = _type == TYPE_VENUS_FIRE_TRAP_RED ? ID_ANI_VENUS_FIRE_RED_DOWN : ID_ANI_VENUS_FIRE_GREEN_DOWN;
		break;
	default:
		break;
	}

	return aniId;
}

void CVenusFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CVenusFireTrap::Release()
{
}

void CVenusFireTrap::OnNoCollision(DWORD dt)
{
}

void CVenusFireTrap::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CVenusFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (_state != STATE_VENUS_FIRE_TRAP_DIE || _state != STATE_VENUS_FIRE_TRAP_IDLE) {
		left = _position.x - VENUS_FIRE_TRAP_BBOX_WIDTH / 2;
		top = _position.y - VENUS_FIRE_TRAP_BBOX_HIEGHT / 2;
		right = left + VENUS_FIRE_TRAP_BBOX_WIDTH;
		bottom = top + VENUS_FIRE_TRAP_BBOX_HIEGHT;
	}
}
