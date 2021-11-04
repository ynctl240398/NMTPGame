#include "CSprite.h"
#include "CUtil.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;

	// Set the sprite’s shader resource view
	_sprite.pTexture = tex->getShaderResourceView();

	_sprite.TexCoord.x = this->left / (float)tex->getWidth();
	_sprite.TexCoord.y = this->top / (float)tex->getHeight();

	int spriteWidth = (this->right - this->left + 1);
	int spriteHeight = (this->bottom - this->top + 1);

	_sprite.TexSize.x = spriteWidth / (float)tex->getWidth();
	_sprite.TexSize.y = spriteHeight / (float)tex->getHeight();

	_sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	_sprite.TextureIndex = 0;

	D3DXMatrixScaling(&this->_scaleMatrix, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);

	_cameraInstance = CCam::GetInstance();
}

void CSprite::_ScaleSprite(D3DXVECTOR2 scale, unsigned int alpha) {

	_sprite.TexCoord.x = left / static_cast<float>(texture->getWidth());
	_sprite.TexCoord.y = top / static_cast<float>(texture->getHeight());

	int spriteWidth = right - left;
	int spriteHeight = bottom - top;
	_sprite.TexSize.x = spriteWidth / static_cast<float>(texture->getWidth());
	_sprite.TexSize.y = spriteHeight / static_cast<float>(texture->getHeight());
	_sprite.ColorModulate = { 1.0f, 1.0f, 1.0f, alpha / 255.0f };
	_sprite.TextureIndex = 0;

	D3DXMatrixScaling(&_scaleMatrix, static_cast<float>(spriteWidth) * scale.x, static_cast<float>(spriteHeight) * scale.y, 1.0f);
}

void CSprite::Draw(float x, float y, D3DXVECTOR2 scale, unsigned int alpha)
{

	CGame* g = CGame::GetInstance();

	_ScaleSprite(scale, alpha);

	D3DXMATRIX matTranslation;

	float nx = x - _cameraInstance->GetPosition().x;
	float ny = (g->GetBackBufferHeight() - y) + _cameraInstance->GetPosition().y;

	D3DXVECTOR2 spritePosition = { floor(nx), floor(ny) };

	D3DXMatrixTranslation(&matTranslation, spritePosition.x, spritePosition.y, 0.1f);
	this->_sprite.matWorld = (this->_scaleMatrix * matTranslation);

	CUtil::SpriteHandler->DrawSpritesImmediate(&_sprite, 1, 0, 0);
}

CSprites* CSprites::__instance = NULL;

CSprites* CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprites::Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;
}

LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}

void CSprite::Release() {
	//_sprite = nullptr;
	//_tex = nullptr;
}

void CSprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}