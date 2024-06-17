#include "Physics.h"

float Physics_Body::gravity = -9.81f;
Physics_Body::Physics_Body(){

}
void Physics_Body::Update(float* x, float* y, float* z) {
	velocity[0] = *x - lastPos[0];
	velocity[1] = *y - lastPos[1];
	velocity[2] = *z - lastPos[2];
	lastPos[0] = *x;
	lastPos[1] = *y;
	lastPos[2] = *z;
	if (gravity == 0.0f) {
		return;
	}
	if (isAffectedByGravity) {
		g_velocity += gravity * (*Globals::dT);
		*y += (g_velocity * (*Globals::dT)) / 2;
	}
}
bool Physics_Body::isMoving() {
	if (velocity[0] == 0 &&
		velocity[1] == 0 &&
		velocity[2] == 0) {
		return false;
	}
	return true;
}