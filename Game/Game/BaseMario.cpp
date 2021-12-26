#include "BaseMario.h"
#include "CMario.h"
#include "CKeyBoard.h"

void BaseMario::_WalkUpdate(DWORD dt)
{
	if (!mario->controllable) {
		return;
	}

	float vx, vy;
	mario->GetVelocity(vx, vy);
	CKeyBoardCustom* kb = CKeyBoardCustom::GetInstance();

	if (kb->IsKeyDown(DIK_LEFT) || kb->IsKeyDown(DIK_RIGHT)) {
		int keySign = kb->IsKeyDown(DIK_LEFT) ? -1 : 1;

		if (mario->IsOnGround()) {
			mario->walkState = MarioWalkState::Walk;
		}
		mario->accelerate.x = MARIO_WALK_ACCELERATION * keySign;
		float maxSpeed = MARIO_WALK_SPEED;

		if (kb->IsKeyDown(DIK_A)) {
			if (mario->IsOnGround()) {
				mario->walkState = MarioWalkState::Run;
			}
			mario->accelerate.x = MARIO_RUN_ACCELERATION * keySign;
			maxSpeed = MARIO_RUN_SPEED;
		}

		if (mario->GetVelocity().x * keySign < 0) {
			mario->skid = 1;
			mario->accelerate.x = (kb->IsKeyDown(DIK_A) ? MARIO_SKID_ACCELERATION : MARIO_SKID_ACCELERATION * 0.5) * keySign;

			if (!mario->IsOnGround()) {
				mario->accelerate.x = MARIO_SKID_ACCELERATION * keySign * 2;
			}
		}

		vx += mario->accelerate.x * dt;

		if (mario->jumpState != MarioJumpState::Idle) 
			maxSpeed = min(maxSpeed, MAX_FLY_SPEED);

		if (abs(mario->GetVelocity().x) > maxSpeed) {
			int sign = mario->GetVelocity().x < 0 ? -1 : 1;
			if (abs(mario->GetVelocity().x) - maxSpeed > MARIO_RUN_DRAG_FORCE * dt) {
				vx -= MARIO_RUN_DRAG_FORCE * dt * sign;
			}
			else {
				vx = maxSpeed * sign;
			}
		}

		//skid end detect
		if (mario->GetVelocity().x * keySign >= 0) {
			mario->skid = 0;
		}

		mario->_direction = vx < 0 ? -1 : 1;
	}
	else {
		mario->skid = 0;

		if (abs(vx) > mario->_drag * dt) {
			int sign = mario->GetVelocity().x < 0 ? -1 : 1;
			vx -= mario->_drag * dt * sign;
		}
		else {
			vx = 0.0f;
			if (mario->walkState != MarioWalkState::Sit) {
				mario->walkState = MarioWalkState::Idle;
			}
		}
	}

	if (mario->walkState != MarioWalkState::Sit) {
		mario->_drag = mario->walkState == MarioWalkState::Run ? MARIO_RUN_DRAG_FORCE : MARIO_WALK_DRAG_FORCE;
	}
	mario->_drag *= mario->IsOnGround();

	mario->SetVelocity({ vx, vy });

	float maxRun = abs(vx) > MARIO_RUN_SPEED * 0.85f;

	if (maxRun && mario->IsOnGround())
		mario->powerMeter = max(0.0f, min(mario->powerMeter + PMETER_UP_STEP * dt, PMETER_MAX + 1));
	else if (mario->powerMeter > 0)
		mario->powerMeter = max(0.0f, min(mario->powerMeter - PMETER_DOWN_STEP * dt, PMETER_MAX));
}

void BaseMario::_JumpUpdate(DWORD dt)
{
	if (!mario->controllable) {
		return;
	}

	float vx, vy;
	mario->GetVelocity(vx, vy);
	float x, y;
	mario->GetPosition(x, y);
	CKeyBoardCustom* kb = CKeyBoardCustom::GetInstance();

	if (mario->IsOnGround()) {
		mario->jumpState = MarioJumpState::Idle;

		if (kb->IsKeyPressed(DIK_S)) {
			mario->jumpState = MarioJumpState::Jump;
			mario->SetOnGround(false);
			mario->_jumpStartHeight = y;
		}
	}

	float jumpHeight = MARIO_JUMP_HEIGHT;
	float minJumpHeight = MARIO_MIN_JUMP_HEIGHT;
	float height = 0;

	switch (mario->jumpState)
	{
		case MarioJumpState::Fly:
			break;
		case MarioJumpState::HighJump:
			jumpHeight = MARIO_HIGH_JUMP_HEIGHT;
			minJumpHeight = MARIO_MIN_HIGH_JUMP_HEIGHT;
		case MarioJumpState::Jump:
			height = abs(mario->_jumpStartHeight - y - vy * dt);

			if (height < minJumpHeight || (height < jumpHeight && kb->IsKeyDown(DIK_S))) {
				vy = -MARIO_PUSH_FORCE - MARIO_GRAVITY * dt;
			}
			else {
				if (kb->IsKeyDown(DIK_S) && mario->jumpState == MarioJumpState::Jump) {
					mario->jumpState = MarioJumpState::HighJump;
				}
				else {
					mario->jumpState = MarioJumpState::Fall;
					vy = -MARIO_PUSH_FORCE / 2;
				}
			}
			break;
		case MarioJumpState::Float:
		case MarioJumpState::Fall:
			break;
	}

	mario->SetVelocity({ vx, vy });
}

void BaseMario::_SitUpdate(DWORD dt)
{
}

void BaseMario::_PositionUpdate(DWORD dt)
{
	float vx, vy;
	mario->GetVelocity(vx, vy);

	vy += MARIO_GRAVITY * dt;

	mario->SetVelocity({ vx, vy });
}

BaseMario::BaseMario(CMario* mario, MarioState stateId)
{
	this->mario = mario;
	this->_stateId = stateId;
}

void BaseMario::AnimationInit()
{

}

void BaseMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	_SitUpdate(dt);
	_WalkUpdate(dt);
	_JumpUpdate(dt);
	_PositionUpdate(dt);

	CCollision::GetInstance()->Process(mario, dt, coObjects);

	D3DXVECTOR2 pos = mario->GetPosition();
	if (pos.x < 0) {
		mario->SetPosition({ 0, pos.y });
	}
}

void BaseMario::Render()
{
	mario->RenderBoundingBox();
}

void BaseMario::OnNoCollision(DWORD dt)
{
	float vx, vy;
	mario->GetVelocity(vx, vy);
	D3DXVECTOR2 pos = mario->GetPosition();

	pos.x += vx * dt;
	pos.y += vy * dt;

	mario->SetPosition(pos);
	mario->SetOnGround(false);
}

void BaseMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void BaseMario::OnBlockingOn(bool isHorizontal, float z)
{
	float vx, vy;
	mario->GetVelocity(vx, vy);
	D3DXVECTOR2 pos = mario->GetPosition();

	if (isHorizontal) {
		vx = 0;
	}
	else {
		vy = 0;
		if (z < 0) {
			mario->SetOnGround(true);
		}
		else {
			mario->jumpState = MarioJumpState::Fall;
		}
	}

	mario->SetVelocity({ vx, vy });
}

MarioState BaseMario::GetStateID()
{
	return _stateId;
}
