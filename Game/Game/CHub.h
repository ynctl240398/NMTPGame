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

#define ARROW_POSITION_Y_LIST {61.5, 69.5, 77.5, 85.5, 93.5, 101.5}
#define LENGTH_ARROW    6

class CHub
{
private:
	static CHub* __instance;

	int _pLevel;
	bool _maxP;

	float _arrowX[LENGTH_ARROW] = ARROW_POSITION_Y_LIST;
public:
	CHub();

	void Update(DWORD dt);
	void Render();

	static CHub* GetInstance();
};

