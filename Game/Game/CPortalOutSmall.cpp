#include "CPortalOutSmall.h"

CPortalOutSmall::CPortalOutSmall(float x, float y, int direction)
{
	_position = { x,y };
	_direction = direction;
}

void CPortalOutSmall::Update(DWORD, vector<CGameObject*>*)
{
}

void CPortalOutSmall::Render()
{
	RenderBoundingBox();
}

void CPortalOutSmall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _position.x - PORTAL_WIDTH / 2;
	right = left + PORTAL_WIDTH;
	top = _position.y - PORTAL_HEIGHT / 2;
	bottom = top + PORTAL_WIDTH;
}

CPortalOutSmall::~CPortalOutSmall()
{
}