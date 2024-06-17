#include "Mouse.h"
#include <unordered_map>
#include <iostream>

float Mouse::GestureX = 0;
float Mouse::GestureY = 0;
bool Mouse::mouse_button[2] = {0,0};
int Mouse::mouse_xpos =0;
int Mouse::mouse_ypos =0;
float Mouse::ncd_coordinate_x = 0;
float Mouse::ncd_coordinate_y = 0;
int Mouse::deltaMouseX = 0;
int Mouse::deltaMouseY = 0;
void Mouse::RegisterForRaw() {
	raw_dev.usUsage = 0x02;
	raw_dev.dwFlags = 0;
	raw_dev.usUsagePage = 0x01;
	RegisterRawInputDevices(&raw_dev, 1, sizeof(raw_dev));
}
int Mouse::GetRawData(RAWINPUT* raw,LPARAM *lparam) {
	
	UINT size = sizeof(RAWINPUT);
	GetRawInputData((HRAWINPUT)(*lparam), RID_INPUT, raw, &size, sizeof(RAWINPUTHEADER));

	if (raw->data.mouse.lLastX == 0 && raw->data.mouse.lLastY == 0) {
	
	}
	if (raw->header.dwType == RIM_TYPEMOUSE) {
		RAWMOUSE& raw_mouse = raw->data.mouse;
		if (raw_mouse.usFlags == MOUSE_MOVE_RELATIVE) {
			XY = (int)raw_mouse.lLastX;
			//mouseXY[1] = (int)raw_mouse.lLastY;
		}
	}
	return XY;
}
void Mouse::DestroyRawInput() {
}
