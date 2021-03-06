#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>

#include "CBackGround.h"
#include "CGame.h"
#include "CCam.h"
#include "CUtil.h"
#include "CMario.h"
#include "CHub.h"

#define INIT_FILE_PATH L"textures\\init.txt"


void Update(DWORD dt)
{
	CGame::GetInstance()->GetCurrentScene()->Update(dt);
	CHub::GetInstance()->Update(dt);
}

void Render()
{
	CGame* g = CGame::GetInstance();

	auto device = CUtil::DirectDevice;
	auto spriteHandler = CUtil::SpriteHandler;

	auto swapChain = g->GetPipeLineInstance()->GetSwapChain();
	auto renderTargetView = g->GetPipeLineInstance()->GetRenderTargetView();
	auto blendState = g->GetPipeLineInstance()->GetBlendState();

	if (device != NULL)
	{
		// clear the background 
		device->ClearRenderTargetView(renderTargetView, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE | D3DX10_SPRITE_SAVE_STATE);

		// Use Alpha blending for transparent sprites
		//FLOAT NewBlendFactor[4] = { 0,0,0,0 };
		//device->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

		

		// Uncomment this line to see how to draw a porttion of a texture  
		//g->Draw(10, 10, texMisc, 300, 117, 316, 133);

		float newBlendFactor[4] = { 0.0f };
		device->OMSetBlendState(blendState, newBlendFactor, 0xffffffff);

		CGame::GetInstance()->GetCurrentScene()->Render();
		CHub::GetInstance()->Render();

		spriteHandler->End();
		swapChain->Present(0, 0);
	}
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		ULONGLONG dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			CGame::GetInstance()->ProcessKeyboardInputs();

			Update((DWORD)dt);
			Render();
			
			CGame::GetInstance()->SwitchScene();
		}
		else {
			Sleep((DWORD)(tickPerFrame - dt));
		}
	}

	return 1;
}

int __stdcall WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	CGame* gameInstance = CGame::GetInstance();

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	CGame::windowAdjustX = (rect.right - rect.left) - SCREEN_WIDTH;
	CGame::windowAdjustY = (rect.bottom - rect.top) - SCREEN_HEIGHT;

	HWND hWND = gameInstance->CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (!gameInstance->Init(hWND)) {
		MessageBoxA(hWND, "init game failed", "Error", MB_ICONERROR);
		return -1;
	};

	if (!gameInstance->InitKeyboard(hWND)) {
		MessageBoxA(hWND, "init keyboard failed", "Error", MB_ICONERROR);
		return -1;
	};

	gameInstance->Load(INIT_FILE_PATH);

	RECT windowRect;
	GetWindowRect(hWND, &windowRect);
	unsigned int xPos = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 3;
	unsigned int yPos = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom) / 4;

	SetWindowPos(hWND, nullptr, xPos, yPos, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2 , SWP_NOOWNERZORDER | SWP_NOZORDER);
	
	Run();

	return 0;
}