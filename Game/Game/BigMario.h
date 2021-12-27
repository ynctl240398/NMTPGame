#pragma once
#include "BaseMario.h"
class BigMario :
    public BaseMario
{
protected:
    void _OnCollisionWithItem(LPCOLLISIONEVENT e);

public:
    BigMario(CMario* mario);

    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

