#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include <fstream>
#include <algorithm>

#include "CCollision.h"

#define BBOX_ALPHA 0.25f		// Bounding box transparency

using namespace std;

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

class CGameObject
{
protected:
	bool _isActive;
	bool _handleNoCollisionX;
	bool _handleNoCollisionY;

	D3DXVECTOR2 _velocity;
	D3DXVECTOR2 _distance;
	D3DXVECTOR2 _normal;

	D3DXVECTOR2 _position;
	D3DXVECTOR2 _rotation;
	D3DXVECTOR2 _translation;
	D3DXVECTOR2 _scale;
	D3DXVECTOR2 _startPostion;

	int _state;
	int _startState;

	bool _isDeleted;

public:

	CGameObject();
	~CGameObject();

	//Is this Java?
	void SetActive(bool);
	bool IsActive() const;

	virtual void SetState(int state) { this->_state = state; }
	int GetState() { return this->_state; }

	virtual void Delete() { _isDeleted = true; }
	bool IsDeleted() { return _isDeleted; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;

	void SetVelocity(D3DXVECTOR2);
	D3DXVECTOR2 GetVelocity() const;
	void GetVelocity(float& x, float& y) { x = _velocity.x; y = _velocity.y; }

	void SetDistance(D3DXVECTOR2);
	D3DXVECTOR2 GetDistance() const;

	void SetNormal(D3DXVECTOR2);
	D3DXVECTOR2 GetNormal() const;

	void SetPosition(D3DXVECTOR2);
	D3DXVECTOR2 GetPosition() const;
	void GetPosition(float& x, float& y) { x = _position.x; y = _position.y; }

	void SetRotation(D3DXVECTOR2);
	D3DXVECTOR2 GetRotation() const;

	void SetTranslation(D3DXVECTOR2);
	D3DXVECTOR2 GetTranslation() const;

	void SetScale(D3DXVECTOR2);
	D3DXVECTOR2 GetScale() const;

	D3DXVECTOR2 GetStartPostion() { return _startPostion; }
	int GetStartState() { return _startState; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() = 0;
	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};

	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }

	virtual void Release() = 0;

	void RenderBoundingBox();

	static bool IsDeleted(const LPGAMEOBJECT& o) { return o->_isDeleted; }
};

