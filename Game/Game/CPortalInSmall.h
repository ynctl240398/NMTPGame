#pragma once

#include "CGameObject.h"

class CPortalInSmall : public CGameObject
{

	int _direction; // down -> 1, up -> -1

	float _cx;
	float _cy;

	bool _running;

	int _camBoundId;

public:
	float _w;
	float _h;

	CPortalInSmall(float x, float y, float w, float h, int direction, float cx, float cy, int camBoundId);

	void Update(DWORD, vector<CGameObject*>* = nullptr) override;
	void Render() override;

	virtual int IsBlocking(LPCOLLISIONEVENT e) { return 0; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;

	~CPortalInSmall();
};

