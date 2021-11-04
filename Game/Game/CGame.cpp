#include "CDebug.h"
#include "CGame.h"
#include "CUtil.h"

#include "CSprite.h"
#include "CAnimation.h"
#include "CPlayScene.h"

#define MAX_GAME_LINE 1024


#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SETTINGS 1
#define GAME_FILE_SECTION_SCENES 2
#define GAME_FILE_SECTION_TEXTURES 3

CGame* CGame::__instance = NULL;

HWND CGame::_hWND = NULL;
HWND CGame::_contentHWND = NULL;

unsigned int CGame::windowAdjustX = 0;
unsigned int CGame::windowAdjustY = 0;

CGame::CGame() {
	this->_pipeline = CPipeLine::GetInstance();
	this->_keyboard = CKeyBoardCustom::GetInstance();
}

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for
	rendering 2D images
	- hWnd: Application window handle
*/
bool CGame::Init(HWND hWND)
{
	_isRunning = true;

	RECT window;
	GetClientRect(hWND, &window);

	_backBufferWidth = window.right + 1;
	_backBufferHeight = window.bottom + 1;

	_windowWidth = window.right + 1;
	_windowHeight = window.bottom + 1;

	if (!_pipeline->CreateDeviceAndSwapChain(_contentHWND, _backBufferWidth, _backBufferHeight, FRAME_RATE)) {
		return false;
	}

	if (!_pipeline->CreateRenderTagetView()) {
		return false;
	}

	//OPTIONAL:
	//If you want to flip sprites
	//Create a rasterizer state and disable culling
	//Source: https://gamedev.net/forums/topic/541543-cull-disable-dx10/4493656/
	if (!_pipeline->CreateRasterizerState()) {
		return false;
	}

	if (!_pipeline->CreateViewport(_backBufferWidth, _backBufferHeight)) {
		return false;
	}

	if (!_pipeline->CreateBlendState()) {
		return false;
	}

	if (!_pipeline->PipeCreateFont(15)) {
		return false;
	}

	/*if (_keyboard->InitKeyboard(hWND)) {
		MessageBoxA(hWND, "init keyboard failed", "Error", MB_ICONERROR);
		return false;
	}*/

	return true;
}

/*
	Draw the whole texture or part of texture onto screen
	NOTE: This function is very inefficient because it has to convert
	from texture to sprite every time we need to draw it
*/
void CGame::Draw(float x, float y, LPTEXTURE tex, RECT* rect, float alpha, int sprite_width, int sprite_height)
{
	if (tex == NULL) return;

	int spriteWidth = 0;
	int spriteHeight = 0;

	D3DX10_SPRITE sprite;

	// Set the sprite’s shader resource view
	sprite.pTexture = tex->getShaderResourceView();

	if (rect == NULL)
	{
		// top-left location in U,V coords
		sprite.TexCoord.x = 0;
		sprite.TexCoord.y = 0;

		// Determine the texture size in U,V coords
		sprite.TexSize.x = 1.0f;
		sprite.TexSize.y = 1.0f;

		spriteWidth = tex->getWidth();
		spriteHeight = tex->getHeight();
	}
	else
	{
		sprite.TexCoord.x = rect->left / (float)tex->getWidth();
		sprite.TexCoord.y = rect->top / (float)tex->getHeight();

		spriteWidth = (rect->right - rect->left + 1);
		spriteHeight = (rect->bottom - rect->top + 1);

		sprite.TexSize.x = spriteWidth / (float)tex->getWidth();
		sprite.TexSize.y = spriteHeight / (float)tex->getHeight();
	}

	// Set the texture index. Single textures will use 0
	sprite.TextureIndex = 0;

	// The color to apply to this sprite, full color applies white.
	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);

	//
	// Build the rendering matrix based on sprite location 
	//

	// The translation matrix to be created
	D3DXMATRIX matTranslation;

	// Create the translation matrix
	D3DXMatrixTranslation(&matTranslation, x, (_backBufferHeight - y), 0.1f);

	// Scale the sprite to its correct width and height because by default, DirectX draws it with width = height = 1.0f 
	D3DXMATRIX matScaling;
	D3DXMatrixScaling(&matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);

	// Setting the sprite’s position and size
	sprite.matWorld = (matScaling * matTranslation);

	CUtil::SpriteHandler->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

