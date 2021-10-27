#include "CGameObject.h"
#include "CEntity.h"

CGameObject::CGameObject() {
	_isActive = true;
	_normal = D3DXVECTOR2(1.0f, 1.0f);
	_scale = D3DXVECTOR2(1.0f, 1.0f);
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

void CGameObject::SetOjectType(GameObjectType objectType) {
	_objectType = objectType;
}

CGameObject::GameObjectType CGameObject::GetObjectType() const
{
	return _objectType;
}

CRect CGameObject::GetBoundingBox(int index) const {
	CRect rect;
	rect.SetLeft(_position.x + _boundingBox.GetBoxOffsetX(index));
	rect.SetTop(_position.y + _boundingBox.GetBoxOffsetY(index));
	rect.SetRight(rect.GetLeft() + _boundingBox.GetBoxWidth(index));
	rect.SetBottom(rect.GetTop() + _boundingBox.GetBoxHeight(index));
	return rect;
}

float CGameObject::GetBoxWidth(int index) const {
	return _boundingBox.GetBoxWidth(index);
}

float CGameObject::GetBoxHeight(int index)  const {
	return _boundingBox.GetBoxHeight(index);
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

void CGameObject::SweptAABB(
	CRect movingObject,
	CRect staticObject,
	D3DXVECTOR2 distance,
	D3DXVECTOR2& normal,
	float& time)
{
	D3DXVECTOR2 dEntry;
	D3DXVECTOR2 dExit;

	D3DXVECTOR2 tEntry;
	D3DXVECTOR2 tExit;

	float entry;
	float exit;

	//No collision
	time = -1.0f;
	normal = D3DXVECTOR2(0, 0);

	//Broad phase test
	CRect box;
	box.SetLeft(distance.x > 0 ? movingObject.GetLeft() : movingObject.GetLeft() + distance.x);
	box.SetTop(distance.y > 0 ? movingObject.GetTop() : movingObject.GetTop() + distance.y);
	box.SetRight(distance.x > 0 ? movingObject.GetRight() + distance.x : movingObject.GetRight());
	box.SetBottom(distance.y > 0 ? movingObject.GetBottom() + distance.y : movingObject.GetBottom());

	if (box.GetRight() < staticObject.GetLeft() ||
		box.GetLeft() > staticObject.GetRight() ||
		box.GetBottom() < staticObject.GetTop() ||
		box.GetTop() > staticObject.GetBottom())
	{
		return;
	}

	if (distance.x == 0.0f && distance.y == 0.0f) {
		return;
	}

	if (distance.x > 0) {
		dEntry.x = staticObject.GetLeft() - movingObject.GetRight();
		dExit.x = staticObject.GetRight() - movingObject.GetLeft();
	}
	else if (distance.x < 0) {
		dEntry.x = staticObject.GetRight() - movingObject.GetLeft();
		dExit.x = staticObject.GetLeft() - movingObject.GetRight();
	}

	if (distance.y > 0) {
		dEntry.y = staticObject.GetTop() - movingObject.GetBottom();
		dExit.y = staticObject.GetBottom() - movingObject.GetTop();
	}
	else if (distance.y < 0) {
		dEntry.y = staticObject.GetBottom() - movingObject.GetTop();
		dExit.y = staticObject.GetTop() - movingObject.GetBottom();
	}

	if (distance.x == 0) {
		tEntry.x = -std::numeric_limits<float>::infinity();
		tExit.x = std::numeric_limits<float>::infinity();
	}
	else {
		tEntry.x = dEntry.x / distance.x;
		tExit.x = dExit.x / distance.x;
	}

	if (distance.y == 0) {
		tEntry.y = -std::numeric_limits<float>::infinity();
		tExit.y = std::numeric_limits<float>::infinity();
	}
	else {
		tEntry.y = dEntry.y / distance.y;
		tExit.y = dExit.y / distance.y;
	}

	if ((tEntry.x < 0.0f && tEntry.y < 0.0f) ||
		tEntry.x > 1.0f ||
		tEntry.y > 1.0f)
	{
		return;
	}

	entry = tEntry.x > tEntry.y ? tEntry.x : tEntry.y;
	exit = tExit.x < tExit.y ? tExit.x : tExit.y;

	if (entry > exit) {
		return;
	}

	time = entry;

	if (tEntry.x > tEntry.y) {
		normal.y = 0.0f;
		distance.x > 0 ? normal.x = -1.0f : normal.x = 1.0f;
	}
	else {
		normal.x = 0.0f;
		distance.y > 0 ? normal.y = -1.0f : normal.y = 1.0f;
	}
}

void CGameObject::FilterCollision(
	const vector<LPCOLLISIONEVENT>& collisionEvents,
	vector<LPCOLLISIONEVENT>& eventsResult,
	D3DXVECTOR2& minTime,
	D3DXVECTOR2& normal,
	D3DXVECTOR2& relativeDistance)
{
	minTime = D3DXVECTOR2(1.0f, 1.0f);
	normal = D3DXVECTOR2(0.0f, 0.0f);

	int minIndX = -1, minIndY = -1;

	for (unsigned int i = 0; i < collisionEvents.size(); ++i) {
		LPCOLLISIONEVENT coEvent = collisionEvents.at(i);

		if (coEvent->entity->isPassThroughable) {
			continue;
		}

		if (coEvent->time < minTime.x && coEvent->normal.x != 0) {
			minTime.x = coEvent->time;
			normal.x = coEvent->normal.x;
			relativeDistance.x = coEvent->distance.x;
			minIndX = i;
		}

		if (coEvent->time < minTime.y && coEvent->normal.y != 0) {
			minTime.y = coEvent->time;
			normal.y = coEvent->normal.y;
			relativeDistance.y = coEvent->distance.y;
			minIndY = i;
		}
	}

	if (minIndX >= 0) {
		eventsResult.emplace_back(collisionEvents.at(minIndX));
	}

	if (minIndY >= 0) {
		eventsResult.emplace_back(collisionEvents.at(minIndY));
	}

	for (unsigned int i = 0; i < collisionEvents.size(); ++i) {
		LPCOLLISIONEVENT coEvent = collisionEvents.at(i);
		if (coEvent->entity->isPassThroughable) {
			eventsResult.emplace_back(coEvent);
		}
	}
}

void CGameObject::Update(DWORD dt, std::vector<CGameObject*>* collidableObjects) {
	_deltaTime = dt;
	_distance = _velocity * static_cast<float>(_deltaTime);
}

void CGameObject::Render() {}