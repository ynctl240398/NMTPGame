#include "CObjKoopaTropa.h"
#include "CBrick.h"
#include "CPlatform.h"
#include "CKoopaParaTropa.h"
#include "CMario.h"

CObjKoopaTropa::CObjKoopaTropa(CKoopaParaTropa* koopa) {
	_koopa = koopa;
}

void CObjKoopaTropa::Render() {
	RenderBoundingBox();
	RenderBoundingBox();
	RenderBoundingBox();
	RenderBoundingBox();
}

void CObjKoopaTropa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	_koopa->GetPosition(_position.x, _position.y);
	_position.y += 16;
	_position.x += _koopa->GetVelocity().x > 0 ? 8 : -8;

	float l, t, r, b;
	float sl, st, sr, sb;

	bool turnBack = true;

	GetBoundingBox(l, t, r, b);
	for (LPGAMEOBJECT var : *coObjects)
	{
		if (var == _koopa || var == CMario::GetInstance()) {
			continue;
		}
		var->GetBoundingBox(sl, st, sr, sb);
		if (CCollision::IsOverlap(l, t, r, b, sl, st, sr, sb)) {
			turnBack = false;
		}
	}

	if (turnBack) {
		float vx, vy;
		_koopa->GetVelocity(vx, vy);
		_koopa->SetVelocity({ vx * -1, vy });
	}
}


void CObjKoopaTropa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _position.x - OBJ_BBOX_WIDTH / 2;
	top = _position.y - OBJ_BBOX_HEIGHT / 2;
	right = left + OBJ_BBOX_WIDTH;
	bottom = top + OBJ_BBOX_HEIGHT;
}