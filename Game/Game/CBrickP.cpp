#include "CBrickP.h"
#include "CItem.h"
#include "CBrickQuestion.h"

#define ID_ANI_BRICK_P_BRICK		11000
#define ID_ANI_BRICK_P_IDLE			11001
#define ID_ANI_BRICK_P_PUSHED		11002
#define ID_ANI_BRICK_P_BRICK_UP		ID_ANI_BRICK_QUESTION_IDLE
#define ID_ANI_BRICK_P_COIN			ID_ANI_ITEM_COIN

void CBrickP::_HandleBrickBroken()
{
	if (_state == STATE_BRICK_P_BRICK_BREAK && _brickBrokens.size() == 0) {
		return;
	}

	const int listBrickBroken = 4;
	for (int i = 0; i < listBrickBroken; i++)
	{
		CBrickBorken* brickBroken = new CBrickBorken(_position.x, _position.y);
		float vx, vy;

		if (i % 2 == 0) {
			vx = -BRICK_BROKEN_SPEED_X;
		}
		else vx = BRICK_BROKEN_SPEED_X;

		if (i < 2) {
			vy = -BRICK_BROKEN_JUMP_DEFLECT_SPEED;
		}
		else vy = -BRICK_BROKEN_JUMP_DEFLECT_SPEED / 2;

		brickBroken->SetVelocity({ vx,vy });
		_brickBrokens.push_back(brickBroken);
	}
}

int CBrickP::_GetAnimationId()
{
	int aniId = -1;
	if (_state == STATE_BRICK_P_BRICK) {
		aniId = ID_ANI_BRICK_P_BRICK;
	}
	else if (_state == STATE_BRICK_P_IDLE) {
		if (_isShowP)
			aniId = ID_ANI_BRICK_P_IDLE;
	}
	else if (_state == STATE_BRICK_P_PUSHED) {
		aniId = ID_ANI_BRICK_P_PUSHED;
	}
	else if (_state == STATE_BRICK_P_BRICK_UP) {
		aniId = ID_ANI_BRICK_P_BRICK_UP;
	}
	else if (_state == STATE_BRICK_P_COIN) {
		aniId = ID_ANI_BRICK_P_COIN;
	}

	return aniId;
}

void CBrickP::SetState(int state)
{
	switch (state)
	{
	case STATE_BRICK_P_BRICK:
		break;
	case STATE_BRICK_P_IDLE:
		break;
	case STATE_BRICK_P_PUSHED:
		break;
	case STATE_BRICK_P_BRICK_UP:
		_isShowP = true;
		break;
	case STATE_BRICK_P_BRICK_BREAK:
		_HandleBrickBroken();
		break;
	case STATE_BRICK_P_COIN:
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}

void CBrickP::Render()
{
	for (int i = 0; i < (int)_brickBrokens.size(); i++)
		_brickBrokens[i]->Render();

	_aniId = _GetAnimationId();
	CGameObject::Render();
}

void CBrickP::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (_state == STATE_BRICK_P_BRICK_BREAK && !_isCanBreak) {
		SetState(STATE_BRICK_P_BRICK_UP);
	}

	for (int i = 0; i < (int)_brickBrokens.size(); i++) {
		if (_brickBrokens[0]->IsDeleted()) {
			_brickBrokens.clear();
		}
		else
			_brickBrokens[i]->Update(dt, coObjects);
	}
}

void CBrickP::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _position.x - BRICK_P_BBOX_WIDTH / 2;
	top = _position.y - BRICK_P_BBOX_HEIGHT / 2;
	right = left + BRICK_P_BBOX_WIDTH;
	bottom = top + BRICK_P_BBOX_HEIGHT;
}
