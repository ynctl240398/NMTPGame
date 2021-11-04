#include "CPortal.h"
#include "CGame.h"
#include "CTexture.h"

#include "CCam.h"

CPortal::CPortal(float l, float t, float r, float b, int scene_id)
{
	this->scene_id = scene_id;
	_position.x = l;
	_position.y = t;
	width = r - l;
	height = b - t;
}

void CPortal::RenderBoundingBox()
{
	D3DXVECTOR3 p(_position.x, _position.y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_IMG_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CCam::GetInstance()->GetPosition(cx, cy);

	CGame::GetInstance()->Draw(_position.x - cx, _position.y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CPortal::Render()
{
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = _position.x - width / 2;
	t = _position.y - height / 2;
	r = _position.x + width / 2;
	b = _position.y + height / 2;
}

void CPortal::Release() {

}

void CPortal::Load(LPCWSTR filePath) {

}