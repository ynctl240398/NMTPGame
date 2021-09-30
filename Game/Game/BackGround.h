#pragma once
#include "Sprite.h"
#include "Sprites.h"
#include "Textures.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class CPos
{
protected:
	float x;
	float y;
	int id;
public:
	CPos(int id, float x, float y) {
		this->id = id;
		this->x = x;
		this->y = y;
	}

	void setId(int p) { this->id = p; }
	void setX(int p) { this->x = p; }
	void setY(int p) { this->y = p; }
	int getId() { return this->id; }
	int getX() { return this->x; }
	int getY() { return this->y; }

	~CPos() {
		this->x = 0;
		this->y = 0;
		this->id = 0;
	}
};

typedef CPos* LPPOS;

class CTitle 
{
protected:
	int l;
	int t;
	int r;
	int b;
public:
	CTitle(int l, int t, int r, int b) {
		this->l = l;
		this->t = t;
		this->r = r;
		this->b = b;
	}

	void setL(int p) { this->l = p; }
	void setT(int p) { this->t = p; }
	void setR(int p) { this->r = p; }
	void setB(int p) { this->b = p; }
	int getL() { return this->l; }
	int getT() { return this->t; }
	int getR() { return this->r; }
	int getB() { return this->b; }

	~CTitle() {
		this->l = 0;
		this->t = 0;
		this->r = 0;
		this->b = 0;
	}
};

typedef CTitle* LPTILTE;

class CBackGround
{
protected:
	string filePathTxt;
	string filePathImg;
	unordered_map<int, LPTILTE> titles;
	unordered_map<int, LPPOS> poss;
	unordered_map<int, LPSPRITE> sprites;
	CTextures* textures;
	int countSprite;
	void initTitles();
	void _ParseSection_Sprites(string line, LPTEXTURE tex);
	vector<string> split(string line, string delimeter);
	wstring ToWSTR(string st);
public:
	CBackGround(string filePathTxt, string filePathImg);
	void Load();
	void Render();
	void Update();
};