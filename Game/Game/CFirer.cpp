#include "CFirer.h"
#include "CAnimation.h"
#include "CMario.h"
#include "CDebug.h"

#define ID_ANI_FIRER		10000

#define FIRER_FLY_SPEED		0.05f
#define FIRER_JUMP_SPEED_X	0.02f
#define FIRER_JUMP_SPEED_Y	0.02f

#define FIRER_GRAVITY		0.0002f

#define FIRER_BBOX_WIDTH	8
#define FIRER_BBOX_HEIGHT	8

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
	_state = -1;
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
	_aniId = _GetAnimationId();
	CGameObject::Render();
}

void CFirer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void CFirer::OnNoCollision(DWORD dt)
{
	//_position.x += _velocity.x * dt;
	//_position.y += _velocity.y * dt;
}

void CFirer::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (dynamic_cast<CMario*>(e->obj) && e->obj->IsBlocking(e)) {
	//	CMario* mario = dynamic_cast<CMario*>(e->obj);
	//	int levelMario = mario->GetLevel();
	//	if (levelMario > LEVEL_SMALL)
	//	{
	//		mario->SetLevel(--levelMario);
	//		mario->StartUntouchable();
	//	}
	//	else
	//	{
	//		DebugOut(L">>> Mario DIE >>> \n");
	//		mario->SetState(STATE_MARIO_DIE);
	//	}
	//}
	_handleNoCollisionX = true;
	_handleNoCollisionY = true;
}

void CFirer::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _position.x - FIRER_BBOX_WIDTH / 2;
	top = _position.y - FIRER_BBOX_HEIGHT / 2;
	right = left + FIRER_BBOX_WIDTH;
	bottom = top + FIRER_BBOX_HEIGHT;
}
