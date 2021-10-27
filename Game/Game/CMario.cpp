#include "CMario.h"
#include "DefineMario.h"

#define LENGTH_SPRITES 5
#define LENGTH_ANI 2
#define LENGTH_ANIS 1

#define SECTION_UNKNOW 0
#define SECTION_SPRITE 1
#define SECTION_FRAME 2
#define SECTION_ANIMATION 3

#define DEFAULT_TIME 100

#define MIN_X 10

CMario* CMario::__instance = NULL;

CMario::CMario() {
	_position = { 0,0 };
	_state = STATE_MARIO_IDLE;
	_level = LEVEL_SMALL;
	_currentAniId = NULL;
	_direction = DIRECTION_LEFT;
	_tex = CTextures::GetInstance();
	_tex->Add(ID_IMG_MARIO, PATH_IMG_MARIO);
	_ani = new CAnimation(DEFAULT_TIME);
	_isActive = false;
	_runSpeed = 0.09f;
	_jumpSpeed = 0.327f;
}

void CMario::_ParseSectionSprites(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_SPRITES) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());

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

void CMario::Load(string filePath) {
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
			_HandleState();
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
}

void CMario::Update(DWORD dt, vector<CGameObject*>* collidableObjects) {
	if (!_isActive) {
		return;
	}

	//test
	if (_position.y <= 399) {
		_position.y += dt * 0.1f;
	}

	if (_position.x < MIN_X) {
		_position.x = MIN_X;
	}
	else _position.x += _velocity.x * dt * 1.1f;

	_HandleKeyboard();
	_HandleState();

	_UpdateCamPosition();
}

void CMario::_HandleKeyboard() {
	CKeyBoardCustom* _keyboard = CKeyBoardCustom::GetInstance();

	if (_keyboard->IsKeyDown(DIK_LEFTARROW)) {
		_state = STATE_MARIO_WALK;
		_direction = DIRECTION_LEFT;
		_scale.x = 1.0f;
		return;
	}
	else if (_keyboard->IsKeyDown(DIK_RIGHTARROW)) {
		_state = STATE_MARIO_WALK;
		_direction = DIRECTION_RIGHT;
		_scale.x = -1.0f;
		return;
	}
	_state = STATE_MARIO_IDLE;
}

void CMario::_HandleState() {
	switch (_state)
	{
	case STATE_MARIO_IDLE:
		this->_Idle();
			break;
	case STATE_MARIO_WALK:
		this->_Move();
		break;
	case STATE_MARIO_JUMP:
		this->_Jump();
		break;
	case STATE_MARIO_SIT:
		this->_Sit();
		break;
	case STATE_MARIO_DIE:
		this->_Die();
		break;
	default:
		break;
	}
}

void CMario::Render() {
	if (_currentAniId != NULL) {
		CAnimations::GetInstance()->Get(_currentAniId)->Render(ID_IMG_MARIO, _position.x, _position.y, _scale);
	}
}

void CMario::Release() {

}

void CMario::_Move() {
	_velocity.x = _direction * _runSpeed;
	if (_level == LEVEL_SMALL) {
		_currentAniId = ID_SMALL_MARIO_ANI_WALK;
	}
	else if (_level == LEVEL_BIG) {
		_currentAniId = ID_BIG_MARIO_ANI_WALK;
	}
	else if (_level == LEVEL_SUPER) {
		_currentAniId = ID_SUPER_MARIO_ANI_WALK;
	}
}

void CMario::_Jump() {

}

void CMario::_Attack() {

}

void CMario::_Sit() {

}

void CMario::_Idle() {
	_velocity.x = 0.0f;
	if (_level == LEVEL_SMALL) {
		_currentAniId = ID_SMALL_MARIO_ANI_IDLE;
	}
	else if (_level == LEVEL_BIG) {
		_currentAniId = ID_BIG_MARIO_ANI_IDLE;
	}
	else if (_level == LEVEL_SUPER) {
		_currentAniId = ID_SUPER_MARIO_ANI_IDLE;
	}
}

void CMario::_Die() {

}

void CMario::_HanldeDie() {

}

void CMario::_UpdateCamPosition() {
	int _sceneWidth = 2816;
	int _sceneHeight = 656;

	LPRECTCUSTOM cameraBound = CCam::GetInstance()->GetCameraBound();
	D3DXVECTOR2 cameraPosition = CCam::GetInstance()->GetPosition();

	cameraPosition = GetPosition();
	cameraPosition.x -= CGame::GetInstance()->GetWindowWidth() / 2.25f;
	if (cameraPosition.x < cameraBound->GetLeft()) {
		cameraPosition.x = cameraBound->GetLeft();
	}
	else if (cameraPosition.x + CGame::GetInstance()->GetWindowWidth() > cameraBound->GetRight()) {
		cameraPosition.x = cameraBound->GetRight() - CGame::GetInstance()->GetWindowWidth();
	}

	cameraPosition.y -= CGame::GetInstance()->GetWindowHeight() / 2.25f;
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