#include "CPortalOutSmall.h"
#include "CKeyBoard.h"
#include "CMario.h"

CPortalOutSmall::CPortalOutSmall(float x, float y, float w, float h, int direction)
{
	_position = { x,y };
	_w = w;
	_h = h;
	_direction = direction;
}

void CPortalOutSmall::Update(DWORD, vector<CGameObject*>*)
{
	CKeyBoardCustom* kb = CKeyBoardCustom::GetInstance();
	CMario* mario = CMario::GetInstance();

	float l, t, r, b;
	float ml, mt, mr, mb;
	GetBoundingBox(l, t, r, b);
	mario->GetBoundingBox(ml, mt, mr, mb);

	if (CCollision::IsOverlap(l, t, r, b, ml, mt, mr, mb)) {
		mario->_tele = true;
		D3DXVECTOR2 pos = mario->GetPosition();
		pos.x = _position.x;
		pos.y += 0.7 * _direction;
		mario->SetPosition(pos);
		mario->SetVelocity({ 0, 0 });

		mario->GetBoundingBox(ml, mt, mr, mb);

		if (!CCollision::IsOverlap(l, t, r, b, ml, mt, mr, mb)) {
			mario->_tele = false;
			mario->walkState = MarioWalkState::Idle;
			mario->_direction = 1;
		}
	}
}

void CPortalOutSmall::Render()
{
	RenderBoundingBox();
}

void CPortalOutSmall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _position.x - _w / 2;
	right = left + _w;
	top = _position.y - _h / 2;
	bottom = top + _h;
}

CPortalOutSmall::~CPortalOutSmall()
{
}