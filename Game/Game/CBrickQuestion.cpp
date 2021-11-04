#include "CBrickQuestion.h"
#include "CUtil.h"

#define SECTION_UNKNOW 0
#define SECTION_SPRITE 1
#define SECTION_FRAME 2
#define SECTION_ANIMATION 3

#define LENGTH_SPRITES 5
#define LENGTH_ANI 2
#define LENGTH_ANIS 1

#define DEFAULT_TIME 100

CBrickQuestion::CBrickQuestion(float x, float y) {
	_position = { x,y };
	_startY = y;
	_tex = CTextures::GetInstance();
	_ani = new CAnimation(DEFAULT_TIME);
	_state = STATE_BRICK_QUESTION_RUN;
	_mVy = MAX_JUMP;
	_ay = 0;
}

void CBrickQuestion::_ParseSectionSprites(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_SPRITES) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = l + atoi(tokens[3].c_str());
	int b = t + atoi(tokens[4].c_str());

	CSprites::GetInstance()->Add(id, l, t, r, b, _tex->Get(ID_IMG_TITLE_SET));
}

void CBrickQuestion::_ParseSectionFrames(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_ANI) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	int time = atoi(tokens[1].c_str());

	_ani->Add(id, time);
}

void CBrickQuestion::_ParseSectionAnimation(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_ANIS) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());

	CAnimations::GetInstance()->Add(id, _ani);

	_ani = new CAnimation(DEFAULT_TIME);
}

void CBrickQuestion::Load(LPCWSTR filePath) {
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

		if (line == "[END]") {
			section = SECTION_UNKNOW;
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

void CBrickQuestion::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (_ay != 0) {
		_velocity.y += _ay * dt;
	}
	else {
		_velocity.y = 0;
	}

	if (abs(_velocity.y) >= _mVy) {
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
	aniId = aniId == -1 ? ID_BRICK_QUESTION_ANI_IDLE : aniId;
	return aniId;
}