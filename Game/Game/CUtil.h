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

//#define BACKGROUND_COLOR D3DXCOLOR(156.0f/255.0f, 252.0f/255.0f, 240.0f/255.0f, 1.0f)
#define BACKGROUND_COLOR D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 256
#define MAX_SCENE_LINE 1024

//ID_IMG
#define ID_IMG_BBOX			-100		// special texture to draw object bounding box
#define ID_IMG_TITLE_SET	000
#define ID_IMG_MARIO		100
#define ID_IMG_ITEM			200
#define ID_IMG_ENEMIES		300
#define ID_IMG_PROJECTTILE	400
#define ID_IMG_PARTICLES	500
#define ID_IMG_HUB			600
#define ID_IMG_TREE			700

#define DIF 8


#define ID_ANI_POINT_100		12001
#define ID_ANI_POINT_200		12002
#define ID_ANI_POINT_400		12003
#define ID_ANI_POINT_800		12004
#define ID_ANI_POINT_1000		12005
#define ID_ANI_ATTACK_EFFECT	12006

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

void  AddPointAni(int aniId, float x, float y);
void  AddAttackEffect(float x, float y);