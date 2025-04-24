#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "MACROS.h"
class InputManager {
	static PRAWINPUT data;
	static UINT cbSize;
public:
	INPUT_API static Mouse Ms;
	INPUT_API static Keyboard kb;
	INPUT_API static void Read(LPARAM lparam);
	INPUT_API static char GetLastKeyPress();
	INPUT_API static void Init();
};