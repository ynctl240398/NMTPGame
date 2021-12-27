#include "CGameObject.h"

#define OBJ_BBOX_WIDTH 5
#define OBJ_BBOX_HEIGHT 5

class CKoopaParaTropa;

class CObjKoopaTropa :public CGameObject
{
	CKoopaParaTropa* _koopa;

public:
	CObjKoopaTropa(CKoopaParaTropa* koopa);

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};