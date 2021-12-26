#pragma once
#include "BaseMario.h"
class BigMario :
    public BaseMario
{

public:
    BigMario(CMario* mario);

    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

