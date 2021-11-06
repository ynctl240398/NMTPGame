#pragma once
#include "CGameObject.h"

class CCam : public CGameObject
{
private:
	static CCam* _cameraInstance;

	unsigned int _cameraWidth, _cameraHeight;
	vector<float> _upVectors;
	vector<CRect*> _cameraBounds;

	CCam();
	~CCam();

public:
	static CCam* GetInstance();

	void Load(LPCWSTR);

	void SetCameraWidth(unsigned int);
	unsigned int GetCameraWidth() const;
	void SetCameraHeight(unsigned int);
	unsigned int GetCameraHeight() const;

	float GetUpVector(int = 0) const;
	void AddUpVector(float);

	LPRECTCUSTOM GetViewport() const;
	LPRECTCUSTOM GetCameraBound(int = 0) const;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	void Render() override;

	void AddCameraBound(LPRECTCUSTOM);

	void Update(DWORD, vector<CGameObject*>* = nullptr) override;

	void Release() override;
};

typedef CCam* LPCAM;