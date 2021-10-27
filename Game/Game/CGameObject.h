#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

class CGameObject;
class CEntity;

class CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;

class CRect {
	float _left, _top, _right, _bottom;
public:
	CRect() {
		_left = _top = _right = _bottom = 0;
	}
	CRect(float l, float t, float r, float b) {
		_left = l;
		_top = t;
		_right = r;
		_bottom = b;
	}
	float GetLeft() { return _left; }
	float GetTop() { return _top; }
	float GetRight() { return _right; }
	float GetBottom() { return _bottom; }

	void SetLeft(float f) { _left = f; }
	void SetTop(float f) { _top = f; }
	void SetRight(float f) { _right = f; }
	void SetBottom(float f) { _bottom = f; }
};

typedef CRect* LPRECTCUSTOM;

class CBoundingBox {

private:
	vector<CRect*> _rects;
public:
	CRect* GetBoundingBox(int index = 0) const {
		return _rects.empty() ? new CRect() : _rects.at(index);
	}
	float GetBoxOffsetX(int index = 0) const {
		return GetBoundingBox(index)->GetLeft();
	}
	float GetBoxOffsetY(int index = 0) const {
		return GetBoundingBox(index)->GetTop();
	}
	float GetBoxWidth(int index = 0) const {
		return GetBoundingBox(index)->GetRight();
	}
	float GetBoxHeight(int index = 0) const {
		return GetBoundingBox(index)->GetBottom();
	}

	void AddHitbox(CRect* rect) { _rects.emplace_back(rect); }
};

class CCollisionEvent {
public:
	CGameObject* gameOject;
	CEntity* entity;
	D3DXVECTOR2 normal;
	D3DXVECTOR2 distance;
	float time;
	CCollisionEvent(CGameObject*& obj, D3DXVECTOR2 norm, D3DXVECTOR2 dist, float t) {
		gameOject = obj;
		normal = norm;
		distance = dist;
		time = t;
	}

	CCollisionEvent(CEntity*& ent, D3DXVECTOR2 norm, D3DXVECTOR2 dist, float t) {
		entity = ent;
		normal = norm;
		distance = dist;
		time = t;
	}

	static bool CompareCollisionEvent(const LPCOLLISIONEVENT& a, const LPCOLLISIONEVENT& b) {
		return a->time < b->time;
	}
};

class CGameObject
{
public:
	enum class GameObjectType {
		//Players
		GAMEOBJECT_TYPE_MARIO = 0,
		GAMEOBJECT_TYPE_LUIGI = 1,
		//NPCs
		GAMEOBJECT_TYPE_GOOMBA = 100,
		GAMEOBJECT_TYPE_PARAGOOMBA = 101,
		GAMEOBJECT_TYPE_KOOPA = 102,
		GAMEOBJECT_TYPE_PARAKOOPA = 103,
		GAMEOBJECT_TYPE_PIRAPLANT = 104,
		GAMEOBJECT_TYPE_VENUSPLANT = 105,
		GAMEOBJECT_TYPE_BOOMERBRO = 106,
		//Projectiles (P - Player, V - Venus)
		GAMEOBJECT_TYPE_PFIREBALL = 107,
		GAMEOBJECT_TYPE_VFIREBALL = 108,
		GAMEOBJECT_TYPE_BOOMERANG = 109,
		//Special entities
		GAMEOBJECT_TYPE_TAIL = 197,
		GAMEOBJECT_TYPE_PORTAL = 198,
		GAMEOBJECT_TYPE_MOVINGPLATFORM = 199,
		//Items
		GAMEOBJECT_TYPE_RMUSHROOM = 200,
		GAMEOBJECT_TYPE_GMUSHROOM = 201,
		GAMEOBJECT_TYPE_LEAF = 202,
		GAMEOBJECT_TYPE_FLOWER = 203,
		GAMEOBJECT_TYPE_STAR = 204,
		GAMEOBJECT_TYPE_COIN = 205,
		GAMEOBJECT_TYPE_BONUSITEM = 206,
		//Animated blocks
		GAMEOBJECT_TYPE_QUESTIONBLOCK = 300,
		GAMEOBJECT_TYPE_SHINYBRICK = 301,
		GAMEOBJECT_TYPE_PBLOCK = 302,
		//Overworld NPCs
		GAMEOBJECT_TYPE_CACTUS = 400,
		GAMEOBJECT_TYPE_HELPTEXT = 401,
		GAMEOBJECT_TYPE_HAMMERBRO = 402,
		//Intro items
		GAMEOBJECT_TYPE_LOGO = 500,
		GAMEOBJECT_TYPE_ICON = 501,
		GAMEOBJECT_TYPE_SELECT = 502,
		GAMEOBJECT_TYPE_CURTAIN = 503,
		GAMEOBJECT_TYPE_PROPPLANT = 504,
		GAMEOBJECT_TYPE_BUZZYBEETLE = 505,
		GAMEOBJECT_TYPE_PROPMARIO = 506,
		GAMEOBJECT_TYPE_PROPLUIGI = 507,
		GAMEOBJECT_TYPE_PROPFASTKOOPA = 508,
		GAMEOBJECT_TYPE_PROPKOOPASHELL = 509,
		GAMEOBJECT_TYPE_PROPNORMALKOOPA = 510,
		//Effects
		GAMEOBJECT_TYPE_SCOREEFFECT = 600,
		GAMEOBJECT_TYPE_BRICKEFFECT = 601,
		GAMEOBJECT_TYPE_SMOKEEFFECT = 602,
		GAMEOBJECT_TYPE_SPARKEFFECT = 603,
		//Special objects
		GAMEOBJECT_TYPE_ONEHITPLATFORM = 666,
		GAMEOBJECT_TYPE_TILE = 777,
		GAMEOBJECT_TYPE_ONEWAYPLATFORM = 999
	};

protected:
	bool _isActive;

