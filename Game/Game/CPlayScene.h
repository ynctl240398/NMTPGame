#pragma once
#include "CTexture.h"
#include "CScene.h"
#include "CGameObject.h"

#include "CBackGround.h"
#include "CCam.h"
#include "CAnimation.h"

class CPlayScene : public CScene
{
protected:
	int _currentIdTex;
	// A play scene has to have player, right?  -> yes :))
	LPGAMEOBJECT player;

	vector<LPGAMEOBJECT> objects;

	CBackGround* _bg;
	CCam* _cam;
	CAnimation* _ani;

	void _ParseSectionSprites(string);
	void _ParseSectionFrames(string);
	void _ParseSectionAnimation(string);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void _LoadAssets(LPCWSTR assetFile);

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void HandleKeyDown(int keyCode) override;

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;