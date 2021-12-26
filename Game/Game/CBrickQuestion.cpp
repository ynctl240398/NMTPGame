#include "CBrickQuestion.h"
#include "CUtil.h"
#include "CItem.h"


CBrickQuestion::CBrickQuestion(float x, float y, string typeItem, string skin) {
	_position = { x,y };
	_startPostion = _position;
	_active = false;
	_step = 0;
	_item = new CItem(x, y, typeItem);
	
	if (skin == "question") {
		_state = STATE_BRICK_QUESTION_RUN;
	}
	if (skin == "brick") {
		_state = STATE_BRICK_BRICK_RUN;
	}
}

void CBrickQuestion::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (_active && _step < 2) {
		if (_step == 0) {
			_position.y -= JUMP_SPEED * dt;
			if (abs(_position.y - _startPostion.y) > 12) {
				_step = 1;
			}
		}
		if (_step == 1) {
			_position.y += JUMP_SPEED * dt;
			if (_position.y >= _startPostion.y) {
				_position.y = _startPostion.y;
				_step = 2;
				_state = STATE_BRICK_QUESTION_IDLE;
			}
		}
	}
}

void CBrickQuestion::Render() {
	_aniId = _GetAnimationId();
	CGameObject::Render();
}

void CBrickQuestion::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = _position.x - BRICK_WIDTH / 2;
	top = _position.y - BRICK_HEIGHT / 2;
	right = left + BRICK_WIDTH;
	bottom = top + BRICK_HEIGHT;
}

void CBrickQuestion::Active()
{
	if (!_active) {
		_active = true;
		CGame::GetInstance()->GetCurrentScene()->SpawnObject(_item);
	}
}

bool CBrickQuestion::IsActivated()
{
	return _active;
}

int CBrickQuestion::_GetAnimationId() {
	int aniId = -1;
	switch (_state)
	{
	case STATE_BRICK_QUESTION_IDLE:
		aniId = ID_ANI_BRICK_QUESTION_IDLE;
		break;
	case STATE_BRICK_QUESTION_RUN:
		aniId = ID_ANI_BRICK_QUESTION_RUN;
		break;
	case STATE_BRICK_BRICK_RUN:
		aniId = ID_ANI_BRICK_BRICK_RUN;
		break;
	}
	return aniId;
}