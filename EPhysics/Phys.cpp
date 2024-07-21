#include "pch.h"
#include "Phys.h"
#include "ImGui/imgui.h"
float Physics_Body::gravity = -9.81f;
void Physics_Body::inPlayMode() {
	Update();
}
void Physics_Body::show()
{
	if (ImGui::CollapsingHeader("Physics Body")) {
		ImGui::Checkbox("Affected by Gravity", &isAffectedByGravity);
	}
}
Physics_Body::Physics_Body(TransformStruct*& t,float*& dt):t(t),dt(dt) {

}
void Physics_Body::Update() {
	float test = *dt;
	velocity[0] = t->position[0] - lastPos[0];
	velocity[1] = t->position[1] - lastPos[1];
	velocity[2] = t->position[2] - lastPos[2];
	lastPos[0] = t->position[0];
	lastPos[1] = t->position[1];
	lastPos[2] = t->position[2];
	if (gravity == 0) {
		return;
	}
	if (isAffectedByGravity) {
		g_velocity += gravity * (*dt);
		t->position[1] += (g_velocity * (*dt)) / 2;
	}
}
void Physics_Body::ResetGravity() {
	gravity = 0;
}
Physics_Body* CreatePhysicsBody(TransformStruct* t)
{
	return new Physics_Body(t);
}
