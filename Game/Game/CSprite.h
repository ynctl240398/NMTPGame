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

	void _ScaleSprite(D3DXVECTOR2 scale, unsigned int alpha);

public:
	CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex);

	RECT GetBoundingBoxSprite() { return{ left, top, right, bottom }; }

	void Draw(float x, float y, D3DXVECTOR2, unsigned int, bool);
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
	void Clear();

	static CSprites* GetInstance();
};