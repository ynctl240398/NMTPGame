#include "CPlatform.h"

#include "CSprite.h"
#include "CTexture.h"

#include "CCam.h"

void CPlatform::Render()
{
	RenderBoundingBox();
}

void CPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = _position.x - _width / 2;
	t = _position.y - _height / 2;
	r = l + _width;
	b = t + _height;
}

int CPlatform::IsBlocking(LPCOLLISIONEVENT e)
{
	return e->ny < 0;
}

void CPlatform::Release() {

}

void CPlatform::Load(LPCWSTR filePath) {

}