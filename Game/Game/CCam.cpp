#include "CCam.h"
#include "CUtil.h"
#include "CKeyBoard.h"

CCam* CCam::_cameraInstance = NULL;

CCam::CCam() {}

CCam::~CCam() {}

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
	return _cameraBounds.at(index);
}

void CCam::AddCameraBound(LPRECTCUSTOM cameraBound) {
	_cameraBounds.emplace_back(cameraBound);
}

void CCam::Update(DWORD dt, vector<CGameObject*>* collidableObjects) {
	CKeyBoardCustom* _keyboard = CKeyBoardCustom::GetInstance();
	if (_keyboard->IsKeyDown(DIK_RIGHTARROW)) {
		_position.x += dt * 0.1f;
	}
	else if (_keyboard->IsKeyDown(DIK_LEFTARROW)) {
		_position.x -= dt * 0.1f;
	}
}

void CCam::Release() {
	_cameraBounds.clear();

	if (_cameraInstance) {
		delete _cameraInstance;
		_cameraInstance = NULL;
	}
}