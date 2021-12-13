#include "CBrickQuestion.h"
#include "CUtil.h"
#include "CItem.h"


CBrickQuestion::CBrickQuestion(float x, float y, string typeItem) {
	_position = { x,y };
	_startPostion = _position;
	_state = STATE_BRICK_QUESTION_RUN;
	_maxVy = MAX_JUMP;
	_ay = 0;
	_item = new CItem(x, y, typeItem);
}

void CBrickQuestion::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (_ay != 0) {
		_velocity.y += _ay * dt;
	}
	else {
		_velocity.y = 0;
	}

	if (abs(_velocity.y) >= _maxVy) {
		_ay = JUMP_SPEED;
	}

	float dy = _velocity.y * dt / 4;

	if (_position.y + dy >= _startPostion.y) {
		_position = _startPostion;
		_ay = 0;
	}
	else
		_position.y += dy;

	_item->Update(dt, coObjects);
	
}

void CBrickQuestion::Render() {
	_item->Render();
	_aniId = _GetAnimationId();
	CGameObject::Render();
}

void CBrickQuestion::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = _position.x - BRICK_WIDTH / 2;
	top = _position.y - BRICK_HEIGHT / 2;
	right = left + BRICK_WIDTH;
	bottom = top + BRICK_HEIGHT;
}

void CBrickQuestion::SetState(int state) {
	if (state == STATE_BRICK_QUESTION_IDLE && _state == STATE_BRICK_QUESTION_RUN) {
		_ay = -JUMP_SPEED;
		_HandleStateItem();
	}
	CGameObject::SetState(state);
}

int CBrickQuestion::_GetAnimationId() {
	int aniId = -1;
	if (_state == STATE_BRICK_QUESTION_RUN) {
		aniId = ID_ANI_BRICK_QUESTION_RUN;
	}
	else aniId = ID_ANI_BRICK_QUESTION_IDLE;
	return aniId;
}

void CBrickQuestion::_HandleStateItem() {
	if (_item->GetType() == TYPE_ITEM_COIN_BRICK) {
		_item->SetState(STATE_ITEM_JUMP);
	}
	else if (_item->GetType() == TYPE_ITEM_MUSHROOM_GREEN || _item->GetType() == TYPE_ITEM_MUSHROOM_RED) {
		_item->SetState(STATE_ITEM_UP);
	}
}