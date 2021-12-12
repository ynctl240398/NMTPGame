#include "CBrick.h"

CBrick::CBrick(float x, float y, int w, int h, bool isBig) {
	_position = { x + (w / 2) - DIF, y + h / 2 - DIF };
	_width = w;
	_height = h;
	_isBig = isBig;
}

void CBrick::Load(LPCWSTR filePath) {

}

void CBrick::Render() {
	RenderBoundingBox();
}

void CBrick::Update(DWORD dt, vector<CGameObject*>* obj) {

}

void CBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = _isBig ? _position.x - _width / 2 : _position.x - _width / 2 + DIF / 2;
	top = _position.y - _height / 2;
	right = left + _width;
	bottom = top + _height;
}