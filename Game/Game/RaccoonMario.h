#pragma once
#include "BaseMario.h"
class RaccoonMario :
    public BaseMario
{

public:
    RaccoonMario(CMario* mario);

    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

