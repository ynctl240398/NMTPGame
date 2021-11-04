#include "CPlatform.h"

#include "CSprite.h"
#include "CTexture.h"

#include "CCam.h"

void CPlatform::RenderBoundingBox()
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

	float xx = _position.x - this->cellWidth / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, _position.y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CPlatform::Render()
{
	if (this->length <= 0) return;
	float xx = _position.x;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdBegin)->Draw(xx, _position.y, { 1.0f,1.0f }, 255);
	xx += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddle)->Draw(xx, _position.y, { 1.0f,1.0f }, 255);
		xx += this->cellWidth;
	}
	if (length > 1)
		s->Get(this->spriteIdEnd)->Draw(xx, _position.y, { 1.0f,1.0f }, 255);

	RenderBoundingBox();
}

void CPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = _position.x - cellWidth_div_2;
	t = _position.y - this->cellHeight / 2;
	r = l + this->cellWidth * this->length;
	b = t + this->cellHeight;
}

void CPlatform::Release() {

}

void CPlatform::Load(LPCWSTR filePath) {

}