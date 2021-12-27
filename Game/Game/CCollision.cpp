#include "CCollision.h"
#include "CGameObject.h"
#include "CMario.h"
#include "CGoomba.h"

//#define BLOCK_PUSH_FACTOR 0.4f

CCollision* CCollision::__instance = NULL;

CCollision* CCollision::GetInstance()
{
	if (__instance == NULL) __instance = new CCollision();
	return __instance;
}

/*
	SweptAABB
*/
void CCollision::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny, float& tl)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;
	tl = 0;				// the measurement of collision surface, if that =0, no collision

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -999999.0f;
		tx_exit = 999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999.0f;
		ty_exit = 99999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry; //return t_entry

	// collide x or y first
	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f; //direction of response after collision

		float mst = mt + dy * t_entry;
		float msb = mb + dy * t_entry;
		tl = min(msb, sb) - max(mst, st); //touching length
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;

		float msl = ml + dx * t_entry;
		float msr = mr + dx * t_entry;
		tl = (min(msr, sr) - max(msl, sl));
	}

}

bool CCollision::IsOverlap(float ml, float mt, float mr, float mb, float sl, float st, float sr, float sb)
{
	return !(mr <= sl || ml >= sr || mt >= sb || mb <= st);
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CCollision::SweptAABB(LPGAMEOBJECT objSrc, DWORD dt, LPGAMEOBJECT objDest)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny, tl;

	float mvx, mvy;
	objSrc->GetVelocity(mvx, mvy);
	float mdx = mvx * dt;
	float mdy = mvy * dt;

	float svx, svy;
	objDest->GetVelocity(svx, svy);
	float sdx = svx * dt;
	float sdy = svy * dt;

	//
	// NOTE: new m speed = original m speed - collide object speed
	// 
	float dx = mdx - sdx;
	float dy = mdy - sdy;

	objSrc->GetBoundingBox(ml, mt, mr, mb);
	objDest->GetBoundingBox(sl, st, sr, sb);

	SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny, tl
	);

	CCollisionEvent* e = new CCollisionEvent(t, tl, nx, ny, dx, dy, objDest, objSrc);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects
	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CCollision::Scan(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* objDests, vector<LPCOLLISIONEVENT>& coEvents)
{
	vector<LPCOLLISIONEVENT> temp;

	for (UINT i = 0; i < objDests->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABB(objSrc, dt, objDests->at(i));

		if (e->WasCollided() == 1)
			temp.push_back(e);
		else
			delete e;
	}

	std::sort(temp.begin(), temp.end(), CCollisionEvent::compare);

	for (LPCOLLISIONEVENT coll : temp)
	{
		for (LPCOLLISIONEVENT result : coEvents) {
			if (!result->obj->IsBlocking(result)) {
				continue;
			}

			float sl, st, sr, sb;		// static object bbox
			float ml, mt, mr, mb;		// moving object bbox
			float t, nx, ny, tl;

			coll->obj->GetBoundingBox(sl, st, sr, sb);
			objSrc->GetBoundingBox(ml, mt, mr, mb);

			float mvx, mvy;
			objSrc->GetVelocity(mvx, mvy);
			float mdx = mvx * dt;
			float mdy = mvy * dt;

			float svx, svy;
			coll->obj->GetVelocity(svx, svy);
			float sdx = svx * dt;
			float sdy = svy * dt;

			//
			// NOTE: new m speed = original m speed - collide object speed
			// 
			float rdx = mdx - sdx;
			float rdy = mdy - sdy;

			//calc relative dx from previous calc
			if (coll->nx != 0) {
				rdy = rdy * result->t;
			}
			else {
				rdx = rdx * result->t;
			}

			SweptAABB(
				ml, mt, mr, mb,
				rdx, rdy,
				sl, st, sr, sb,
				t, nx, ny, tl
			);

			CCollisionEvent* e = new CCollisionEvent(t, tl, nx, ny, rdx, rdy, coll->obj);

			//no collision
			if (e->WasCollided() == 0) {
				coll->t = -100;

				if (database.count(e->obj) > 0) {
					database[e->obj].erase(objSrc);
				}
			}
			delete e;
		}

		if (coll->WasCollided() == 1)
		{
			coEvents.push_back(coll);
		}
		else
			delete coll;
	}
}

