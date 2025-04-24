#include "pch.h"
#include "Keyboard.h"
char Keyboard::keyPressed =0;
void Keyboard::Init() {
	rwK.dwFlags = 0;
	rwK.hwndTarget = 0;
	rwK.usUsage = 0x06;
	rwK.usUsagePage = 0x01;
	RegisterRawInputDevices(&rwK, 1, sizeof(rwK));
}
void Keyboard::Read(RAWINPUT* raw) {
	if (raw->data.keyboard.Flags == RI_KEY_MAKE) {
		keyPressed = (char)raw->data.keyboard.VKey;
		USHORT check = raw->data.keyboard.Message;
	}
	else {
		keyPressed = 0;
	}
	
	return;
}