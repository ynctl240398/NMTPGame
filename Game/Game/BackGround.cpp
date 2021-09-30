#include "BackGround.h"

#define MAX_SCENE_LINE 1024
#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ID 0
#define SCENE_SECTION_POS 1
#define BG_ID_WORLD_1_1 100
#define TOTAL_SPRITES 95
#define TITLE_WITDH 16
#define TITLE_HEIGHT 16
#define TITLE_SPACE 1
#define TITLE_ROW 0

void CBackGround::initTitles()
{
	for (size_t i = 0; i < TOTAL_SPRITES; i++)
	{
		int id = BG_ID_WORLD_1_1 + i;
		int l = i * (TITLE_WITDH + TITLE_SPACE);
		int r = l + TITLE_WITDH;
		int t = TITLE_ROW;
		int b = t + TITLE_HEIGHT;
		this->titles[id] = new CTitle(l, t, r, b);
	}
}

void CBackGround::_ParseSection_Sprites(string line, LPTEXTURE tex)
{
	vector<string> tokens = split(line, "\t");
	if (tokens.size() < 3) return; // skip invalid lines

	int id = atoi(tokens[0].c_str());
	int x = stoi(tokens[1].c_str());
	int y = stoi(tokens[2].c_str());

	LPSPRITE sprite = new CSprite(this->countSprite, this->titles[id]->getL(), this->titles[id]->getT(), this->titles[id]->getR(), this->titles[id]->getB(), tex);
	this->sprites[this->countSprite] = sprite;
	LPPOS pos = new CPos(id, x + TITLE_WITDH / 2, y + TITLE_HEIGHT / 2);
	this->poss[this->countSprite] = pos;

	this->countSprite++;
}

vector<string> CBackGround::split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

wstring CBackGround::ToWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring wstr(wcstring);

	// delete wcstring   // << can I ? 
	return wstr;
}

CBackGround::CBackGround(string filePathTxt, string filePathImg) {
	this->filePathTxt = filePathTxt;
	this->filePathImg = filePathImg;
	this->countSprite = 0;
	this->textures = new CTextures();
	this->initTitles();
}

void CBackGround::Load()
{
	int section = SCENE_SECTION_UNKNOWN;
	ifstream f;
	f.open(this->filePathTxt);
	int id;
	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue; //comment line

		if (line == "[ID]") {
			section = SCENE_SECTION_ID; continue;
		}

		if (line == "[POS]") {
			section = SCENE_SECTION_POS; continue;
		}

		if (section == SCENE_SECTION_ID) {
			id = atoi(line.c_str());
			wstring path = ToWSTR(this->filePathImg);
			textures->Add(id, path.c_str());
			continue;
		}

		if (section == SCENE_SECTION_POS)
		{
			_ParseSection_Sprites(line, textures->Get(id));
		}
	}
}

void CBackGround::Render()
{
	for (size_t i = 0; i < this->countSprite; i++)
	{
		this->sprites[i]->Draw(this->poss[i]->getX(), 100);
	}
}

void CBackGround::Update()
{
}
