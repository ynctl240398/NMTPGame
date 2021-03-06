#pragma once
#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>

#include "CTexture.h"
#include "CPipeLine.h"
#include "CKeyBoard.h"
#include "CScene.h"

#define MAX_FRAME_RATE 60
#define FRAME_RATE 60

#define WINDOW_CLASS_NAME L"Game"
#define MAIN_WINDOW_TITLE L"Mario"
#define WINDOW_ICON_PATH L"mario.ico"

#define RATIO_X 8
#define RATIO_Y 7


/*
	Our simple game framework
*/
class CPipeline;
class CGame
{

	static LRESULT CALLBACK _WinProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK _WinProcContent(HWND, UINT, WPARAM, LPARAM);

	CPipeLine* _pipeline;
	CKeyBoardCustom* _keyboard;

	static HWND _hWND;
	static HWND _contentHWND;

	static CGame* __instance;

	unsigned int _backBufferWidth;					// Backbuffer width & height, will be set during Direct3D initialization
	unsigned int _backBufferHeight;

	unsigned int _windowWidth;
	unsigned int _windowHeight;

	unsigned int _defaultSceneID;
	bool _isRunning;

	unordered_map<int, LPSCENE> scenes;
	int current_scene;
	int next_scene = -1;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);
	void _ParseSection_TEXTURES(string line);

	void _CreateContentWindow(HINSTANCE);
	static void _ResizeWindow(int, RECT&);

public:
	static unsigned int windowAdjustX;
	static unsigned int windowAdjustY;

	CGame();

	int GetWindowWidth() { return _windowWidth; }
	int GetWindowHeight() { return _windowHeight; }

	// Init DirectX, Sprite Handler
	bool Init(HWND hWnd);

	bool InitKeyboard(HWND hWnd){
		if (!_keyboard->InitKeyboard(hWnd)) {
			return false;
		}
		return true;
	}

	void ProcessKeyboardInputs() { _keyboard->ProcessKeyboardInputs(); }

	// Draw a portion or ALL the texture at position (x,y) on the screen
	// rect : if NULL, the whole texture will be drawn
	//        if NOT NULL, only draw that portion of the texture 
	void Draw(float x, float y, LPTEXTURE tex, RECT* rect = NULL, float alpha = 1.0f, int sprite_width = 0, int sprite_height = 0);

	void Draw(float x, float y, LPTEXTURE tex, int l, int t, int r, int b)
	{
		RECT rect;
		rect.left = l;
		rect.top = t;
		rect.right = r;
		rect.bottom = b;
		this->Draw(x, y, tex, &rect);
	}

	LPTEXTURE LoadTexture(LPCWSTR texturePath);

	int GetBackBufferWidth() { return _backBufferWidth; }
	int GetBackBufferHeight() { return _backBufferHeight; }

	CPipeLine* GetPipeLineInstance() {
		return _pipeline->GetInstance();
	}

	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void Load(LPCWSTR gameFile);
	void SwitchScene();
	void InitiateSwitchScene(int scene_id);

	HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int width, int height);
	
	static CGame* GetInstance();

	~CGame();
};
typedef CGame* LPGAME;