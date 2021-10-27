#include "CBackGround.h"
#include "CGame.h"
#include "CCam.h"
#include "CUtil.h"

#define LENGTH_SPRITES 4
#define WIDTH_TITLE_SET 16
#define HEIGHT_TITLE_SET 16

CBackGround::CBackGround() {
	wstring path = ToWSTR(PATH_IMG_TITLE_SET);
	this->_tex = CTextures::GetInstance();
	_tex->Add(ID_IMG_TITLE_SET, path.c_str());
	_sprite.pTexture = _tex->Get(ID_IMG_TITLE_SET)->getShaderResourceView();
}

void CBackGround::_ParseSectionSprites(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_SPRITES) return; // skip invalid lines

	float x = stof(tokens[0].c_str());
	float y = stof(tokens[1].c_str());
	int l = atoi(tokens[2].c_str());
	int t = atoi(tokens[3].c_str());

	RECT spriteBound;
	spriteBound.left = l;
	spriteBound.top = t;
	spriteBound.right = l + WIDTH_TITLE_SET;
	spriteBound.bottom = t + HEIGHT_TITLE_SET;

	D3DXVECTOR2 position = D3DXVECTOR2(x, y);

	this->_sprites.emplace_back(std::make_pair(spriteBound, position));
}

void CBackGround::_ScaleSprite(const RECT& spriteBound) {
	_sprite.TexCoord.x = spriteBound.left / static_cast<float>(_tex->Get(ID_IMG_TITLE_SET)->getWidth());
	_sprite.TexCoord.y = spriteBound.top / static_cast<float>(_tex->Get(ID_IMG_TITLE_SET)->getHeight());

	int spriteWidth = spriteBound.right - spriteBound.left;
	int spriteHeight = spriteBound.bottom - spriteBound.top;
	_sprite.TexSize.x = spriteWidth / static_cast<float>(_tex->Get(ID_IMG_TITLE_SET)->getWidth());
	_sprite.TexSize.y = spriteHeight / static_cast<float>(_tex->Get(ID_IMG_TITLE_SET)->getHeight());
	_sprite.ColorModulate = { 1.0f, 1.0f, 1.0f, 1.0f };
	_sprite.TextureIndex = 0;

	D3DXMatrixScaling(&_scaleMatrix, static_cast<float>(spriteWidth), static_cast<float>(spriteHeight), 1.0f);
}

void CBackGround::Load(string filePath) {
	ifstream f;
	f.open(filePath);
	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue; //comment line
		_ParseSectionSprites(line);
	}
}

void CBackGround::Render() {
	for (const auto& sprite : _sprites) {
		float x = sprite.second.x - CCam::GetInstance()->GetPosition().x;
		float y = (CGame::GetInstance()->GetBackBufferHeight() - sprite.second.y) + CCam::GetInstance()->GetPosition().y;
		D3DXVECTOR2 spritePosition = { floor(x), floor(y) };

		_ScaleSprite(sprite.first);

		D3DXMATRIX translationMatrix;
		D3DXMatrixTranslation(&translationMatrix, spritePosition.x, spritePosition.y, 0.1f);
		_sprite.matWorld = _scaleMatrix * translationMatrix;

		CUtil::SpriteHandler->DrawSpritesImmediate(&_sprite, 1, 0, 0);
	}
}

void CBackGround::Release() {
	_sprites.clear();
	_sprite.pTexture = nullptr;
	_tex = nullptr;
}

CBackGround::~CBackGround() {}