D3DXVECTOR2 CCollision::GetClampDistance(DWORD dt, LPGAMEOBJECT objSrc, vector<LPCOLLISIONEVENT> coEvents, D3DXVECTOR2& jet)
{
	float mvx, mvy;
	objSrc->GetVelocity(mvx, mvy);
	float mdx = mvx * dt;
	float mdy = mvy * dt;

	float min_tx = 1.0f;
	float min_ty = 1.0f;

	jet.x = 0.0f;
	jet.y = 0.0f;

	for (LPCOLLISIONEVENT c : coEvents)
	{
		if (!c->obj->IsBlocking(c)) continue;

		if (c->t < min_tx && c->nx != 0 && c->nx * mdx < 0.0f) {
			min_tx = c->t;
			jet.x = c->nx;
		}

		if (c->t < min_ty && c->ny != 0 && c->ny * mdy < 0.0f) {
			min_ty = c->t;
			jet.y = c->ny;
		}
	}

	return D3DXVECTOR2(min_tx * mdx, min_ty * mdy);
}

/*
*  Simple/Sample collision framework
*  NOTE: Student might need to improve this based on game logic
*/
void CCollision::Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	LPCOLLISIONEVENT colX = NULL;
	LPCOLLISIONEVENT colY = NULL;

	coEvents.clear();

	if (objSrc->IsCollidable())
	{
		Scan(objSrc, dt, coObjects, coEvents);

		for (auto x : database) {
			if (x.second.count(objSrc) > 0) {
				LPCOLLISIONEVENT dbItem = x.second[objSrc];
				bool has = false;
				for (LPCOLLISIONEVENT e : coEvents) {
					if (e->obj == dbItem->src_obj) {
						has = true;
						break;
					}
				}
				if (!has) {
					LPCOLLISIONEVENT e = new CCollisionEvent(dbItem->t, dbItem->tl, dbItem->nx * -1, dbItem->ny * -1, dbItem->dx, dbItem->dy, dbItem->src_obj, dbItem->obj);
					coEvents.push_back(e);
				}
			}
		}

		unordered_map<LPGAMEOBJECT, LPCOLLISIONEVENT> map;
		for (LPCOLLISIONEVENT e : coEvents) {
			map[e->obj] = e;
			if (database.count(e->obj) > 0) {
				if (database[e->obj].count(objSrc) == 0) {
					D3DXVECTOR2 sv = objSrc->GetVelocity();
					D3DXVECTOR2 ev = e->obj->GetVelocity();
					LPCOLLISIONEVENT e1 = new CCollisionEvent(e->t, e->tl, e->nx * -1, e->ny * -1, e->dx, e->dy, objSrc, e->obj);
					e->obj->OnCollisionWith(e1);
				}
			}
		}
		database[objSrc] = map;
	}

	D3DXVECTOR2 jet{ 0, 0 };
	D3DXVECTOR2 clampedDistance = GetClampDistance(dt, objSrc, coEvents, jet);

	objSrc->SetPosition(objSrc->GetPosition() + clampedDistance);

	if (coEvents.size() == 0)
	{
		objSrc->OnNoCollision(dt);
	}
	else
	{
		if (jet.x != 0) {
			objSrc->OnBlockingOn(true, jet.x);
		}

		if (jet.y != 0) {
			objSrc->OnBlockingOn(false, jet.y);
		}

		for (LPCOLLISIONEVENT e : coEvents)
		{
			objSrc->OnCollisionWith(e);
		}
	}
}

void CCollision::Clear()
{
	for (auto data : database) {
		for (auto item : data.second) {
			delete item.second;
		}
		data.second.clear();
	};
	database.clear();
}
