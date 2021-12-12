#include "CMario.h"
#include "DefineMario.h"

#include "CBrick.h"
#include "CItem.h"
#include "CBrickQuestion.h"
#include "CDebug.h"
#include "CGoomba.h"
#include "CKoopaParaTropa.h"
#include "CParaGoomba.h"
#include "CKoopaTropa.h"
#include "CVenusFireTrap.h"
#include "CPiranhaPlant.h"

#define MIN_X 0
#define MAX_Y 400
#define MIN_Y 300
#define START_LIVE 4

CMario* CMario::__instance = NULL;

CMario::CMario() {
	_isSitting = false;
	_position = { 0,0 };
	_state = STATE_MARIO_IDLE;
	_level = LEVEL_SMALL;
	_direction = DIRECTION_RIGHT;
	_scale = { 1.0f * _direction,1.0f };
	_velocity = { 0,0 };
	_isActive = false;
	_maxVx = 0.0f;
	_ax = 0.0f;
	_ay = MARIO_GRAVITY;
	_untouchable = 0;
	_untouchable_start = -1;
	_isOnPlatform = false;
	_dt = 0;
	_startPostion = _position;
	_coin = 0;
	_live = START_LIVE;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (dynamic_cast<CBrick*>(e->obj))
		_OnCollisionWithBrick(e);
	else if (dynamic_cast<CBrickQuestion*>(e->obj))
		_OnCollisionWithBrickQuestion(e);
	else if (dynamic_cast<CItem*>(e->obj)) 
		_OnCollisionWithItem(e);
	else if (dynamic_cast<CGoomba*>(e->obj)) 
		_OnCollisionWithGoomba(e);
	else if (dynamic_cast<CKoopaParaTropa*>(e->obj))
		_OnCollisionWithKoopaParaTropa(e);
	else if (dynamic_cast<CKoopaTropa*>(e->obj))
		_OnCollisionWithKoopaTropa(e);
	else if (dynamic_cast<CParaGoomba*>(e->obj))
		_OnCollisionWithParaGoomba(e);
	else if (dynamic_cast<CVenusFireTrap*>(e->obj) || dynamic_cast<CPiranhaPlant*>(e->obj))
		if (_untouchable == 0) _OnCollisionWithEnemy(e);
}

void CMario::_OnCollisionWithBrick(LPCOLLISIONEVENT e) {
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
	if (brick->IsBig()) {
		if (e->ny != 0 && e->obj->IsBlocking())
		{
			if (e->ny < 0) {
				_velocity.y = 0;
				_isOnPlatform = true;
			}
			else {
				_handleNoCollisionY = true;
			}
		}
		else
			if (e->nx != 0 && e->obj->IsBlocking())
			{
				_handleNoCollisionX = true;
			}
	}
	else {
		if (e->ny != 0 && e->obj->IsBlocking())
		{
			_velocity.y = 0;
			if (e->ny < 0) _isOnPlatform = true;
		}
		else
			if (e->nx != 0 && e->obj->IsBlocking())
			{
				_velocity.x = 0;
			}
	}
}

void CMario::_OnCollisionWithBrickQuestion(LPCOLLISIONEVENT e) {
	CBrickQuestion* brick = dynamic_cast<CBrickQuestion*>(e->obj);

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		_velocity.y = 0;
		if (e->ny < 0) {
			_isOnPlatform = true;
		}
		else if (brick->GetState() == STATE_BRICK_QUESTION_RUN && e->obj->IsBlocking())
		{
			brick->SetState(STATE_BRICK_QUESTION_IDLE);
		}
	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			_velocity.x = 0;
		}
}


void CMario::_OnCollisionWithItem(LPCOLLISIONEVENT e) {
	CItem* item = dynamic_cast<CItem*>(e->obj);

	if (item->GetType() == TYPE_ITEM_COIN) {
		item->Delete();
	}

	if (item->GetState() == STATE_ITEM_IDLE) {
		return;
	}

	if (item->GetType() == TYPE_ITEM_MUSHROOM_RED) {
		SetLevel(++_level);
		item->SetState(STATE_ITEM_DISAPPEAR);
	}
}

