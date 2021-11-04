#include "CMario.h"
#include "DefineMario.h"

#include "CBrick.h"
#include "CBrickQuestion.h"

#define LENGTH_SPRITES 5
#define LENGTH_ANI 2
#define LENGTH_ANIS 1

#define SECTION_UNKNOW 0
#define SECTION_SPRITE 1
#define SECTION_FRAME 2
#define SECTION_ANIMATION 3

#define DEFAULT_TIME 100

#define MIN_X 0
#define MAX_Y 400
#define MIN_Y 300

CMario* CMario::__instance = NULL;

CMario::CMario() {
	_position = { 0,0 };
	_state = STATE_MARIO_IDLE;
	_level = LEVEL_BIG;
	_direction = DIRECTION_RIGHT;
	_scale = { 1.0f * _direction,1.0f };
	_velocity = { 0,0 };
	_tex = CTextures::GetInstance();
	_ani = new CAnimation(DEFAULT_TIME);
	_isActive = false;
	_runSpeed = 0.09f;
	_jumpSpeed = 0.327f;
	_isJumping = false;
	_isOnGround = false;
	_canJump = true;
	_maxVx = 0.0f;
	_ax = 0.0f;
	_ay = MARIO_GRAVITY;
	_untouchable = 0;
	_untouchable_start = -1;
	_isOnPlatform = false;
}

void CMario::_ParseSectionSprites(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_SPRITES) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = l + atoi(tokens[3].c_str());
	int b = t + atoi(tokens[4].c_str());

	CSprites::GetInstance()->Add(id, l, t, r, b, _tex->Get(ID_IMG_MARIO));
}

void CMario::_ParseSectionFrames(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_ANI) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	int time = atoi(tokens[1].c_str());

	_ani->Add(id, time);
}

void CMario::_ParseSectionAnimation(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_ANIS) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());

	CAnimations::GetInstance()->Add(id, _ani);

	_ani = new CAnimation(DEFAULT_TIME);
}

void CMario::Load(LPCWSTR filePath) {
	ifstream f;
	f.open(filePath);

	int section = SECTION_UNKNOW;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#' || line.empty()) continue; //comment line
		
		if (line == "[SPRITE]") {
			section = SECTION_SPRITE;
			continue;
		}
		if (line == "[FRAME]") {
			section = SECTION_FRAME;
			continue;
		}
		if (line == "[ANIMATION]") {
			section = SECTION_ANIMATION;
			continue;
		}

		if (line=="[END]") {
			section = SECTION_UNKNOW;
			_isActive = true;
			_HandleState(STATE_MARIO_IDLE);
			continue;
		}

		switch (section)
		{
		case SECTION_SPRITE:
			_ParseSectionSprites(line);
			break;
		case SECTION_FRAME:
			_ParseSectionFrames(line);
			break;
		case SECTION_ANIMATION:
			_ParseSectionAnimation(line);
			break;
		default:
			break;
		}
	}
	f.close();
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		_velocity.y = 0;
		if (e->ny < 0) _isOnPlatform = true;
		if (e->ny > 0) {
		}
	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			_isOnPlatform = true;
			_velocity.x = 0;
		}
	if (dynamic_cast<CBrick*>(e->obj))
		_OnCollisionWithBrick(e);
	else if (dynamic_cast<CBrickQuestion*>(e->obj))
		_OnCollisionWithBrickQuestion(e);
}

void CMario::_OnCollisionWithBrick(LPCOLLISIONEVENT e) {
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
	
	
}

void CMario::_OnCollisionWithBrickQuestion(LPCOLLISIONEVENT e) {
	CBrickQuestion* brick = dynamic_cast<CBrickQuestion*>(e->obj);
	if (e->ny > 0) {
		brick->SetState(STATE_BRICK_QUESTION_IDLE);
	}
}

void CMario::_OnCollisionWithGoomba(LPCOLLISIONEVENT e) {

}


