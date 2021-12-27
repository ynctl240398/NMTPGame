#pragma once

#include "CGameObject.h"

class CPlatform : public CGameObject
{
protected:
	int _width;
	int _height;

public:
	CPlatform(float x, float y, int w, int h) :CGameObject()
	{
		h = 5;
		_position = { x + (w / 2) - DIF, y + h / 2 - DIF };
		_width = w;
		_height = h;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsBlocking(LPCOLLISIONEVENT e);
	void Release();
	void Load(LPCWSTR);
};

typedef CPlatform* LPPLATFORM;