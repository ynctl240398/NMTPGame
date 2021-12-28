#include "CMario.h"
#include "BaseMario.h"
#include "SmallMario.h"
#include "BigMario.h"
#include "RaccoonMario.h"
#include "CAniObject.h"

CMario* CMario::__instance = NULL;

void CMario::_UpdateState()
{
	if (stateMachine != NULL && stateMachine->GetStateID() == marioState) {
		return;
	}

	float l, t, r, b;
	float nl, nt, nr, nb;
	bool update = false;
	if (stateMachine != nullptr) {
		GetBoundingBox(l, t, r, b);
		update = true;
	}

	_untouchableTimer.Reset();
	_untouchableTimer.Start();

	switch (marioState)
	{
	case MarioState::Small:
		stateMachine = new SmallMario(this);
		break;
	case MarioState::Big:
		stateMachine = new BigMario(this);
		break;
	case MarioState::Raccoon:
		stateMachine = new RaccoonMario(this);
		break;
	default:
		stateMachine = new SmallMario(this);
		break;
	}

	if (update) {
		GetBoundingBox(nl, nt, nr, nb);
		_position.y -= nb - b;
	}
}

void CMario::_CheatPointUpdate()
{
	CKeyBoardCustom* kb = CKeyBoardCustom::GetInstance();
	if (kb->IsKeyPressed(DIK_1)) {
		marioState = MarioState::Small;
	}
	if (kb->IsKeyPressed(DIK_2)) {
		marioState = MarioState::Big;
	}
	if (kb->IsKeyPressed(DIK_3)) {
		marioState = MarioState::Raccoon;
	}
	if (kb->IsKeyPressed(DIK_E)) {
		_position.x = 2200;
		_position.y = 515;
	}
	if (kb->IsKeyPressed(DIK_Q)) {
		_position.x = 0;
		_position.y = 350;
		CCam::GetInstance()->LockTop();
	}
	if (kb->IsKeyPressed(DIK_W)) {
		_position.x = 1324;
		_position.y = 122;
		CCam::GetInstance()->UnlockTop();
	}
	if (kb->IsKeyPressed(DIK_R)) {
		_position.x = 2268;
		_position.y = 80;
		CCam::GetInstance()->UnlockTop();
	}
	if (kb->IsKeyPressed(DIK_END)) {
		_Die();
	}
}

void CMario::_Die()
{
	hand = nullptr;
	_isActive = false;
	int _aniId = ID_ANI_MARIO_DIE + (LEVEL_SMALL % 100) * 100;
	CAniObject* aniObj = new CAniObject(_position, 0, -0.3f, _aniId, { 1, 1 });
	CGame::GetInstance()->GetCurrentScene()->SpawnAniObject(aniObj);
}

void CMario::_SheldUpdate(DWORD dt)
{
	if (hand == nullptr) return;
	CKeyBoardCustom* kb = CKeyBoardCustom::GetInstance();
	if (kb->IsKeyDown(DIK_A)) {
		D3DXVECTOR2 pos = _position;
		pos += {12 * _direction, 0};
		hand->SetVelocity({ 0, -0.0008f * dt });
		hand->SetPosition(pos);
	}
	else {
		D3DXVECTOR2 pos = _position;
		pos += {18 * _direction, -2};
		hand->SetVelocity({ 0, 0 });
		hand->SetPosition(pos);

		_velocity.x += 0.1 * _direction;
		hand = nullptr;
	}
}

CMario* CMario::GetInstance()
{
	if (__instance == NULL) {
		__instance = new CMario;
	}
	return __instance;
}

CMario::CMario()
{
	_UpdateState();
	_kickTimer = new CTimer(true, 150);
	_isActive = true;
	hand = nullptr;
	_untouchableTimer.Stop();
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!_isActive) return;

	_UpdateState();
	_CheatPointUpdate();
	_SheldUpdate(dt);

	_kickTimer->Update(dt);
	_untouchableTimer.Update(dt);

	if (_untouchableTimer.GetState() != CTimerState::RUNNING) {
		_untouchable = false;
	}
	else {
		_untouchable = true;
	}

	stateMachine->_hasQBrick = false;

	if (_tele) return;

	stateMachine->Update(dt, coObjects);
}

void CMario::Render() {
	if (!_isActive) return;

	stateMachine->Render();
}

void CMario::SetOnGround(bool og)
{
	onGround = og;
}

bool CMario::IsOnGround()
{
	return onGround;
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	stateMachine->GetBoundingBox(left, top, right, bottom);
}

void CMario::OnNoCollision(DWORD dt) {
	stateMachine->OnNoCollision(dt);
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	stateMachine->OnCollisionWith(e);
}

void CMario::OnBlockingOn(bool isHorizontal, float z)
{
	stateMachine->OnBlockingOn(isHorizontal, z);
}

int CMario::IsBlocking(LPCOLLISIONEVENT e) {
	return 0;
}
