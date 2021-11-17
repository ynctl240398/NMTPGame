#include "CGameObject.h"

#define OBJ_BBOX_WIDTH 5
#define OBJ_BBOX_HEIGHT 26

#define OBJ_GRAVITY 0.002f
#define OBJ_SPEED 0.05f

class CObjKoopaTropa :public CGameObject
{
	float _ay;
	float _ax;
	bool _isNoCollisionWithPlatform;
public:
	CObjKoopaTropa(float x, float y);

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	int IsCollidable()
	{
		return 1;
	}

	bool IsNoCollisionWithPlatform() { return _isNoCollisionWithPlatform; }

	void SetIsNoCollisionWithPlatform(bool value) { _isNoCollisionWithPlatform = value; }

	void Release() override {};

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};