/*
	Utility function to wrap D3DXCreateTextureFromFileEx
*/
LPTEXTURE CGame::LoadTexture(LPCWSTR texturePath)
{
	ID3D10Resource* pD3D10Resource = NULL;
	ID3D10Texture2D* tex = NULL;

	// Loads the texture into a temporary ID3D10Resource object
	HRESULT hr = D3DX10CreateTextureFromFile(CUtil::DirectDevice,
		texturePath,
		NULL, //&info,
		NULL,
		&pD3D10Resource,
		NULL);

	// Make sure the texture was loaded successfully
	if (FAILED(hr))
	{
		DebugOut((wchar_t*)L"[ERROR] Failed to load texture file: %s with error: %d\n", texturePath, hr);
		return NULL;
	}

	// Translates the ID3D10Resource object into a ID3D10Texture2D object
	pD3D10Resource->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&tex);
	pD3D10Resource->Release();

	if (!tex) {
		DebugOut((wchar_t*)L"[ERROR] Failed to convert from ID3D10Resource to ID3D10Texture2D \n");
		return NULL;
	}

	//
	// Create the Share Resource View for this texture 
	// 	   
	// Get the texture details
	D3D10_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	// Create a shader resource view of the texture
	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	// Clear out the shader resource view description structure
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));

	// Set the texture format
	SRVDesc.Format = desc.Format;

	// Set the type of resource
	SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;

	ID3D10ShaderResourceView* gSpriteTextureRV = NULL;

	CUtil::DirectDevice->CreateShaderResourceView(tex, &SRVDesc, &gSpriteTextureRV);

	//DebugOut(L"[INFO] Texture loaded Ok from file: %s \n", texturePath);

	return new CTexture(tex, gSpriteTextureRV);
}

CGame::~CGame()
{
	if (_pipeline != NULL) {
		_pipeline->Release();
	}
}

CGame* CGame::GetInstance()
{
	if (__instance == NULL) {
		__instance = new CGame();
	}
	return __instance;
}

LRESULT CGame::_WinProc(HWND hWND, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		//https://stackoverflow.com/questions/28968012/win32-add-black-borders-to-fullscreen-window
		//You have no idea how long it took just to find this
	case WM_SIZE:
	{
		const SIZE ar = { SCREEN_WIDTH,  SCREEN_HEIGHT };
		//Query new client area size
		int clientWidth = LOWORD(lParam);
		int clientHeight = HIWORD(lParam);
		//Calculate new content size
		int contentWidth = MulDiv(clientHeight, ar.cx, ar.cy);
		int contentHeight = MulDiv(clientWidth, ar.cy, ar.cx);

		//Adjust dimensions to fit inside client area
		if (contentWidth > clientWidth) {
			contentWidth = clientWidth;
			contentHeight = MulDiv(contentWidth, ar.cy, ar.cx);
		}
		else {
			contentHeight = clientHeight;
			contentWidth = MulDiv(contentHeight, ar.cx, ar.cy);
		}

		//Calculate offsets to center content
		int offsetX = (clientWidth - contentWidth) / 2;
		int offsetY = (clientHeight - contentHeight) / 2;

		//Adjust content window position
		SetWindowPos(
			_contentHWND,
			nullptr,
			offsetX,
			offsetY,
			contentWidth,
			contentHeight,
			SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER
		);
	}
	break;
	case WM_SIZING:
		_ResizeWindow(static_cast<int>(wParam), *reinterpret_cast<LPRECT>(lParam));
		break;
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(lParam);
		info->ptMinTrackSize.y =
			((info->ptMinTrackSize.x - CGame::windowAdjustX) *
				RATIO_Y) / RATIO_X + CGame::windowAdjustY;
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CHAR:
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
	}

	return DefWindowProc(hWND, message, wParam, lParam);
}

LRESULT CGame::_WinProcContent(HWND hWND, UINT message, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hWND, message, wParam, lParam);
}

void CGame::_ResizeWindow(int edge, RECT& rect) {
	int size_x_desired = (rect.right - rect.left) - windowAdjustX;
	int size_y_desired = (rect.bottom - rect.top) - windowAdjustY;

	switch (edge) {
	case WMSZ_BOTTOM:
	case WMSZ_TOP:
	{
		int size_x = windowAdjustX + (size_y_desired * RATIO_X) / RATIO_Y;
		rect.left = (rect.left + rect.right) / 2 - size_x / 2;
		rect.right = rect.left + size_x;
	}
	break;
	case WMSZ_BOTTOMLEFT:
	{
		int size_x, size_y;

		if (size_x_desired * RATIO_Y > size_y_desired * RATIO_X)
		{
			size_x = rect.right - rect.left;
			size_y = windowAdjustY + ((size_x - windowAdjustX) * RATIO_Y) / RATIO_X;
		}
		else
		{
			size_y = rect.bottom - rect.top;
			size_x = windowAdjustX + ((size_y - windowAdjustY) * RATIO_X) / RATIO_Y;
		}

		rect.left = rect.right - size_x;
		rect.bottom = rect.top + size_y;
	}
	break;
	case WMSZ_BOTTOMRIGHT:
	{
		int size_x, size_y;

		if (size_x_desired * RATIO_Y > size_y_desired * RATIO_X)
		{
			size_x = rect.right - rect.left;
			size_y = windowAdjustY + ((size_x - windowAdjustX) * RATIO_Y) / RATIO_X;
		}
		else
		{
			size_y = rect.bottom - rect.top;
			size_x = windowAdjustX + ((size_y - windowAdjustY) * RATIO_X) / RATIO_Y;
		}

		rect.right = rect.left + size_x;
		rect.bottom = rect.top + size_y;
	}
	break;
	case WMSZ_LEFT:
	case WMSZ_RIGHT:
	{
		int size_y = windowAdjustY + (size_x_desired * RATIO_Y) / RATIO_X;
		rect.top = (rect.top + rect.bottom) / 2 - size_y / 2;
		rect.bottom = rect.top + size_y;
	}
	break;
	case WMSZ_TOPLEFT:
	{
		int size_x, size_y;

		if (size_x_desired * RATIO_Y > size_y_desired * RATIO_X)
		{
			size_x = rect.right - rect.left;
			size_y = windowAdjustY + ((size_x - windowAdjustX) * RATIO_Y) / RATIO_X;
		}
		else
		{
			size_y = rect.bottom - rect.top;
			size_x = windowAdjustX + ((size_y - windowAdjustY) * RATIO_X) / RATIO_Y;
		}

		rect.left = rect.right - size_x;
		rect.top = rect.bottom - size_y;
	}
	break;
	case WMSZ_TOPRIGHT:
	{
		int size_x, size_y;

		if (size_x_desired * RATIO_Y > size_y_desired * RATIO_X)
		{
			size_x = rect.right - rect.left;
			size_y = windowAdjustY + ((size_x - windowAdjustX) * RATIO_Y) / RATIO_X;
		}
		else
		{
			size_y = rect.bottom - rect.top;
			size_x = windowAdjustX + ((size_y - windowAdjustY) * RATIO_X) / RATIO_Y;
		}

		rect.right = rect.left + size_x;
		rect.top = rect.bottom - size_y;
	}
	break;
	}
}

