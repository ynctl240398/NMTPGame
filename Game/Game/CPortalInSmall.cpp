#include "CPortalInSmall.h"

CPortalInSmall::CPortalInSmall(float x, float y, int direction, float cx, float cy)
{
	_position = { x,y };
	_cy = cy;
	_cx = cx;
	_direction = direction;
}

void CPortalInSmall::Update(DWORD, vector<CGameObject*>*)
{
}

void CPortalInSmall::Render()
{
	RenderBoundingBox();
}

void CPortalInSmall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _position.x - PORTAL_WIDTH / 2;
	right = left + PORTAL_WIDTH;
	top = _position.y - PORTAL_HEIGHT / 2;
	bottom = top + PORTAL_WIDTH;
}

CPortalInSmall::~CPortalInSmall()
{
}
