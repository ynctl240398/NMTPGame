//#pragma once
//#include "CGameObject.h"
//#include "CTexture.h"
//
//class CGrid;
//struct SCell;
//
//class CEntity : public CGameObject
//{
//protected:
//	enum class _DataFileSection {
//		DATAFILE_SECTION_UNKNOWN,
//		DATAFILE_SECTION_SPRITES,
//		DATAFILE_SECTION_HITBOXES,
//		DATAFILE_SECTION_SETTINGS
//	};
//
//	vector<string> _extraData;
//
//	float _runSpeed;
//	float _jumpSpeed;
//	float _bounceSpeed;
//	float _gravity;
//	float _acceleration;
//
//	bool _isOnGround;
//	bool _isHeld;
//
//	//-1: set to inactive
//	int _health;
//
//	//The lower the value, the higher the render priority;
//	unsigned int _renderPriority;
//
//	std::string _variant;
//
//	//AnimatedSprite _animatedSprite;
//
//	DWORD _removeStart;
//	//Default is 2s
//	DWORD _removeTime;
//
//	virtual void _ParseSprites(std::string) = 0;
//	virtual void _ParseHitboxes(std::string);
//
//public:
//	bool tookDamage;
//	bool isBeingHeld;
//	SCell* ownerCell;
//
//	static bool CompareRenderPriority(CEntity*&, CEntity*&);
//
//	CEntity();
//	virtual ~CEntity();
//
//
//
//	bool IsRemoved() const;
//	void StartRemoveTimer();
//
//	vector<string> GetExtraData() const;
//
//	void SetGravity(float);
//	float GetGravity() const;
//
//	virtual void SetHealth(int);
//	virtual int GetHealth() const;
//
//	//Entity data  file path, texture, extra data/properties
//	virtual void ParseData(string, LPTEXTURE&, vector<string> = vector<string>());
//
//	virtual void TakeDamage();
//
//	virtual void HandleStates() = 0;
//	//Event results, min time, offset, normal, relative distance
//	virtual void HandleCollisionResult(LPCOLLISIONEVENT, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&, D3DXVECTOR2&) = 0;
//	//Delta time, collidable entities, collidable tiles, grid
//	virtual void Update(DWORD, vector<CEntity*>* = nullptr, vector<CEntity*>* = nullptr, CGrid* = nullptr);
//
//	//An extension of the SweptAABB method
//	LPCOLLISIONEVENT SweptAABBEx(CEntity*&);
//	//Calculate potential collision between entities
//	void CalcPotentialCollision(vector<CEntity*>*, vector<LPCOLLISIONEVENT>&);
//};
//
