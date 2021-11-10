#include "CTexture.h"
#include <Windows.h>

#include "CDebug.h"
#include "CGame.h"

CTextures* CTextures::__instance = NULL;

CTextures::CTextures()
{
}

CTextures* CTextures::GetInstance()
{
	if (__instance == NULL) __instance = new CTextures();
	return __instance;
}

void CTextures::Add(int id, LPCWSTR filePath)
{
	textures[id] = CGame::GetInstance()->LoadTexture(filePath);
}

void CTextures::Load(LPCWSTR filePath) {
	ifstream f;
	f.open(filePath);

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#' || line.empty()) continue;	// skip comment lines

		vector<string> tokens = split(line);
		int ID = atoi(tokens[0].c_str());
		wstring path = ToWSTR(tokens[1]);

		this->Add(ID, path.c_str());
	}
	f.close();
}

LPTEXTURE CTextures::Get(unsigned int i)
{
	return textures[i];
}