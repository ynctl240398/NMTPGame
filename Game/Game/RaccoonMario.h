#pragma once
#include "BaseMario.h"
#include "CTimer.h"

class RaccoonMario :
    public BaseMario
{
protected:
    virtual void _JumpUpdate(DWORD dt);
    virtual void _PowerMeterUpdate(DWORD dt);
    virtual void _AttackUpdate(DWORD dt);
    virtual void _GetJumpAnimationId(int& aniId);
    virtual void _GetAttackAnimationId(int& aniId);

    CTimer _attackTimer{ true, 250 };
    CTimer _flyTimer{ true, 250L };
    CTimer _pmeterTimer{ true, 4000L };

    float MARIO_ATTACK_DURATION = 250; 
    float MARIO_FLYING_UP_FORCE = 0.082f;
    float MARIO_FLOATING_SPEED = 0.02f;

public:
    RaccoonMario(CMario* mario);

    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

