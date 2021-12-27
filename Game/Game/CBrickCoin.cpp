#include "CBrickCoin.h"
#include "CKeyBoard.h"
#include "CMario.h"
#include "CTail.h"
#include "CAniObject.h"

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

void CBrickCoin::_SpawnBreakEffect()
{
    CAniObject* piece1 = new CAniObject(_position, -0.05, -0.2, ID_ANI_PARTICLES_BRICK_BROKEN_IDLE);
    CAniObject* piece3 = new CAniObject(_position, -0.05, -0.1, ID_ANI_PARTICLES_BRICK_BROKEN_IDLE);
    CAniObject* piece2 = new CAniObject(_position, 0.05, -0.2, ID_ANI_PARTICLES_BRICK_BROKEN_IDLE);
    CAniObject* piece4 = new CAniObject(_position, 0.05, -0.1, ID_ANI_PARTICLES_BRICK_BROKEN_IDLE);

    CGame::GetInstance()->GetCurrentScene()->SpawnAniObject(piece1);
    CGame::GetInstance()->GetCurrentScene()->SpawnAniObject(piece2);
    CGame::GetInstance()->GetCurrentScene()->SpawnAniObject(piece3);
    CGame::GetInstance()->GetCurrentScene()->SpawnAniObject(piece4);
}

CBrickCoin::CBrickCoin(float x, float y, bool brick)
{
    this->_position = { x, y };
    this->_velocity = { 0, 0 };
    this->_ax = 0;
    this->_ay = 0;
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
    if (_brick) {
        if (dynamic_cast<CTail*>(e->obj)) {
            /*CTail* tail = dynamic_cast<CTail*>(e->obj);
            float vx, vy;
            tail->GetVelocity(vx, vy);
            DebugOut(L"Tail: vx: %f\tvy: %f\n", vx, vy);*/

            _isDeleted = true;
            _SpawnBreakEffect();
        }
        if (e->ny < 0) {
            if (dynamic_cast<CMario*>(e->obj)) {
                _isDeleted = true;
                _SpawnBreakEffect();
            }
        }
    }
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
