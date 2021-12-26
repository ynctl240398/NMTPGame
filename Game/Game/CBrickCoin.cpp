#include "CBrickCoin.h"
#include "CKeyBoard.h"
#include "CMario.h"

int CBrickCoin::_GetAnimationId()
{
    if (_brick) {
        return ID_ANI_BRICK_COIN_BRICK;
    }
    else {
        return ID_ANI_BRICK_COIN_COIN;
    }
}

void CBrickCoin::_TranformUpdate(DWORD dt)
{
    transferTimer.Update(dt);
    if (transferTimer.GetState() == CTimerState::TIMEOVER) {
        _brick = !_brick;
        transferTimer.Stop();
    }
}

CBrickCoin::CBrickCoin(float x, float y, bool brick)
{
    this->_position.x = x;
    this->_position.y = y;
    this->_brick = brick;
}

void CBrickCoin::Render()
{
    _aniId = _GetAnimationId();
    CGameObject::Render();
}

void CBrickCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (CKeyBoardCustom::GetInstance()->IsKeyPressed(DIK_B)) {
        Transform();
    }

    _TranformUpdate(dt);
    if (_brick) {
        CCollision::GetInstance()->Process(this, dt, coObjects);
    }
    else {
        float ml, mt, mr, mb, sl, st, sr, sb;
        GetBoundingBox(ml, mt, mr, mb);
        CMario::GetInstance()->GetBoundingBox(sl, st, sr, sb);

        if (CCollision::IsOverlap(ml, mt, mr, mb, sl, st, sr, sb)) {
            _isDeleted = true;
        }
    }
}

void CBrickCoin::OnCollisionWith(LPCOLLISIONEVENT e)
{
    //tail
}

int CBrickCoin::IsBlocking(LPCOLLISIONEVENT e)
{
    return _brick;
}

void CBrickCoin::Transform()
{
    _brick = !_brick;
    transferTimer.Reset();
    transferTimer.Start();
}

void CBrickCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = _position.x - BRICK_COIN_WIDTH / 2;
    top = _position.y - BRICK_COIN_HEIGHT / 2;
    right = left + BRICK_COIN_WIDTH;
    bottom = top + BRICK_COIN_HEIGHT;
}
