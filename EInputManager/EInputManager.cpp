#include "pch.h"
#include "EInputManager.h"
#include <stdlib.h>
#include <thread>
Keyboard InputManager::kb;
PRAWINPUT InputManager::data;
UINT InputManager::cbSize;
void InputManager::Init() {
	kb.Init();
}
void InputManager::Start(LPARAM lparam) {
	GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &cbSize, sizeof(RAWINPUTHEADER));
	LPBYTE lpbyte = new BYTE[cbSize];

	GetRawInputData((HRAWINPUT)lparam,RID_INPUT,lpbyte,&cbSize, sizeof(RAWINPUTHEADER));

	RAWINPUT* raw = (RAWINPUT*)lpbyte;
	if (raw->header.dwType == RIM_TYPEKEYBOARD) {
		kb.Read(raw);
	}
}

char InputManager::GetLastKeyPress()
{
	return kb.keyPressed;
}
