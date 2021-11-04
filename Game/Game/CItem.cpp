#include "CItem.h"
#include "CUtil.h"

#define SECTION_UNKNOW 0
#define SECTION_SPRITE 1
#define SECTION_FRAME 2
#define SECTION_ANIMATION 3

#define LENGTH_SPRITES 5
#define LENGTH_ANI 2
#define LENGTH_ANIS 1

#define DEFAULT_TIME 100

CItem::CItem(float x, float y, string type) {
	_position = { x,y };
	_isActive = false;
	_tex = CTextures::GetInstance();
	_type = type;
	_ani = new CAnimation(DEFAULT_TIME);
}

void CItem::_ParseSectionSprites(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_SPRITES) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = l + atoi(tokens[3].c_str());
	int b = t + atoi(tokens[4].c_str());

	CSprites::GetInstance()->Add(id, l, t, r, b, _tex->Get(ID_IMG_ITEM));
}

void CItem::_ParseSectionFrames(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_ANI) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	int time = atoi(tokens[1].c_str());

	_ani->Add(id, time);
}

void CItem::_ParseSectionAnimation(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_ANIS) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());

	CAnimations::GetInstance()->Add(id, _ani);

	_ani = new CAnimation(DEFAULT_TIME);
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

}

void CItem::Load(LPCWSTR filePath) {
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

void CItem::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	int aniId;
	if (_type == TYPE_MUSHROOM_RED) {
		aniId = ID_ITEM_MUSHROOM_RED_ANI;
	}
	else aniId = ID_ITEM_MUSHROOM_GREEN_ANI;

	LPANIMATION animation = animations->Get(aniId);

	if (animation == NULL) {
		return;
	}

	animations->Get(aniId)->Render(_position.x, _position.y, _scale);
	RenderBoundingBox();
}

void CItem::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = _position.x - ITEM_WIDTH / 2;
	top = _position.y - ITEM_HEIGHT / 2;
	right = left + ITEM_WIDTH;
	bottom = top + ITEM_HEIGHT;
}

void CItem::Release() {

}

CItem::~CItem() {

}