void CMario::_OnCollisionWithGoomba(LPCOLLISIONEVENT e) {
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (e->ny < 0) {
		goomba->SetState(STATE_GOOMBA_DIE);
		_velocity.y = -MARIO_JUMP_DEFLECT_SPEED;
	}
	else {
		if (_untouchable == 0)
		{
			_OnCollisionWithEnemy(e);
		}
	}
}

void CMario::_OnCollisionWithKoopaParaTropa(LPCOLLISIONEVENT e) {
	CKoopaParaTropa* koopaParaTropa = dynamic_cast<CKoopaParaTropa*>(e->obj);
	if (e->ny < 0) {
		if (koopaParaTropa->GetState() != STATE_KOOPA_PARA_TROPA_DIE) {
			if (koopaParaTropa->GetState() == STATE_KOOPA_PARA_TROPA_WALK || koopaParaTropa->GetState() == STATE_KOOPA_PARA_TROPA_SHELD_RUN) {
				_velocity.y = -MARIO_JUMP_DEFLECT_SPEED;
				koopaParaTropa->SetState(STATE_KOOPA_PARA_TROPA_SHELD);
			}
			else if (koopaParaTropa->GetState() == STATE_KOOPA_PARA_TROPA_SHELD) {
				if (_position.x < koopaParaTropa->GetPosition().x) {
					koopaParaTropa->SetScale({ -koopaParaTropa->GetScale().x, koopaParaTropa->GetScale().y });
				}
				else koopaParaTropa->SetScale({ koopaParaTropa->GetScale().x, koopaParaTropa->GetScale().y });
				koopaParaTropa->SetState(STATE_KOOPA_PARA_TROPA_SHELD_RUN);
				_handleNoCollisionY = true;
			}
		}
	}
	else {
		if (_untouchable == 0) {
			if (koopaParaTropa->GetState() == STATE_KOOPA_PARA_TROPA_WALK 
				|| koopaParaTropa->GetState() == STATE_KOOPA_PARA_TROPA_FLY
				|| koopaParaTropa->GetState() == STATE_KOOPA_PARA_TROPA_SHELD_RUN) {
				_OnCollisionWithEnemy(e);
			}
			else {
				if (koopaParaTropa->GetState() == STATE_KOOPA_PARA_TROPA_SHELD) {
					koopaParaTropa->SetScale({ _scale.x, koopaParaTropa->GetScale().y });
					koopaParaTropa->SetState(STATE_KOOPA_PARA_TROPA_SHELD_RUN);
					SetState(STATE_MARIO_KICK);
				}
			}
		}
	}
}

void CMario::_OnCollisionWithKoopaTropa(LPCOLLISIONEVENT e) {
	CKoopaTropa* koopaTropa = dynamic_cast<CKoopaTropa*>(e->obj);
	if (e->ny < 0) {
		if (koopaTropa->GetState() != STATE_KOOPA_TROPA_DIE) {
			if (koopaTropa->GetState() == STATE_KOOPA_TROPA_FLY) {
				_velocity.y = -MARIO_JUMP_DEFLECT_SPEED;
				koopaTropa->SetState(STATE_KOOPA_TROPA_WALK);
			}
			else if (koopaTropa->GetState() == STATE_KOOPA_TROPA_WALK || koopaTropa->GetState() == STATE_KOOPA_TROPA_SHELD_RUN) {
				_velocity.y = -MARIO_JUMP_DEFLECT_SPEED;
				koopaTropa->SetState(STATE_KOOPA_TROPA_SHELD);
			}
			else if (koopaTropa->GetState() == STATE_KOOPA_TROPA_SHELD) {
				if (_position.x < koopaTropa->GetPosition().x) {
					koopaTropa->SetScale({ -koopaTropa->GetScale().x, koopaTropa->GetScale().y });
				}
				else koopaTropa->SetScale({ koopaTropa->GetScale().x, koopaTropa->GetScale().y });
				koopaTropa->SetState(STATE_KOOPA_TROPA_SHELD_RUN);
				_handleNoCollisionY = true;
			}
		}
	}
	else {
		if (_untouchable == 0) {
			if (koopaTropa->GetState() == STATE_KOOPA_TROPA_WALK
				|| koopaTropa->GetState() == STATE_KOOPA_TROPA_FLY
				|| koopaTropa->GetState() == STATE_KOOPA_TROPA_SHELD_RUN) {
				_OnCollisionWithEnemy(e);
			}
			else {
				if (koopaTropa->GetState() == STATE_KOOPA_TROPA_SHELD) {
					koopaTropa->SetScale({ _scale.x, koopaTropa->GetScale().y });
					koopaTropa->SetState(STATE_KOOPA_TROPA_SHELD_RUN);
				}
			}
		}
	}
}

