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
		_position.x = 2409;
		_position.y = 399;
	}
	if (kb->IsKeyPressed(DIK_END)) {
		_Die();
	}
}

void CMario::_Die()
{
	_isActive = false;
	int _aniId = ID_ANI_MARIO_DIE + (LEVEL_SMALL % 100) * 100;
	CAniObject* aniObj = new CAniObject(_position, 0, -0.3f, _aniId, { 1, 1 });
	CGame::GetInstance()->GetCurrentScene()->SpawnAniObject(aniObj);
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
	_untouchableTimer.Stop();
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!_isActive) return;

	_UpdateState();
	_CheatPointUpdate();

	_kickTimer->Update(dt);
	_untouchableTimer.Update(dt);

	if (_untouchableTimer.GetState() != CTimerState::RUNNING) {
		_untouchable = false;
	}
	else {
		_untouchable = true;
	}

	stateMachine->_hasQBrick = false;

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
