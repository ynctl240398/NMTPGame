#pragma once

#include "CUtil.h"
#include "CDebug.h"
#include <unordered_map>

using namespace std;

class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;

struct CCollisionEvent
{
	LPGAMEOBJECT src_obj;		// source object : the object from which to calculate collision
	LPGAMEOBJECT obj;			// the target object

	float t, nx, ny, tl;

	float dx, dy;				// *RELATIVE* movement distance between this object and obj
	bool isDeleted;

	CCollisionEvent(float t, float tl, float nx, float ny, float dx = 0, float dy = 0,
		LPGAMEOBJECT obj = NULL, LPGAMEOBJECT src_obj = NULL)
	{
		this->t = t;
		this->tl = tl;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
		this->src_obj = src_obj;
		this->isDeleted = false;
	}

	int WasCollided() { return t >= 0.0f && t <= 1.0f && tl > 0; }

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};

class CCollision
{
	static CCollision* __instance;
	unordered_map<LPGAMEOBJECT, unordered_map<LPGAMEOBJECT, LPCOLLISIONEVENT>> database;
public:
	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny,
		float& tl);

	static bool IsOverlap(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float sl,			// static left
		float st,
		float sr,
		float sb);

	LPCOLLISIONEVENT SweptAABB(
		LPGAMEOBJECT objSrc,
		DWORD dt,
		LPGAMEOBJECT objDest);
	void Scan(
		LPGAMEOBJECT objSrc,
		DWORD dt,
		vector<LPGAMEOBJECT>* objDests,
		vector<LPCOLLISIONEVENT>& coEvents);

	D3DXVECTOR2 GetClampDistance(DWORD dt, LPGAMEOBJECT objSrc, vector<LPCOLLISIONEVENT> coEvents, D3DXVECTOR2& jet);

	void Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Clear();

	static CCollision* GetInstance();
};