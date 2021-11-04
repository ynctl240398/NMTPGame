#include "CUtil.h"

namespace CUtil {
	ID3D10Device* DirectDevice = NULL;
	ID3DX10Sprite* SpriteHandler = NULL;
	ID3DX10Font* Font = NULL;

	void GetFontRect(LPCSTR text, RECT* rect) {
		//Using DT_CALCRECT causes the rect to be determined
		//But not to be drawn
		Font->DrawTextA(
			SpriteHandler,
			text,
			-1,
			rect,
			DT_CALCRECT,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
		);
	}

	void WriteTextToScreen(LPCSTR text, RECT* rect, UINT format, D3DXCOLOR fontColor) {
		SetRectEmpty(rect);
		GetFontRect(text, rect);
		Font->DrawTextA(SpriteHandler, text, -1, rect, format, fontColor);
	}
}

wstring ToWSTR(string st)
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

vector<string> split(string line, string delimeter)
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

LPCWSTR ToLPCWSTR(string st)
{
	const char* str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring* w = new wstring(wcstring);

	// delete wcstring   // << can I ? 
	return w->c_str();
}