#pragma once

#include <Windows.h>

#include "CCam.h"

class CScene
{
protected:
	int id;
	LPCWSTR sceneFilePath;

	CCam* _cam;

public:
	CScene(int id, LPCWSTR filePath)
	{
		this->id = id;
		this->sceneFilePath = filePath;
		_cam = CCam::GetInstance();
	}

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
};
typedef CScene* LPSCENE;
