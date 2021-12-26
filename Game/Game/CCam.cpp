#include "CCam.h"
#include "CUtil.h"
#include "CKeyBoard.h"
#include "CGame.h"
#include "CMario.h"

#define LENGTH_NUMBER 7

CCam* CCam::_cameraInstance = NULL;

CCam::CCam() {
	_cameraWidth = 0;
	_cameraHeight = 0;
}

CCam::~CCam() {
	_cameraBounds.clear();

	if (_cameraInstance) {
		delete _cameraInstance;
		_cameraInstance = NULL;
	}
}

CCam* CCam::GetInstance() {
	if (_cameraInstance == NULL) {
		_cameraInstance = new CCam;
	}
	return _cameraInstance;
}

void CCam::SetCameraWidth(unsigned int cameraWidth) {
	_cameraWidth = cameraWidth;
}

unsigned int CCam::GetCameraWidth() const {
	return _cameraWidth;
}

void CCam::SetCameraHeight(unsigned int cameraHeight) {
	_cameraHeight = cameraHeight;
}

unsigned int CCam::GetCameraHeight() const {
	return _cameraHeight;
}

float CCam::GetUpVector(int index) const {
	return _upVectors.at(index);
}

void CCam::GetBoundingBox(float& left, float& top, float& right, float& bottom) {

}

void CCam::Load(LPCWSTR filePath) {
	ifstream f;
	f.open(filePath);
	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#' || line.empty()) continue; //comment line

		vector<string> tokens = split(line);
		if (tokens.size() < LENGTH_NUMBER) return; // skip invalid lines

		float l = stof(tokens[0].c_str());
		float t = stof(tokens[1].c_str());
		float r = stof(tokens[2].c_str());
		float b = stof(tokens[3].c_str());
		float x = stof(tokens[4].c_str());
		float y = stof(tokens[5].c_str());
		float upVector = stof(tokens[6].c_str());

		CRect* cameraBound = new CRect(l, t, r, b);

		SetPosition({ x, y });
		AddUpVector(upVector);

		AddCameraBound(cameraBound);
	}
	f.close();
}

void CCam::AddUpVector(float upVector) {
	_upVectors.emplace_back(upVector);
}

LPRECTCUSTOM CCam::GetViewport() const {
	float left = _position.x - SCREEN_WIDTH * 0.2f;
	float top = _position.y - SCREEN_HEIGHT * 0.2f;
	float right = _position.x + SCREEN_WIDTH * 1.2f;
	float bottom = _position.y + SCREEN_HEIGHT * 1.2f;
	return new CRect(left, top, right, bottom);
}

LPRECTCUSTOM CCam::GetCameraBound(int index) const {
	if (index < 0) {
		return new CRect(0, 0, 0, 0);
	}
	return _cameraBounds.at(index);
}

void CCam::AddCameraBound(LPRECTCUSTOM cameraBound) {
	_cameraBounds.emplace_back(cameraBound);
}

void CCam::Update(DWORD dt, vector<CGameObject*>* collidableObjects) {
	if (CMario::GetInstance() == nullptr) {
		return;
	}

	int _sceneHeight = 656;

	LPRECTCUSTOM cameraBound = GetCameraBound();

	D3DXVECTOR2 cameraPosition = CMario::GetInstance()->GetPosition();
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

	_position = cameraPosition;
	_position.x -= 8;
}


void CCam::Render() {
	
}