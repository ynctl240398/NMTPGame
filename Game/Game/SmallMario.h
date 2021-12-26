#pragma once
#include "BaseMario.h"

class SmallMario :
    public BaseMario
{

public:
    SmallMario(CMario* mario);

    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