HWND CGame::CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = reinterpret_cast<WNDPROC>(_WinProc);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hIcon = reinterpret_cast<HICON>(LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	wndClass.hIconSm = reinterpret_cast<HICON>(LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = WINDOW_CLASS_NAME;
	RegisterClassEx(&wndClass);

	_hWND = CreateWindow(
		WINDOW_CLASS_NAME,
		MAIN_WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		ScreenWidth,
		ScreenHeight,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (_hWND == NULL) {
		OutputDebugStringA("[GAME] _hWND was nullptr\n");
		return FALSE;
	}

	_CreateContentWindow(hInstance);

	ShowWindow(_hWND, nCmdShow);
	UpdateWindow(_hWND);

	return _hWND;
}

void CGame::_CreateContentWindow(HINSTANCE hInstance) {
	WNDCLASSEX wndClass{ };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = reinterpret_cast<WNDPROC>(_WinProcContent);
	wndClass.hIcon = reinterpret_cast<HICON>(LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	wndClass.hIconSm = reinterpret_cast<HICON>(LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClass.lpszClassName = L"Content_Class";
	RegisterClassEx(&wndClass);

	_contentHWND = CreateWindow(
		L"Content_Class",
		nullptr,
		WS_CHILD | WS_VISIBLE,
		0,
		0,
		0,
		0,
		_hWND,
		nullptr,
		hInstance,
		nullptr
	);

	if (_contentHWND == nullptr) {
		OutputDebugStringA("[GAME] _contentHWND was nullptr\n");
		return;
	}
}

/*
	Load game campaign file and load/initiate first scene
*/
void CGame::Load(LPCWSTR gameFile)
{
	//DebugOut(L"[INFO] Start loading game file : %s\n", gameFile);

	ifstream f;
	f.open(gameFile);
	char str[MAX_GAME_LINE];

	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") { section = GAME_FILE_SECTION_SETTINGS; continue; }
		if (line == "[TEXTURES]") { section = GAME_FILE_SECTION_TEXTURES; continue; }
		if (line == "[SCENES]") { section = GAME_FILE_SECTION_SCENES; continue; }
		if (line[0] == '[')
		{
			section = GAME_FILE_SECTION_UNKNOWN;
			//DebugOut(L"[ERROR] Unknown section: %s\n", ToLPCWSTR(line));
			continue;
		}

		//
		// data section
		//
		switch (section)
		{
		case GAME_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GAME_FILE_SECTION_SCENES: _ParseSection_SCENES(line); break;
		case GAME_FILE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		}
	}
	f.close();

	//DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n", gameFile);

	SwitchScene();
}

void CGame::SwitchScene()
{
	if (next_scene < 0 || next_scene == current_scene) return;

	//DebugOut(L"[INFO] Switching to scene %d\n", next_scene);

	scenes[current_scene]->Unload();

	CSprites::GetInstance()->Clear();
	CAnimations::GetInstance()->Clear();

	current_scene = next_scene;
	LPSCENE s = scenes[next_scene];
	
	s->Load();
}

void CGame::InitiateSwitchScene(int scene_id)
{
	next_scene = scene_id;
}

void CGame::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start")
		next_scene = atoi(tokens[1].c_str());
	else {
		//DebugOut(L"[ERROR] Unknown game setting: %s\n", ToWSTR(tokens[0]).c_str());
	}
}

void CGame::_ParseSection_SCENES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	int id = atoi(tokens[0].c_str());
	LPCWSTR path = ToLPCWSTR(tokens[1]);   // file: ASCII format (single-byte char) => Wide Char

	LPSCENE scene = new CPlayScene(id, path);
	scenes[id] = scene;
}

void CGame::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);

	CTextures::GetInstance()->Add(texID, path.c_str());
}