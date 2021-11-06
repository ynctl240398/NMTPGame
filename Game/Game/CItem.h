#pragma once

#include "CGameObject.h"
#include "CTexture.h"
#include "CAnimation.h"

#define ITEM_WIDTH 16
#define ITEM_HEIGHT 16

//syntax
#define TYPE_MUSHROOM_RED "mushroom_red"
#define TYPE_MUSHROOM_GREEN "mushroom_green"

#define ID_ITEM_MUSHROOM_RED_ANI 3000
#define ID_ITEM_MUSHROOM_GREEN_ANI 3001

class CItem : public CGameObject
{
	CTextures* _tex;
	CAnimation* _ani;

	string _type;

	void _ParseSectionSprites(string);
	void _ParseSectionFrames(string);
	void _ParseSectionAnimation(string);
public:

	CItem(float x, float y, string type);

	void Load(LPCWSTR filePath);
	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Release() override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;

	string GetType() { return this->_type; }

	~CItem();
};

