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

void CSprite::_ScaleSprite(const RECT& spriteBound, int idTex, D3DXVECTOR2 scale, unsigned int alpha) {

	LPTEXTURE texture = CTextures::GetInstance()->Get(idTex);
	if (texture == nullptr) {
		return;
	}

	_sprite.TexCoord.x = spriteBound.left / static_cast<float>(texture->getWidth());
	_sprite.TexCoord.y = spriteBound.top / static_cast<float>(texture->getHeight());

	int spriteWidth = spriteBound.right - spriteBound.left;
	int spriteHeight = spriteBound.bottom - spriteBound.top;
	_sprite.TexSize.x = spriteWidth / static_cast<float>(texture->getWidth());
	_sprite.TexSize.y = spriteHeight / static_cast<float>(texture->getHeight());
	_sprite.ColorModulate = { 1.0f, 1.0f, 1.0f, alpha / 255.0f };
	_sprite.TextureIndex = 0;

	D3DXMatrixScaling(&_scaleMatrix, static_cast<float>(spriteWidth) * scale.x, static_cast<float>(spriteHeight) * scale.y, 1.0f);
}

void CSprite::Draw(RECT rect,float x, float y, int idTex, D3DXVECTOR2 scale, unsigned int alpha)
{
	CGame* g = CGame::GetInstance();

	_ScaleSprite(rect, idTex, scale, alpha);

	D3DXMATRIX matTranslation;

	float nx = x - _cameraInstance->GetPosition().x;
	float ny = (g->GetBackBufferHeight() - y) + _cameraInstance->GetPosition().y;
	D3DXMatrixTranslation(&matTranslation, nx, ny, 0.1f);
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