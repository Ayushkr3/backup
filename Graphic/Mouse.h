#pragma once

class Mouse {
public:
	static bool mouse_button[2];
	static int mouse_xpos;
	static int mouse_ypos;
	static float ncd_coordinate_x;
	static float ncd_coordinate_y;
	static void ncd_calc();    //normal device coordinate = ncd
	
};