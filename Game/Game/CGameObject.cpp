#include "CGameObject.h"
#include "CGame.h"
#include "CCam.h"
#include "CAnimation.h"

CGameObject::CGameObject() {
	_isActive = true;
	_normal = D3DXVECTOR2(1.0f, 1.0f);
	_scale = D3DXVECTOR2(1.0f, 1.0f);
	_isDeleted = false;
	_state = -1;
	_velocity = { 0.0f,0.0f };
	_handleNoCollisionX = false;
	_handleNoCollisionY = false;
	_startState = -1;
	_aniId = -1;
	_ax = 0;
	_ay = 0;
	_alpha = ALPHA_DEFAULT;
}

CGameObject::~CGameObject() {}

void CGameObject::SetActive(bool active) {
	if (_isActive != active) {
		_isActive = active;
	}
}

bool CGameObject::IsActive() const {
	return _isActive;
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(_position.x, _position.y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_IMG_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);

	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx = CCam::GetInstance()->GetPosition().x, cy = CCam::GetInstance()->GetPosition().y;

	//CGame::GetInstance()->Draw(p.x - cx, p.y - cy, bbox, &rect, BBOX_ALPHA);
}

void CGameObject::SetVelocity(D3DXVECTOR2 velocity) {
	_velocity = velocity;
}

D3DXVECTOR2 CGameObject::GetVelocity() const {
	return _velocity;
}

void CGameObject::SetDistance(D3DXVECTOR2 distance) {
	_distance = distance;
}

D3DXVECTOR2 CGameObject::GetDistance() const {
	return _distance;
}

void CGameObject::SetNormal(D3DXVECTOR2 normal) {
	_normal = normal;
}

D3DXVECTOR2 CGameObject::GetNormal()const {
	return _normal;
}

void CGameObject::SetPosition(D3DXVECTOR2 position) {
	_position = position;
}

D3DXVECTOR2 CGameObject::GetPosition() const {
	return _position;
}

void CGameObject::SetRotation(D3DXVECTOR2 rotation) {
	_rotation = rotation;
}

D3DXVECTOR2 CGameObject::GetRotation()const {
	return _rotation;
}

void CGameObject::SetTranslation(D3DXVECTOR2 translation) {
	_translation = translation;
}

D3DXVECTOR2 CGameObject::GetTranslation()const {
	return _translation;
}

void CGameObject::SetScale(D3DXVECTOR2 scale) {
	_scale = scale;
}

D3DXVECTOR2 CGameObject::GetScale() const {
	return _scale;
}

void CGameObject::Update(DWORD dt, std::vector<CGameObject*>* collidableObjects) {

	CCollision::GetInstance()->Process(this, dt, collidableObjects);

	if (_handleNoCollisionX) {
		_handleNoCollisionX = false;
		_velocity.x += _ax * dt;
		_position.x += _velocity.x * dt;
	}
	if (_handleNoCollisionY) {
		_handleNoCollisionY = false;
		_velocity.y += _ay * dt;
		_position.y += _velocity.y * dt;
	}
}

void CGameObject::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	LPANIMATION animation = animations->Get(_aniId);
	if (animation == NULL) {
		return;
	}

	animations->Get(_aniId)->Render(_position.x, _position.y, _scale, _alpha);
	RenderBoundingBox();
}