	GameObjectType _objectType;

	CBoundingBox _boundingBox;

	DWORD _deltaTime;

	D3DXVECTOR2 _velocity;
	D3DXVECTOR2 _distance;
	D3DXVECTOR2 _normal;

	D3DXVECTOR2 _position;
	D3DXVECTOR2 _rotation;
	D3DXVECTOR2 _translation;
	D3DXVECTOR2 _scale;

public:

	//If true, the entity is removed from the container
	//But its resources will not be released
	//Useful for when an object needs to know when another object is ready to be removed
	bool flaggedForRemoval;
	//Pass throughable entities are entities that
	//Other entities can pass through without blocking their velocity
	bool isPassThroughable;

	CGameObject();
	virtual ~CGameObject();

	//Is this Java?
	void SetActive(bool);
	bool IsActive() const;

	void SetOjectType(GameObjectType);
	GameObjectType GetObjectType() const;

	virtual CRect GetBoundingBox(int = 0) const;
	float GetBoxWidth(int = 0) const;
	float GetBoxHeight(int = 0) const;

	virtual void SetVelocity(D3DXVECTOR2);
	D3DXVECTOR2 GetVelocity() const;
	virtual void SetDistance(D3DXVECTOR2);
	D3DXVECTOR2 GetDistance() const;
	virtual void SetNormal(D3DXVECTOR2);
	D3DXVECTOR2 GetNormal() const;
	virtual void SetPosition(D3DXVECTOR2);
	D3DXVECTOR2 GetPosition() const;
	virtual void SetRotation(D3DXVECTOR2);
	D3DXVECTOR2 GetRotation() const;
	virtual void SetTranslation(D3DXVECTOR2);
	D3DXVECTOR2 GetTranslation() const;
	virtual void SetScale(D3DXVECTOR2);
	D3DXVECTOR2 GetScale() const;

	//Moving object, static object, distance of moving object, normal, time
	void SweptAABB(CRect obj1, CRect obj2, D3DXVECTOR2, D3DXVECTOR2&, float&);
	//Filter the collision between objects based on the shortest time
	void FilterCollision(const vector<LPCOLLISIONEVENT>&, vector<LPCOLLISIONEVENT>&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&);

	virtual void Update(DWORD, vector<CGameObject*>* = nullptr);
	virtual void Render();

	virtual void Release() = 0;
};

