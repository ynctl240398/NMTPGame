#include "CFirer.h"
#include "CAnimation.h"
#include "CMario.h"

#define ID_ANI_FIRER		10000

#define FIRER_FLY_SPEED		0.05f
#define FIRER_JUMP_SPEED_X	0.02f
#define FIRER_JUMP_SPEED_Y	0.02f

#define FIRER_GRAVITY		0.0002f

int CFirer::_GetAnimationId()
{
	int aniId = -1;
	if (_state != STATE_FIRER_DISAPPEAR ) {
		aniId = ID_ANI_FIRER;
	}
	return aniId;
}

CFirer::CFirer(float x, float y)
{
	_position = { x,y };
}

void CFirer::SetState(int state) {
	switch (state)
	{
	case STATE_FIRER_FLY:
		if (CMario::GetInstance()->GetPosition().y > _position.y) {
			_velocity.y = FIRER_FLY_SPEED / 2;
		}
		else _velocity.y = -FIRER_FLY_SPEED / 2;
		if (CMario::GetInstance()->GetPosition().x > _position.x) {
			_velocity.x = FIRER_FLY_SPEED;
		}
		else _velocity.x = -FIRER_FLY_SPEED;
		break;
	case STATE_FIRER_JUMP:
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}

void CFirer::Render()
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

void CFirer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void CFirer::OnNoCollision(DWORD dt)
{
	_position.x += _velocity.x * dt;
	_position.y += _velocity.y * dt;
}

void CFirer::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CFirer::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
