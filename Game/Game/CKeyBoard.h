#pragma once

#include "CUtil.h"

#define KEYBOARD_BUFFER_SIZE  1024
#define KEYBOARD_STATE_SIZE 256

class CKeyBoardCustom
{
private:
	static CKeyBoardCustom* __instance;
	static BYTE _keyStates[KEYBOARD_STATE_SIZE];
	static BYTE _keyPressed[KEYBOARD_STATE_SIZE];
	static BYTE _keyRelease[KEYBOARD_STATE_SIZE];

	DIDEVICEOBJECTDATA _keyEvents[KEYBOARD_BUFFER_SIZE];

	LPDIRECTINPUT8 _directInput;
	LPDIRECTINPUTDEVICE8 _keyboard;

	CKeyBoardCustom();
	~CKeyBoardCustom();
public:
	static CKeyBoardCustom* GetInstance();

	//True if key is pressing
	static bool IsKeyDown(int);

	//True if key is pressed in current frame
	static bool IsKeyPressed(int);

	//True if key is release in current frame
	static bool IsKeyReleased(int);

	bool InitKeyboard(HWND);
	void ProcessKeyboardInputs();
	void Release();
};