void CMario::_OnCollisionWithParaGoomba(LPCOLLISIONEVENT e) {
	CParaGoomba* paragoomba = dynamic_cast<CParaGoomba*>(e->obj);

	if (e->ny < 0) {
		if (paragoomba->GetState() == STATE_PARA_GOOMBA_FLY || paragoomba->GetState() == STATE_PARA_GOOMBA_WALK) {
			_velocity.y = -MARIO_JUMP_DEFLECT_SPEED;
			paragoomba->SetState(STATE_RED_GOOMBA_WALK);
		}
		else if (paragoomba->GetState() == STATE_RED_GOOMBA_WALK) {
			_velocity.y = -MARIO_JUMP_DEFLECT_SPEED;
			paragoomba->SetState(STATE_RED_GOOMBA_DIE);
		}
	}
	else {
		if (_untouchable == 0) {
			_OnCollisionWithEnemy(e);
		}
	}
}

void CMario::_OnCollisionWithEnemy(LPCOLLISIONEVENT e) {
	if (_level > LEVEL_SMALL)
	{
		SetLevel(--_level);
		StartUntouchable();
	}
	else
	{
		DebugOut(L">>> Mario DIE >>> \n");
		SetState(STATE_MARIO_DIE);
	}
}

void CMario::OnNoCollision(DWORD dt) {
	_position.x += (_velocity.x * dt);
	_position.y += (_velocity.y * dt) / 2.25f;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	_dt = dt;
	_velocity.x += _ax * dt;
	_velocity.y += _ay * dt;

	if (abs(_velocity.x) > abs(_maxVx)) _velocity.x = _maxVx;

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - _untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		_untouchable_start = 0;
		_untouchable = 0;
	}

	_isOnPlatform = false;

	_UpdateCamPosition();
	CGameObject::Update(dt, coObjects);
	
}

void CMario::_HandleKeyDown(int keyCode) {

	switch (keyCode)
	{
	case DIK_1:
		SetLevel(LEVEL_SMALL);
		break;
	case DIK_2:
		SetLevel(LEVEL_BIG);
		break;
	case DIK_3:
		SetState(STATE_MARIO_DIE);
		break;
	case DIK_4:
		_position.x += 500.0f * -_direction;
		break;
	case DIK_DOWN:
		SetState(STATE_MARIO_SIT);
		break;
	case DIK_S:
		SetState(STATE_MARIO_JUMP);
		break;
	case DIK_0:
		SetState(STATE_MARIO_DIE);
		break;
	default:
		break;
	}

}

