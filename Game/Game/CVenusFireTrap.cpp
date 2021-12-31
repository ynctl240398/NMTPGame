#include "CVenusFireTrap.h"
#include "CAnimation.h"
#include "CMario.h"
#include "CTimer.h"
#include "CKoopaParaTropa.h"
#include "CKoopaTropa.h"
#include "CTail.h"

#define ID_ANI_VENUS_FIRE_TRAP_RED_UP			8000
#define ID_ANI_VENUS_FIRE_TRAP_RED_UP_FIRER		8001
#define ID_ANI_VENUS_FIRE_TRAP_RED_DOWN			8002
#define ID_ANI_VENUS_FIRE_TRAP_RED_DOWN_FIRER	8003
#define ID_ANI_VENUS_FIRE_TRAP_GREEN_UP			8004
#define ID_ANI_VENUS_FIRE_TRAP_GREEN_UP_FIRER	8005
#define ID_ANI_VENUS_FIRE_TRAP_GREEN_DOWN		8006
#define ID_ANI_VENUS_FIRE_TRAP_GREEN_DOWN_FIRER	8007

#define OFFSET_X_TO_UP		20

CVenusFireTrap::CVenusFireTrap(float x, float y, int type, float offSetY)
{
	_position = { x,y + VENUS_FIRE_TRAP_BBOX_HIEGHT + 2};
	_velocity = { 0, 0 };
	_offSetY = offSetY;
	_startPostion = _position;
	_type = type;
	_isUp = true;
	SetState(STATE_VENUS_FIRE_TRAP_IDLE);
	_firer = NULL;
}

