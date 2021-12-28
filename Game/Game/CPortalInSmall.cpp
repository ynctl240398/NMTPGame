#include "CPortalInSmall.h"
#include "CMario.h"
#include "CDebug.h"

CPortalInSmall::CPortalInSmall(float x, float y, float w, float h, int direction, float cx, float cy, int camBoundId)
{
	_position = { x,y };
	_cy = cy;
	_cx = cx;
	_w = w;
	_h = h;
	_direction = direction;
	_running = false;
	_camBoundId = camBoundId;
}

void CPortalInSmall::Update(DWORD dt, vector<CGameObject*>* coObjects)
{
	CKeyBoardCustom* kb = CKeyBoardCustom::GetInstance();
	CMario* mario = CMario::GetInstance();

	float l, t, r, b;
	float ml, mt, mr, mb;
	GetBoundingBox(l, t, r, b);
	mario->GetBoundingBox(ml, mt, mr, mb);

	if ((_direction == 1 && kb->IsKeyDown(DIK_DOWN)) || (_direction == -1 && kb->IsKeyDown(DIK_UP))) {
		if (CCollision::GetInstance()->IsOverlap(l, t, r, b, ml, mt, mr, mb)) {
			_running = true;
		}
	}
	if (_running) {
		mario->_tele = true;
		D3DXVECTOR2 pos = mario->GetPosition();
		pos.x = _position.x;
		pos.y += 0.7 * _direction;
		mario->SetPosition(pos);
		mario->SetVelocity({ 0, 0 });

		mario->GetBoundingBox(ml, mt, mr, mb);
		bool end = false;
		if (_direction > 0) {
			if (mt >= t) {
				end = true;
			}
		}
		else {
			if (mb <= b) {
				end = true;
			}
		}
		if (end) {
			_running = false;
			mario->_tele = false;
			mario->SetPosition({ _cx, _cy });
			mario->walkState = MarioWalkState::Idle;
			CCam::GetInstance()->ActiveCameraBound(_camBoundId);
		}
	}
}

void CPortalInSmall::Render()
{
	RenderBoundingBox();
}

void CPortalInSmall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _position.x - _w / 2;
	top = _position.y - _h / 2;
	right = left + _w;
	bottom = top + _h;
}

CPortalInSmall::~CPortalInSmall()
{
}
