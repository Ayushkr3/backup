#pragma once
#include <Windows.h>
#include "MACROS.h"
class Keyboard{
private:
	UINT cbSize = 16;
public:
	static INPUT_API char keyPressed;
	PRAWINPUT data;
	RAWINPUTDEVICE rwK;
	INPUT_API void Init();
	INPUT_API void Read(RAWINPUT* raw);
};

