#include "CKeyBoard.h"

CKeyBoardCustom* CKeyBoardCustom::__instance = NULL;

BYTE CKeyBoardCustom::_keyStates[256] = { ' ' };

CKeyBoardCustom::CKeyBoardCustom() {
}

CKeyBoardCustom::~CKeyBoardCustom() {}

CKeyBoardCustom* CKeyBoardCustom::GetInstance() {
	if (__instance == NULL) {
		__instance = new CKeyBoardCustom;
	}
	return __instance;
}

bool CKeyBoardCustom::IsKeyDown(int keyCode) {
	return (_keyStates[keyCode] & 0x80) > 0;
}

bool CKeyBoardCustom::InitKeyboard(HWND hWND) {
	HRESULT hResult;

	hResult = DirectInput8Create(
		reinterpret_cast<HINSTANCE>(GetWindowLong(hWND, GWL_HINSTANCE)),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		reinterpret_cast<VOID**>(&_directInput),
		nullptr
	);

	if (hResult != DI_OK) {
		MessageBoxA(hWND, "Failed to create input", "Error", MB_ICONERROR);
		return false;
	}

	hResult = _directInput->CreateDevice(GUID_SysKeyboard, &_keyboard, nullptr);

	if (hResult != DI_OK) {
		MessageBoxA(hWND, "Failed to create device in Device class", "Error", MB_ICONERROR);
		return false;
	}

	_keyboard->SetDataFormat(&c_dfDIKeyboard);
	_keyboard->SetCooperativeLevel(hWND, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE;

	_keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hResult = _keyboard->Acquire();

	if (hResult != DI_OK) {
		MessageBoxA(hWND, "Failed to acquire keyboard", "Error", MB_ICONERROR);
		return false;
	}

	return true;
}

void CKeyBoardCustom::ProcessKeyboardInputs() {
	HRESULT hResult;
	hResult = _keyboard->GetDeviceState(sizeof(_keyStates), _keyStates);

	if (FAILED(hResult)) {
		if (hResult == DIERR_INPUTLOST || hResult == DIERR_NOTACQUIRED) {
			hResult = _keyboard->Acquire();

			if (hResult != DI_OK) {
				return;
			}
		}
		else {
			return;
		}
	}

	//_managerInstance->GetCurrentScene()->HandleStates();

	DWORD elements = KEYBOARD_BUFFER_SIZE;

	hResult = _keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), _keyEvents, &elements, 0);

	if (FAILED(hResult)) {
		return;
	}

	for (DWORD i = 0; i < elements; ++i) {
		int keyCode = _keyEvents[i].dwOfs;
		int keyState = _keyEvents[i].dwData;

		/*if ((keyState & 0x80) > 0) {
			_managerInstance->GetCurrentScene()->OnKeyDown(keyCode);
		}
		else {
			_managerInstance->GetCurrentScene()->OnKeyUp(keyCode);
		}*/
	}
}

void CKeyBoardCustom::Release() {
	if (__instance != NULL) {
		delete __instance;
		__instance = NULL;
	}
}