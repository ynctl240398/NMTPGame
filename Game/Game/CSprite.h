#pragma once

#include "CTexture.h"
#include "CGame.h"
#include "CCam.h"

class CSprite
{
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	CCam* _cameraInstance;

	LPTEXTURE texture;
	D3DX10_SPRITE _sprite;
	D3DXMATRIX _scaleMatrix;

	void _ScaleSprite(const RECT& spriteBound, int idTex);

public:
	CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex);

	RECT GetBoundingBoxSprite() { return{ left, top, left + right, top + bottom }; }

	void Draw(RECT rect, float x, float y, int idTex);
	void Release();
};

typedef CSprite* LPSPRITE;

class CSprites
{
	static CSprites* __instance;
	
	unordered_map<int, LPSPRITE> sprites;

public:
	void Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex);
	LPSPRITE Get(int id);

	static CSprites* GetInstance();
};