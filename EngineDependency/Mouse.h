#pragma once
#include <Windows.h>
#include "MACROS.h"
class Mouse {
public:
	INPUT_API static bool isLeftButtonHeld;
	INPUT_API static bool LeftButtonDown;
	INPUT_API static bool LeftButtonUp;
	INPUT_API static bool isRightButtonHeld;
	INPUT_API static bool RightButtonDown;
	INPUT_API static bool RightButtonUp;
	INPUT_API static short deltaX;
	INPUT_API static short deltaY;
	INPUT_API static bool isMouseWheelScrolling;
	INPUT_API static short WheelDelta;
	RAWINPUTDEVICE rwM;
	INPUT_API void Init();
	INPUT_API void Read(RAWINPUT* raw);
	INPUT_API static void ResetDelta();
};
