#pragma once

#include "CGameObject.h"
#include "CDebug.h"
#include "CUtil.h"

#define ID_ANI_HUB_BLACK			13000
#define ID_ANI_HUB_BOARD			13001
#define ID_ANI_HUB_ARROW_BLACK		13002
#define ID_ANI_HUB_ARROW_WHITE		13003
#define ID_ANI_HUB_P_BLACK			13004
#define ID_ANI_HUB_P_WHITE			13005
#define ID_ANI_HUB_P_FLICK			13006

class CHub
{
private:
	static CHub* __instance;

	int _pLevel;
	bool _maxP;

	float _arrowX[6] = { 1, 2, 3, 4, 5, 6 };
public:
	CHub();

	void Update(DWORD dt);
	void Render();

	static CHub* GetInstance();
};