void CMario::_HandleKeyUp(int keyCode) {
	switch (keyCode)
	{
	case DIK_S:
		SetState(STATE_MARIO_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		SetState(STATE_MARIO_RELEASE_SIT);
		break;
	}
}

void CMario::_HandleKeyState(BYTE* states) {

	CKeyBoardCustom* keyHandle = CKeyBoardCustom::GetInstance();

	if (keyHandle->IsKeyDown(DIK_RIGHTARROW)) {
		_direction = DIRECTION_RIGHT;
		_scale.x = _direction * 1.0f;
		if (keyHandle->IsKeyDown(DIK_A)) {
			SetState(STATE_MARIO_RUN);
		}
		else {
			SetState(STATE_MARIO_WALK);
		}
	}
	else if (keyHandle->IsKeyDown(DIK_LEFTARROW)) {
		_direction = DIRECTION_LEFT;
		_scale.x = _direction * 1.0f;
		if (keyHandle->IsKeyDown(DIK_A)) {
			SetState(STATE_MARIO_RUN);
		}
		else {
			SetState(STATE_MARIO_WALK);
		}
	}
	else SetState(STATE_MARIO_IDLE);
}

void CMario::SetState(int state) {

	if (_state == STATE_MARIO_DIE) return;

	switch (state)
	{
	case STATE_MARIO_IDLE:
		this->_Idle();
		break;
	case STATE_MARIO_WALK:
		this->_Walk();
		break;
	case STATE_MARIO_JUMP:
		this->_Jump();
		break;
	case STATE_MARIO_RELEASE_SIT:
		this->_ReleaseSit();
		break;
	case STATE_MARIO_SIT:
		this->_Sit();
		break;
	case STATE_MARIO_RELEASE_JUMP:
		this->_ReleaseJump();
		break;
	case STATE_MARIO_DIE:
		this->_Die();
		break;
	case STATE_MARIO_RUN:
		this->_Run();
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}

void CMario::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = _GetAnimationId();
	LPANIMATION animation = animations->Get(aniId);
	if (animation == NULL) {
		return;
	}

	animations->Get(aniId)->Render(_position.x, _position.y, _scale);
	RenderBoundingBox();
}

void CMario::OnKeyDown(int KeyCode) {
	_HandleKeyDown(KeyCode);
}

void CMario::OnKeyUp(int keyCode) {
	_HandleKeyUp(keyCode);
}

void CMario::KeyState(BYTE* states) {
	_HandleKeyState(states);
}

void CMario::_Walk() {
	if (_isSitting) return;
	_maxVx = MARIO_WALKING_SPEED * -_direction;
	_ax = MARIO_ACCEL_WALK_X * -_direction;
}

void CMario::_Run() {
	if (_isSitting) return;
	_maxVx = MARIO_RUNNING_SPEED * -_direction;
	_ax = MARIO_ACCEL_RUN_X * -_direction;
}

void CMario::_Jump() {
	if (_isSitting) return;
	if (_isOnPlatform)
	{
		if (abs(_velocity.x) == MARIO_RUNNING_SPEED)
			_velocity.y = -MARIO_JUMP_RUN_SPEED_Y;
		else
			_velocity.y = -MARIO_JUMP_SPEED_Y;
	}
}

void CMario::_ReleaseJump() {
	if (_velocity.y < 0) _velocity.y += MARIO_JUMP_SPEED_Y / 2;
}

void CMario::_Attack() {

}

void CMario::_Sit() {
	if (_isOnPlatform && _level != LEVEL_SMALL && _velocity.x == 0) {
		_state = STATE_MARIO_IDLE;
		_isSitting = true;
		_velocity = { 0.0f,0.0f };
		_position.y += MARIO_SIT_HEIGHT_ADJUST;
	}
}

void CMario::_ReleaseSit() {
	if (_isSitting)
	{
		_isSitting = false;
		_state = STATE_MARIO_IDLE;
		_position.y -= MARIO_SIT_HEIGHT_ADJUST;
	}
}

void CMario::_Idle() {
	_velocity.x = 0.0f;
	_ax = 0.0f;
}

void CMario::_Die() {
	_velocity.y = -MARIO_JUMP_DEFLECT_SPEED;
	_velocity.x = 0;
	_ax = 0;
	_live--;
}

void CMario::_HanldeDie() {

}

void CMario::_UpdateCamPosition() {
	//int _sceneWidth = 2816;
	int _sceneHeight = 656;

	LPRECTCUSTOM cameraBound = CCam::GetInstance()->GetCameraBound();
	D3DXVECTOR2 cameraPosition = CCam::GetInstance()->GetPosition();

	cameraPosition = GetPosition();
	cameraPosition.x -= CGame::GetInstance()->GetWindowWidth() / 2.0f;
	if (cameraPosition.x < cameraBound->GetLeft()) {
		cameraPosition.x = cameraBound->GetLeft();
	}
	else if (cameraPosition.x + CGame::GetInstance()->GetWindowWidth() > cameraBound->GetRight()) {
		cameraPosition.x = cameraBound->GetRight() - CGame::GetInstance()->GetWindowWidth();
	}

	cameraPosition.y -= CGame::GetInstance()->GetWindowHeight() / 2.0f;
	if (GetPosition().y < _sceneHeight * 0.3f) {
		if (cameraPosition.y < cameraBound->GetTop()) {
			cameraPosition.y = cameraBound->GetTop();
		}
		else if (cameraPosition.y + CGame::GetInstance()->GetWindowHeight() > cameraBound->GetBottom()) {
			cameraPosition.y = cameraBound->GetBottom() - CGame::GetInstance()->GetWindowHeight();
		}
	}
	else {
		cameraPosition.y = cameraBound->GetBottom() - CGame::GetInstance()->GetWindowHeight();
	}

	CCam::GetInstance()->SetPosition(cameraPosition);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	if (_level == LEVEL_BIG)
	{
		if (_isSitting)
		{
			left = _position.x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = _position.y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = _position.x - MARIO_BIG_BBOX_WIDTH / 2 + ZONE;
			top = _position.y - MARIO_BIG_BBOX_HEIGHT / 2 + ZONE;
			right = left + MARIO_BIG_BBOX_WIDTH - ZONE;
			bottom = top + MARIO_BIG_BBOX_HEIGHT - ZONE;
		}
	}
	else
	{
		left = _position.x - MARIO_SMALL_BBOX_WIDTH / 2 + ZONE;
		top = _position.y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH - ZONE;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

int CMario::_GetAnimationId() {
	int aniId = -1;
	if (_state == STATE_MARIO_DIE) {
		aniId = ID_MARIO_ANI_DIE;
	}
	else {
		if (!_isOnPlatform)
		{
			if (abs(_ax) == MARIO_ACCEL_RUN_X)
			{
				aniId = ID_MARIO_ANI_JUMP_RUN;
			}
			else
			{
				aniId = ID_MARIO_ANI_JUMP_WALK;
			}
		}
		else
			if (_isSitting)
			{
				aniId = ID_MARIO_ANI_SIT;
			}
			else
				if (_velocity.x == 0)
				{
					if (_state == STATE_MARIO_KICK) {
						aniId = ID_MARIO_ANI_KICK;
					}
					else
						aniId = ID_MARIO_ANI_IDLE;
				}
				else if (_velocity.x > 0)
				{
					if (_ax < 0)
						aniId = ID_MARIO_ANI_BRACE;
					else if (_ax == MARIO_ACCEL_RUN_X)
						aniId = ID_MARIO_ANI_RUN;
					else if (_ax == MARIO_ACCEL_WALK_X)
						aniId = ID_MARIO_ANI_WALK;
				}
				else
				{
					if (_ax > 0)
						aniId = ID_MARIO_ANI_BRACE;
					else if (_ax == -MARIO_ACCEL_RUN_X)
						aniId = ID_MARIO_ANI_RUN;
					else if (_ax == -MARIO_ACCEL_WALK_X)
						aniId = ID_MARIO_ANI_WALK;
				}

		if (aniId == -1) aniId = ID_MARIO_ANI_IDLE;
	}

	aniId += (_level % 100) * 100;

	return aniId;
}

void CMario::SetLevel(int level) {
	_level = level;
	if (level == LEVEL_BIG) {
		_velocity.y = -MARIO_JUMP_DEFLECT_SPEED;
	}
}