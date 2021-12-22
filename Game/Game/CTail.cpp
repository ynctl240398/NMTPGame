#include "CTail.h"
#include "CMario.h"
#include "CGoomba.h"

CTail::CTail(float x, float y)
{
	_position = { x,y };
	_isShow = false;
	_velocity = { 0.5f,0.0f };
}

void CTail::Render()
{
	if (!_isShow) return;

	RenderBoundingBox();
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!_isShow) return;

	CMario* mario = CMario::GetInstance();
	if (mario->GetDirection() == DIRECTION_LEFT) {
		_position.x = mario->GetPosition().x + mario->GetVelocity().x * dt - MARIO_SUPPER_BBOX_WIDTH / 2;
	}
	else _position.x = mario->GetPosition().x + mario->GetVelocity().x * dt + MARIO_SUPPER_BBOX_WIDTH / 2;
	_position.y = mario->GetPosition().y + mario->GetVelocity().y * dt + MARIO_TAIL_BBOX_HEIGHT;
	CGameObject::Update(dt, coObjects);
}

void CTail::OnNoCollision(DWORD dt)
{
}

void CTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CGoomba*>(e->obj)) {
		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
		goomba->SetState(STATE_GOOMBA_DIE_JUMP);
	}
}

void CTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (_isShow) {
		left = _position.x - MARIO_TAIL_BBOX_WIDTH / 2;
		top = _position.y - MARIO_TAIL_BBOX_HEIGHT / 2;
		right = left + MARIO_TAIL_BBOX_WIDTH;
		bottom = top + MARIO_TAIL_BBOX_HEIGHT;
	}
}
