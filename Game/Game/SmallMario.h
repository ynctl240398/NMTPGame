#pragma once
#include "BaseMario.h"

class SmallMario :
    public BaseMario
{
protected:
    void _OnCollisionWithItem(LPCOLLISIONEVENT e);
    void _OnDamaged(LPCOLLISIONEVENT e, float damage) override;

public:
    SmallMario(CMario* mario);

    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

