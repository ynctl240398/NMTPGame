#include "CHub.h"
#include "CAnimation.h"
#include "CMario.h"

#define BOARD_LEFT 264 / 2
#define BOARD_TOP 202

#define HUB_WIDTH 256
#define HUB_HEIGHT 16

#define HUB_X 0
#define HUB_Y 203

#define ARROW_Y 199

#define P_X 110.5
#define P_Y 199

CHub* CHub::__instance = NULL;

CHub::CHub()
{
    _pLevel = 0;
    _maxP = false;
}

void CHub::Update(DWORD dt)
{
    _pLevel = (int)min(floor(CMario::GetInstance()->powerMeter), 6);
    _maxP = CMario::GetInstance()->powerMeter >= 7;
}

void CHub::Render()
{
    for (int i = -8; i < HUB_WIDTH; i += 16)
    {
        for (int j = -8; j < HUB_HEIGHT; j += 16)
        {
            CAnimations::GetInstance()->Get(ID_ANI_HUB_BLACK)->Render((float)HUB_X + i, (float)HUB_Y + j, { 1, 1 }, 255, true);
        }
    }

    CAnimations::GetInstance()->Get(ID_ANI_HUB_BOARD)->Render(BOARD_LEFT, BOARD_TOP, { 1, 1 }, 255, true);

    for (int i = 0; i < _pLevel; i++)
    {
        CAnimations::GetInstance()->Get(ID_ANI_HUB_ARROW_WHITE)->Render(_arrowX[i], ARROW_Y, { 1, 1 }, 255, true);
    }
    if (_maxP) {
        CAnimations::GetInstance()->Get(ID_ANI_HUB_P_FLICK)->Render(P_X, P_Y, { 1, 1 }, 255, true);
    }
}

CHub* CHub::GetInstance()
{
    if (__instance == nullptr) {
        __instance = new CHub();
    }
    return __instance;
}
