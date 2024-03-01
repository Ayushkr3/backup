#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinUser.h>
#include <array>
class Mouse {
private :

	RAWINPUTDEVICE raw_dev;
public:
	int XY = 0 ;
	void RegisterForRaw();
	int GetRawData(RAWINPUT *raw,LPARAM *lparam);
	void DestroyRawInput();
	static bool mouse_button[2];
	static int mouse_xpos;
	static int mouse_ypos;
	static int deltaMouseX;
	static int deltaMouseY;
	static float ncd_coordinate_x;
	static float ncd_coordinate_y;
	static float  GestureX;
	static float GestureY;
	
};