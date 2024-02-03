#include "Mouse.h"

bool Mouse::mouse_button[2] = {0,0};
int Mouse::mouse_xpos =0;
int Mouse::mouse_ypos =0;
float Mouse::ncd_coordinate_x = 0;
float Mouse::ncd_coordinate_y = 0;
void Mouse::ncd_calc() {
	ncd_coordinate_x = (2.0f * Mouse::mouse_xpos / 800) - 1.0f;
	ncd_coordinate_y = 1.0f - (2.0f*Mouse::mouse_ypos/ 600);
}
