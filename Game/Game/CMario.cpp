#include "CMario.h"
#include "BaseMario.h"
#include "SmallMario.h"
#include "BigMario.h"
#include "RaccoonMario.h"

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
		_position.x = 2409.;
		_position.y = 399;
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
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	_UpdateState();
	_CheatPointUpdate();

	stateMachine->Update(dt, coObjects);
}

void CMario::Render() {
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