void CVenusFireTrap::SetState(int state)
{
	switch (state)
	{
	case STATE_VENUS_FIRE_TRAP_UP_FIRER:
		break;
	case STATE_VENUS_FIRE_TRAP_DOWN_FIRER:
		break;
	case STATE_VENUS_FIRE_TRAP_UP:
		break;
	case STATE_VENUS_FIRE_TRAP_DOWN:
		break;
	case STATE_VENUS_FIRE_TRAP_IDLE:
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}

void CVenusFireTrap::Render()
{
	_aniId = _GetAnimationId();
	CGameObject::Render();
}

int CVenusFireTrap::_GetAnimationId()
{
	int aniId = -1;

	switch (_state)
	{
	case STATE_VENUS_FIRE_TRAP_UP_FIRER:
		aniId = _type == TYPE_VENUS_FIRE_TRAP_RED ? ID_ANI_VENUS_FIRE_TRAP_RED_UP_FIRER : ID_ANI_VENUS_FIRE_TRAP_GREEN_UP_FIRER;
		break;
	case STATE_VENUS_FIRE_TRAP_DOWN_FIRER:
		aniId = _type == TYPE_VENUS_FIRE_TRAP_RED ? ID_ANI_VENUS_FIRE_TRAP_RED_DOWN_FIRER : ID_ANI_VENUS_FIRE_TRAP_GREEN_DOWN_FIRER;
		break;
	case STATE_VENUS_FIRE_TRAP_UP:
		aniId = _type == TYPE_VENUS_FIRE_TRAP_RED ? ID_ANI_VENUS_FIRE_TRAP_RED_UP : ID_ANI_VENUS_FIRE_TRAP_GREEN_UP;
		break;
	case STATE_VENUS_FIRE_TRAP_DOWN:
		aniId = _type == TYPE_VENUS_FIRE_TRAP_RED ? ID_ANI_VENUS_FIRE_TRAP_RED_DOWN : ID_ANI_VENUS_FIRE_TRAP_GREEN_DOWN;
		break;
	default:
		break;
	}

	return aniId;
}

void CVenusFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float leftCam = CCam::GetInstance()->GetPosition().x;
	float rightCam = CGame::GetInstance()->GetBackBufferWidth() + leftCam;

	CMario* mario = CMario::GetInstance();

	bool _bPos = _position.x >= leftCam && _position.x <= rightCam;
	bool _bState = _state != STATE_VENUS_FIRE_TRAP_IDLE;
	bool _bPosMario = abs(_position.x - mario->GetPosition().x) > OFFSET_X_TO_UP + VENUS_FIRE_TRAP_BBOX_WIDTH;

	_startTimer.Update(dt);
	_fireTimer.Update(dt);

	if ((_bPos && _bPosMario)|| _bState) {
		if (mario->GetPosition().x < _position.x) {
			_scale = { 1.0f,1.0f };
		}
		else {
			_scale = { -1.0f,1.0f };
		}

		switch (_state)
		{
			case STATE_VENUS_FIRE_TRAP_IDLE:
				if (_startTimer.GetState() == CTimerState::STOPPED) {
					SetState(STATE_VENUS_FIRE_TRAP_UP);
				}
				if (_startTimer.GetState() == CTimerState::TIMEOVER) {
					SetState(STATE_VENUS_FIRE_TRAP_UP);
				}
				break;

			case STATE_VENUS_FIRE_TRAP_UP:
				if (_isUp) {
					if (_startPostion.y - _position.y < _offSetY) {
						_velocity.y = -VENUS_SPEED_Y;
					}
					else {
						_velocity.y = 0;
						if (mario->GetPosition().y < _position.y) {
							SetState(STATE_VENUS_FIRE_TRAP_UP_FIRER);
						}
						else {
							SetState(STATE_VENUS_FIRE_TRAP_DOWN_FIRER);
						}

						_fireTimer.Reset();
						_fireTimer.Start();
						_firer = NULL;
					}
				}
				else {
					if (_startPostion.y >= _position.y) {
						_velocity.y = VENUS_SPEED_Y;
					}
					else {
						_velocity.y = 0;
						SetState(STATE_VENUS_FIRE_TRAP_IDLE);
						_isUp = true;

						_startTimer.Reset();
						_startTimer.Start();
					}
				}
				break;

			case STATE_VENUS_FIRE_TRAP_UP_FIRER:
			case STATE_VENUS_FIRE_TRAP_DOWN_FIRER:
				if (_fireTimer.GetState() == CTimerState::TIMEOVER) {
					if (_firer == NULL) {
						if (mario->GetPosition().y < _position.y) {
							SetState(STATE_VENUS_FIRE_TRAP_UP_FIRER);
						}
						else 
							SetState(STATE_VENUS_FIRE_TRAP_DOWN_FIRER);

						_firer = new CFirer(_position.x, _position.y - DIF);
						_firer->SetState(STATE_FIRER_FLY);
						float vx, vy, x, y;
						CalcFireBallStat(mario->GetPosition().x, mario->GetPosition().y, vx, vy, x, y);
						_firer->SetVelocity({ vx,vy });
						CGame::GetInstance()->GetCurrentScene()->SpawnObject(_firer);

						_fireTimer.Reset();
						_fireTimer.Start();
					}
					else {
						SetState(STATE_VENUS_FIRE_TRAP_UP);
						_fireTimer.Reset();
						_isUp = false;
					}					
				}
				break;
		}

		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
}

void CVenusFireTrap::CalcFireBallStat(float playerX, float playerY, float& fbVx, float& fbVy, float& fbX, float& fbY)
{
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);

	float ballX = l + VENUS_FIRE_TRAP_BBOX_WIDTH / 2;
	float ballY = t + 15;

	float dxFromPlayer = playerX - ballX;
	float dyFromPlayer = playerY - ballY;

	float magDistance = sqrt(dxFromPlayer * dxFromPlayer + dyFromPlayer * dyFromPlayer);

	float angle = 0;

	if (abs(dxFromPlayer) > 48 * 6 || (0 < abs(dyFromPlayer) && abs(dyFromPlayer) < 96))
		angle = (float)(dxFromPlayer > 0 ? 25 : 155) * (dyFromPlayer < 0 ? -1 : 1);
	else
		angle = (float)(dxFromPlayer > 0 ? 45 : 135) * (dyFromPlayer < 0 ? -1 : 1);

	if (dyFromPlayer < 0) {
		_shootHeadDown = 0;
	}
	else _shootHeadDown = 1;

	angle = angle * 3.14f / 180.0f;

	float magV = sqrt(cos(angle) * cos(angle) + sin(angle) * sin(angle));

	fbVx = FIRER_FLY_SPEED * cos(angle) / magV;
	fbVy = FIRER_FLY_SPEED * sin(angle) / magV;

	fbX = ballX;
	fbY = ballY;
}

void CVenusFireTrap::OnNoCollision(DWORD dt)
{
}

void CVenusFireTrap::OnCollisionWith(LPCOLLISIONEVENT e)
{
	CKoopaParaTropa* paraKoopa = dynamic_cast<CKoopaParaTropa*>(e->obj);
	if (paraKoopa) {
		if (paraKoopa->GetState() == STATE_KOOPA_PARA_TROPA_SHELD_RUN) {
			_isDeleted = true;
		}
	}

	CKoopaTropa* koopa = dynamic_cast<CKoopaTropa*>(e->obj);
	if (koopa) {
		if (koopa->GetState() == STATE_KOOPA_TROPA_SHELD_RUN) {
			_isDeleted = true;
		}
	}

	CTail* tail = dynamic_cast<CTail*>(e->obj);
	if (tail) {
		_isDeleted = true;
	}
}

int CVenusFireTrap::IsBlocking(LPCOLLISIONEVENT e)
{
	return 0;
}

void CVenusFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (_state != STATE_VENUS_FIRE_TRAP_DIE || _state != STATE_VENUS_FIRE_TRAP_IDLE) {
		left = _position.x - VENUS_FIRE_TRAP_BBOX_WIDTH / 2;
		top = _position.y - VENUS_FIRE_TRAP_BBOX_HIEGHT / 2;
		right = left + VENUS_FIRE_TRAP_BBOX_WIDTH;
		bottom = top + VENUS_FIRE_TRAP_BBOX_HIEGHT;
	}
}
