#include "CMario.h"
#include "DefineMario.h"

#define LENGTH_SPRITES 5
#define LENGTH_ANI 2
#define LENGTH_ANIS 1

#define SECTION_UNKNOW 0
#define SECTION_SPRITE 1
#define SECTION_ANI 2
#define SECTION_ANIS 3

#define DEFAULT_TIME 100

CMario::CMario() {
	_position = { 0,0 };
	_state = STATE_MARIO_IDLE;
	_level = LEVEL_SMALL;
	_currentAniId = NULL;
	_direction = DIRECTION_RIGHT;
	_tex = CTextures::GetInstance();
	_tex->Add(ID_IMG_MARIO, PATH_IMG_MARIO);
	_ani = new CAnimation(DEFAULT_TIME);
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

void CMario::_ParseSectionAni(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_ANI) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	int time = atoi(tokens[1].c_str());

	_ani->Add(id, time);
}

void CMario::_ParseSectionAnis(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_ANIS) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());

	CAnimations::GetInstance()->Add(id, _ani);
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
		if (line == "[ANI]") {
			section = SECTION_ANI;
			continue;
		}
		if (line == "[ANIS]") {
			section = SECTION_ANIS;
			continue;
		}

		if (line == "[NEW_ANI]") {
			_ani = new CAnimation(DEFAULT_TIME);
			continue;
		}

		if (line=="[END]") {
			_ani = new CAnimation(DEFAULT_TIME);
			section = SECTION_UNKNOW;
			_currentAniId = ID_SMALL_MARIO_ANI_IDLE_LEFT;
			continue;
		}

		switch (section)
		{
		case SECTION_SPRITE:
			_ParseSectionSprites(line);
			break;
		case SECTION_ANI:
			_ParseSectionAni(line);
		case SECTION_ANIS:
			_ParseSectionAnis(line);
			break;
		default:
			break;
		}
	}
}

void CMario::Update(DWORD dt, vector<CGameObject*>* collidableObjects) {
	//_position.y += dt * MARIO_GRAVITY;
}

void CMario::Render() {
	if (_currentAniId != NULL) {
		CAnimations::GetInstance()->Get(_currentAniId)->Render(ID_IMG_MARIO, _position.x, _position.y);
	}
}

void CMario::Release() {

}