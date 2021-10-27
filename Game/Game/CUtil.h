#pragma once

//keyboard
#define DIRECTINPUT_VERSION 0x0800

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <codecvt>

#include <D3DX10.h>
#include <d3d10.h>

#include <dinput.h>

#define BACKGROUND_COLOR D3DXCOLOR(156.0f/255.0f, 252.0f/255.0f, 240.0f/255.0f, 1.0f)
//#define BACKGROUND_COLOR D3DXCOLOR(255.0f, 255.0f, 255.0f, 0.0f)
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 224
#define MAX_SCENE_LINE 1024

//ID_IMG
#define ID_IMG_TITLE_SET 000
#define ID_IMG_MARIO 100

//PATH_IMG
#define PATH_IMG_TITLE_SET "textures\\tileset.png"
#define PATH_IMG_MARIO L"textures\\mario.png"

//PATH file
#define FILE_PATH_BG_WORLD_1 "textures//world1-1//bg.txt"
#define FILE_PATH_MARIO "textures//animation//mario.txt"
#define FILE_PATH_CAMERA "textures//world1-1//camera.txt"

using namespace std;

namespace CUtil {
	extern ID3D10Device* DirectDevice;
	extern ID3DX10Sprite* SpriteHandler;
	extern ID3DX10Font* Font;

	extern void GetFontRect(LPCSTR text, RECT* rect);

	extern void WriteTextToScreen(LPCSTR text, RECT* rect, UINT format, D3DXCOLOR fontColor);
}

wstring ToWSTR(string st);
vector<string> split(string line, string delimeter = "\t");