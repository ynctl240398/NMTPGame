#include "RaccoonMario.h"
#include "CMario.h"

void RaccoonMario::_JumpUpdate(DWORD dt)
{
	if (!mario->controllable) {
		return;
	}

	float vx, vy;
	mario->GetVelocity(vx, vy);
	float x, y;
	mario->GetPosition(x, y);
	CKeyBoardCustom* kb = CKeyBoardCustom::GetInstance();

	_flyTimer.Update(dt);

	if (mario->IsOnGround()) {
		mario->jumpState = MarioJumpState::Idle;

		if (kb->IsKeyPressed(DIK_S)) {
			mario->jumpState = MarioJumpState::Jump;
			mario->SetOnGround(false);
			mario->_jumpStartHeight = y;
		}
	}

	if (kb->IsKeyPressed(DIK_S)) {
		if (mario->powerMeter >= PMETER_MAX) {
			mario->jumpState = MarioJumpState::Fly;
			mario->_jumpStartHeight = y;

			//open cam bound
		}
	}

	float jumpHeight = MARIO_JUMP_HEIGHT;
	float minJumpHeight = MARIO_MIN_JUMP_HEIGHT;
	float height = 0;

	switch (mario->jumpState)
	{
	case MarioJumpState::Fly:
		height = abs(mario->_jumpStartHeight - y - vy * dt);
		minJumpHeight = MARIO_MIN_HIGH_JUMP_HEIGHT;

		if (height < minJumpHeight || (height < MARIO_SUPER_JUMP_HEIGHT && kb->IsKeyDown(DIK_S))) {
			vy = -MARIO_FLYING_UP_FORCE - MARIO_GRAVITY * dt;
		}
		else {
			mario->jumpState = MarioJumpState::Fall;
			vy = -MARIO_FLYING_UP_FORCE / 2;
		}
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
		vy = min(vy, MARIO_FLOATING_SPEED);
		if (!_flyTimer.IsRunning() || _flyTimer.IsTimeOver()) {
			mario->jumpState = MarioJumpState::Fall;
		}
	case MarioJumpState::Fall:	
		if (kb->IsKeyPressed(DIK_S)) {
			mario->jumpState = MarioJumpState::Float;
			_flyTimer.Reset();
			_flyTimer.Start();
		}
		break;
	}

	mario->SetVelocity({ vx, vy });
}

void RaccoonMario::_PowerMeterUpdate(DWORD dt)
{
	float vx, vy;
	mario->GetVelocity(vx, vy);
	CKeyBoardCustom* kb = CKeyBoardCustom::GetInstance();

	float maxRun = abs(vx) > MARIO_RUN_SPEED * 0.85f;

	_pmeterTimer.Update(dt);

	if (!_pmeterTimer.IsRunning()) {
		if (maxRun && mario->IsOnGround())
			mario->powerMeter = max(0.0f, min(mario->powerMeter + PMETER_UP_STEP * dt, PMETER_MAX + 1));
		else if (mario->powerMeter > 0)
			mario->powerMeter = max(0.0f, min(mario->powerMeter - PMETER_DOWN_STEP * dt, PMETER_MAX));

		if (mario->powerMeter >= PMETER_MAX && kb->IsKeyPressed(DIK_S)) {
			_pmeterTimer.Reset();
			_pmeterTimer.Start();
		}
	}
	else {
		if (_pmeterTimer.IsTimeOver()) {
			mario->powerMeter = 0;
			_pmeterTimer.Stop();
		}
	}
}

void RaccoonMario::_AttackUpdate(DWORD dt)
{
	_attackTimer.Update(dt);

	CKeyBoardCustom* kb = CKeyBoardCustom::GetInstance();
	if (kb->IsKeyPressed(DIK_A)) {
		if (!_attackTimer.IsRunning() || _attackTimer.IsTimeOver()) {
			_attackTimer.Reset();
			_attackTimer.Start();
		}
	}
}

void RaccoonMario::_GetJumpAnimationId(int& aniId)
{
	if (mario->IsOnGround()) {
		return;
	}

	aniId = ID_ANI_MARIO_IDLE;

	switch (mario->jumpState)
	{
	case MarioJumpState::Fly:
		aniId = ID_ANI_MARIO_JUMP_RUN;
		break;
	case MarioJumpState::Float:
		aniId = ID_ANI_MARIO_FLY;
		break;
	case MarioJumpState::Fall:
		aniId = ID_ANI_MARIO_JUMP_WALK;
		break;
	case MarioJumpState::Jump:
		aniId = ID_ANI_MARIO_JUMP_WALK;
		break;
	case MarioJumpState::HighJump:
		aniId = ID_ANI_MARIO_JUMP_WALK;
		break;
	default:
		break;
	}

	if (mario->hand) {
		//hold

	}
}

void RaccoonMario::_GetAttackAnimationId(int& aniId)
{
	if (_attackTimer.IsRunning() && !_attackTimer.IsTimeOver()) {
		aniId = ID_ANI_MARIO_ATTACK;
	}
}

RaccoonMario::RaccoonMario(CMario* mario) : BaseMario(mario, MarioState::Raccoon)
{
	_level = LEVEL_SUPER;

	this->MARIO_SUPER_JUMP_HEIGHT = 27;
	this->MAX_FLY_SPEED = 0.135f;
}

void RaccoonMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	int width = mario->walkState == MarioWalkState::Sit ? MARIO_SUPPER_SITTING_BBOX_WIDTH : MARIO_SUPPER_BBOX_WIDTH;
	int height = mario->walkState == MarioWalkState::Sit ? MARIO_SUPPER_SITTING_BBOX_HEIGHT : MARIO_SUPPER_BBOX_HEIGHT;

	mario->GetPosition(left, top);
	left -= width / 2;
	top -= height / 2;
	right = left + width;
	bottom = top + height;
}