void CMario::OnNoCollision(DWORD dt) {
	int dx = _velocity.x * dt;

	if (_position.x + dx <= MIN_X) {
	}
	else _position.x += dx;

	_position.y += _velocity.y * dt / 4.5;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
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
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::_HandleKeyDown(int keyCode) {

	switch (keyCode)
	{
	case DIK_1:
		_level = LEVEL_SMALL;
		break;
	case DIK_2:
		_level = LEVEL_BIG;
		_position.y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT / 2;
		break;
	case DIK_DOWN:
		_HandleState(STATE_MARIO_SIT);
		break;
	case DIK_S:
		_HandleState(STATE_MARIO_JUMP);
		break;
	case DIK_0:
		_HandleState(STATE_MARIO_DIE);
		break;
	default:
		break;
	}

}

void CMario::_HandleKeyUp(int keyCode) {
	switch (keyCode)
	{
	case DIK_S:
		_HandleState(STATE_MARIO_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		_HandleState(STATE_MARIO_RELEASE_SIT);
		break;
	}
}

void CMario::_HandleKeyState(BYTE* states) {

	CKeyBoardCustom* keyHandle = CKeyBoardCustom::GetInstance();

	if (keyHandle->IsKeyDown(DIK_RIGHTARROW)) {
		_direction = DIRECTION_RIGHT;
		_scale.x = _direction * 1.0f;
		if (keyHandle->IsKeyDown(DIK_A)) {
			_HandleState(STATE_MARIO_RUN);
		}
		else {
			_HandleState(STATE_MARIO_WALK);
		}
	}
	else if (keyHandle->IsKeyDown(DIK_LEFTARROW)) {
		_direction = DIRECTION_LEFT;
		_scale.x = _direction * 1.0f;
		if (keyHandle->IsKeyDown(DIK_A)) {
			_HandleState(STATE_MARIO_RUN);
		}
		else {
			_HandleState(STATE_MARIO_WALK);
		}
	}
	else _HandleState(STATE_MARIO_IDLE);
}

void CMario::_HandleState(int state) {
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

void CMario::Release() {

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
			left = _position.x - MARIO_BIG_BBOX_WIDTH / 2;
			top = _position.y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = _position.x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = _position.y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

int CMario::_GetAnimationId() {
	if (_level == LEVEL_BIG) {
		return _HandleAnimationBig();
	}
	else {
		return _HandleAnimationSmall();
	}
}

int CMario::_HandleAnimationBig() {
	int aniId = -1;
	if (!_isOnPlatform)
	{
		if (abs(_ax) == MARIO_ACCEL_RUN_X)
		{
				aniId = ID_BIG_MARIO_ANI_JUMP_RUN;
		}
		else
		{
				aniId = ID_BIG_MARIO_ANI_JUMP_WALK;
		}
	}
	else
		if (_isSitting)
		{
				aniId = ID_BIG_MARIO_ANI_SIT;
		}
		else
			if (_velocity.x == 0)
			{
				aniId = ID_BIG_MARIO_ANI_IDLE;
			}
			else if(_velocity.x > 0)
			{
				if (_ax < 0)
					aniId = ID_BIG_MARIO_ANI_BRACE;
				else if (_ax == MARIO_ACCEL_RUN_X)
					aniId = ID_BIG_MARIO_ANI_RUN;
				else if (_ax == MARIO_ACCEL_WALK_X)
					aniId = ID_BIG_MARIO_ANI_WALK;
			}
			else
			{
				if (_ax > 0)
					aniId = ID_BIG_MARIO_ANI_BRACE;
				else if (_ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_BIG_MARIO_ANI_RUN;
				else if (_ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_BIG_MARIO_ANI_WALK;
			}

	if (aniId == -1) aniId = ID_BIG_MARIO_ANI_IDLE;

	return aniId;
}

int CMario::_HandleAnimationSmall() {
	int aniId = -1;
	if (!_isOnPlatform)
	{
		if (abs(_ax) == MARIO_ACCEL_RUN_X)
		{
			aniId = ID_SMALL_MARIO_ANI_JUMP_RUN;
		}
		else
		{
			aniId = ID_SMALL_MARIO_ANI_JUMP_WALK;
		}
	}
	else
		if (_isSitting)
		{
			//aniId = ID_SMALL_MARIO_ANI_SIT;
		}
		else
			if (_velocity.x == 0)
			{
				aniId = ID_SMALL_MARIO_ANI_IDLE;
			}
			else if (_velocity.x > 0)
			{
				if (_ax < 0)
					aniId = ID_SMALL_MARIO_ANI_BRACE;
				else if (_ax == MARIO_ACCEL_RUN_X)
					aniId = ID_SMALL_MARIO_ANI_RUN;
				else if (_ax == MARIO_ACCEL_WALK_X)
					aniId = ID_SMALL_MARIO_ANI_WALK;
			}
			else {
				if (_ax > 0)
					aniId = ID_SMALL_MARIO_ANI_BRACE;
				else if (_ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_SMALL_MARIO_ANI_RUN;
				else if (_ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_SMALL_MARIO_ANI_WALK;
			}

	if (aniId == -1) aniId = ID_SMALL_MARIO_ANI_IDLE;

	return aniId;
}