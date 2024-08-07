#pragma once
#include "Keyboard.h"
#include "MACROS.h"
class InputManager {
	static PRAWINPUT data;
	static UINT cbSize;
public:
	static Keyboard kb;
	INPUT_API static void Start(LPARAM lparam);
	INPUT_API static char GetLastKeyPress();
	INPUT_API static void Init();
};