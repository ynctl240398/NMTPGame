#pragma once
#include "CGameObject.h"
#include "CTimer.h"
class CAniObject :
    public CGameObject
{
protected:
    CTimer timer{ true, 1 };
    float w, h;

public:
    CAniObject(D3DXVECTOR2 position, float vx, float vy, int aniId, D3DXVECTOR2 scale = {1, 1}, long timeout = 2000, float gravity = 0.001f, int w = 8, int h = 8);

    void Render() override;
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

