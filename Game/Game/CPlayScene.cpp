#include "CPlayScene.h"
#include "CUtil.h"
#include "CDebug.h"
#include "CSprite.h"
#include "CAnimation.h"
#include "CPlatform.h"
#include "CMario.h"
#include "DefineScenePlay.h"
#include "CPortal.h"
#include "CBrick.h"
#include "CBrickQuestion.h"
#include "CItem.h"
#include "CGoomba.h"
#include "CParaGoomba.h"
#include "CKoopaParaTropa.h"

using namespace std;

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2
#define ASSETS_SECTION_FRAME 3

#define MAX_SCENE_LINE 1024

#define LENGTH_SPRITES 5
#define LENGTH_ANI 2
#define LENGTH_ANIS 1

#define DEFAULT_TIME 100

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	_ani = new CAnimation(DEFAULT_TIME);
	_currentIdTex = -1;
	player = NULL;
}

void CPlayScene::_ParseSectionSprites(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_SPRITES) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = l + atoi(tokens[3].c_str());
	int b = t + atoi(tokens[4].c_str());

	CSprites::GetInstance()->Add(id, l, t, r, b, CTextures::GetInstance()->Get(_currentIdTex));
}

void CPlayScene::_ParseSectionFrames(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_ANI) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	int time = atoi(tokens[1].c_str());

	_ani->Add(id, time);
}

void CPlayScene::_ParseSectionAnimation(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < LENGTH_ANIS) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());

	CAnimations::GetInstance()->Add(id, _ani);

	_ani = new CAnimation(DEFAULT_TIME);
}
/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	wstring path;

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			//DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = CMario::GetInstance();
		player = (CMario*)obj;
		player->SetPosition({ x, y });
		//DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: 
		obj = new CGoomba(x, y); 
		break;
	case OBJECT_TYPE_PARA_GOOMBA:
		obj = new CParaGoomba(x, y);
		break;
	case OBJECT_TYPE_KOOPAS_PARA_TROPA:
	{
		int state = atoi(tokens[3].c_str());
		obj = new CKoopaParaTropa(x, y, state);
		break;
	}
	case OBJECT_TYPE_BRICK: 
	{
		int w = atoi(tokens[3].c_str());
		int h = atoi(tokens[4].c_str());
		obj = new CBrick(x, y, w, h);
		break; 
	}
	case OBJECT_TYPE_ITEM:
	{
		string itemType = tokens[3];
		obj = new CItem(x, y, itemType);
		break;
	}
	case OBJECT_TYPE_BIG_BRICK:
	{
		int w = atoi(tokens[3].c_str());
		int h = atoi(tokens[4].c_str());
		obj = new CBrick(x, y, w, h, true);
		break;
	}
	case OBJECT_TYPE_BRICK_QUESTION:
	{
		obj = new CBrickQuestion(x, y);
		break;
	}
	case OBJECT_TYPE_COIN: 
		//obj = new CCoin(x, y);
		break;

	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;

	case OBJECT_TYPE_BG:
		path = ToWSTR(tokens[3]);
		_bg = new CBackGround();
		_bg->Load(path.c_str());
		break;

	case OBJECT_TYPE_CAM:
		path = ToWSTR(tokens[3]);
		CCam::GetInstance()->Load(path.c_str());
		_cam = CCam::GetInstance();
		break;

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	if (object_type != OBJECT_TYPE_BG && object_type != OBJECT_TYPE_CAM) 
	{
		objects.push_back(obj);
	}
}

void CPlayScene::_ParseSection_ASSETS(string line) {
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	int id = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);

	_currentIdTex = id;

	_LoadAssets(path.c_str());
}

void CPlayScene::_LoadAssets(LPCWSTR assetFile) {
	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#' || line.empty()) continue; //comment line

		if (line == "[SPRITE]") {
			section = ASSETS_SECTION_SPRITES;
			continue;
		}
		if (line == "[FRAME]") {
			section = ASSETS_SECTION_FRAME;
			continue;
		}
		if (line == "[ANIMATION]") {
			section = ASSETS_SECTION_ANIMATIONS;
			continue;
		}

		switch (section)
		{
		case ASSETS_SECTION_SPRITES:
			_ParseSectionSprites(line);
			break;
		case ASSETS_SECTION_FRAME:
			_ParseSectionFrames(line);
			break;
		case ASSETS_SECTION_ANIMATIONS:
			_ParseSectionAnimation(line);
			break;
		default:
			break;
		}
	}
	f.close();
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#' || line.empty()) continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	if (player->GetState() == STATE_MARIO_DIE) {
		player->Update(dt);
		return;
	}
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (!dynamic_cast<CMario*>(objects[i])) {
			coObjects.push_back(objects[i]);
		}
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	_bg->Render();
	_cam->Render();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	//DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}

void CPlayScene::HandleKeyDown(int keyCode) {
	LPGAME g = CGame::GetInstance();
	switch (keyCode)
	{
	case DIK_Q:
		this->Clear();
		_ani = new CAnimation(DEFAULT_TIME);
		_currentIdTex = -1;
		player = NULL;
		this->Load();
		break;
	default:
		break;
	}
}