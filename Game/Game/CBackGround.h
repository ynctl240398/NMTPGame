#pragma once
#include "CTexture.h"
#include "CGameObject.h"

class CBackGround : public CGameObject
{
	CTextures* _tex;

	vector<pair<RECT, D3DXVECTOR2>> _sprites;

	D3DXMATRIX _scaleMatrix;
	D3DX10_SPRITE _sprite;

	void _ParseSectionSprites(string line);
	void _ScaleSprite(const RECT& spriteBound);
public:
	CBackGround();
	void Load(LPCWSTR filePath);
	void Render() override;
	void Release() override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	~CBackGround();
};