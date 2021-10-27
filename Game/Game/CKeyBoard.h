#pragma once

#include "CUtil.h"

#define KEYBOARD_BUFFER_SIZE  1024

class CKeyBoardCustom
{
private:
	static CKeyBoardCustom* __instance;
	static BYTE _keyStates[256];

	DIDEVICEOBJECTDATA _keyEvents[KEYBOARD_BUFFER_SIZE];

	LPDIRECTINPUT8 _directInput;
	LPDIRECTINPUTDEVICE8 _keyboard;

	CKeyBoardCustom();
	~CKeyBoardCustom();
public:
	static CKeyBoardCustom* GetInstance();
	static bool IsKeyDown(int);

	bool InitKeyboard(HWND);
	void ProcessKeyboardInputs();
	void Release();
};

