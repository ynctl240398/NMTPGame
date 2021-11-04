#include "CBrick.h"

CBrick::CBrick(float x, float y, int w, int h, bool isBig) {
	_position = { x,y };
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

void CBrick::Release() {

}

void CBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = _position.x - _width / 2;
	top = _position.y - _height / 2;
	right = left + _width;
	bottom = top + _height;
}