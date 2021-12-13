#pragma once

#include "CGameObject.h"

class CBrickP : public CGameObject
{
	int _GetAnimationId();

public:
	CBrickP(float x, float y) {
		_position = { x,y };
	}

	void SetState(int) override;


	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;

	~CBrickP() {

	}
};

