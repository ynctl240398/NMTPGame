#include "CBrickQuestion.h"
#include "CUtil.h"
#include "CItem.h"


CBrickQuestion::CBrickQuestion(float x, float y) {
	_position = { x,y };
	_startY = y;
	_state = STATE_BRICK_QUESTION_RUN;
	_maxVy = MAX_JUMP;
	_ay = 0;
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

	if (_position.y + dy >= _startY) {
		_position.y = _startY;
		_ay = 0;
	}
	else
		_position.y += dy;
}

void CBrickQuestion::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = _GetAnimationId();
	LPANIMATION animation = animations->Get(aniId);
	if (animation == NULL) {
		return;
	}

	animations->Get(aniId)->Render(_position.x, _position.y, _scale);
	RenderBoundingBox();
}

void CBrickQuestion::Release() {

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
	}
	CGameObject::SetState(state);
}

int CBrickQuestion::_GetAnimationId() {
	int aniId = -1;
	if (_state == STATE_BRICK_QUESTION_RUN) {
		aniId = ID_BRICK_QUESTION_ANI_RUN;
	}
	else aniId = ID_BRICK_QUESTION_ANI_IDLE;
	return aniId;
}