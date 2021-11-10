#pragma once

//keyboard
#define DIRECTINPUT_VERSION 0x0800

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <codecvt>

#include <Windows.h>
#include <D3DX10.h>
#include <d3d10.h>

#include <dinput.h>

#define BACKGROUND_COLOR D3DXCOLOR(156.0f/255.0f, 252.0f/255.0f, 240.0f/255.0f, 1.0f)
//#define BACKGROUND_COLOR D3DXCOLOR(255.0f, 255.0f, 255.0f, 0.0f)
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 224
#define MAX_SCENE_LINE 1024

//ID_IMG
#define ID_IMG_BBOX -100		// special texture to draw object bounding box
#define ID_IMG_TITLE_SET 000
#define ID_IMG_MARIO 100
#define ID_IMG_ITEM 200
#define ID_IMG_ENEMIES 300

#define DIF 8

//mario 1xxx
//brick_question 2xxx
//item 3xxx
//goomba 4xxx
//para_goomba 5xxx

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
LPCWSTR ToLPCWSTR(string st);