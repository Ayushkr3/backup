#include "pch.h"
#include "Mouse.h"

//short Mouse::deltaX;
//short Mouse::deltaY;
//bool Mouse::LeftButtonDown;
//bool Mouse::LeftButtonUp;
//bool Mouse::isLeftButtonHeld;
//bool Mouse::RightButtonDown;
//bool Mouse::RightButtonUp;
//bool Mouse::isRightButtonHeld;
//bool Mouse::isMouseWheelScrolling;
//short Mouse::WheelDelta;
void Mouse::Init() {
	rwM.dwFlags = 0;
	rwM.hwndTarget = 0;
	rwM.usUsage = 0x02;
	rwM.usUsagePage = 0x01;
	RegisterRawInputDevices(&rwM, 1, sizeof(rwM));
}
void Mouse::Read(RAWINPUT* raw) {
	WheelDelta = raw->data.mouse.usButtonData;
	//fast using & instead of ==
	if (raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE) {
		if (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0) {
			deltaX = raw->data.mouse.lLastX;
			deltaY = raw->data.mouse.lLastY;
		}
	}
	//left click
	if (raw->data.mouse.usButtonFlags&RI_MOUSE_BUTTON_1_DOWN) {
		isLeftButtonHeld = true;
		LeftButtonDown = true;
	}
	else LeftButtonDown = false;
	if (raw->data.mouse.usButtonFlags&RI_MOUSE_BUTTON_1_UP) {
		isLeftButtonHeld = false;
		LeftButtonUp = true;
	}
	else LeftButtonUp = false;
	//right click
	if (raw->data.mouse.usButtonFlags&RI_MOUSE_BUTTON_2_DOWN) {
		isRightButtonHeld = true;
		RightButtonDown = true;
	}
	else RightButtonDown = false;
	if (raw->data.mouse.usButtonFlags&RI_MOUSE_BUTTON_2_UP) {
		isRightButtonHeld = false;
		RightButtonUp = true;
	}
	else RightButtonUp = false;
	if (raw->data.mouse.usButtonFlags& RI_MOUSE_WHEEL) {
		if (WheelDelta != 0)
			isMouseWheelScrolling = true;
	}
}
void Mouse::ResetDelta() {
	deltaX = 0;
	deltaY = 0;
	WheelDelta = 0;
	isMouseWheelScrolling = false;
}