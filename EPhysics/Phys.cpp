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
		ImGui::Text(("X velocity "+std::to_string(velocity[0])).c_str());
		ImGui::Text(("Y velocity " + std::to_string(velocity[1])).c_str());
		ImGui::Text(("Z velocity " + std::to_string(velocity[2])).c_str());
	}
}
Physics_Body::Physics_Body(TransformStruct*& t,float*& dt):t(t),dt(dt) {
	lastdT = new float;
	DirectX::XMFLOAT3 gravityflo = DirectX::XMFLOAT3(0,gravity,0);
	gravityvec = DirectX::XMVECTOR(DirectX::XMLoadFloat3(&gravityflo));
}
void Physics_Body::Update() {
	last_velocity = {velocity[0],velocity[1],velocity[2]};
	DirectX::XMFLOAT3 res = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMVECTOR resultant = DirectX::XMVECTOR(DirectX::XMLoadFloat3(&res));
	unsigned int i = 0;
	while(i<pendingForces.size()){
		DirectX::XMVECTOR temp = DirectX::XMVECTOR(DirectX::XMLoadFloat3(&(*pendingForces.cbegin())));
		resultant = DirectX::XMVectorAdd(temp,resultant);
		pendingForces.pop_back();
	}
	if (isAffectedByGravity) {
		resultant = DirectX::XMVectorAdd(gravityvec, resultant);
	}
	DirectX::XMStoreFloat3(&res, resultant);
	//if (t->isChangedexternally) {
	//	ResetVelocity();
	//}
	(velocity[1] > 0) ? t->position[1] -= (velocity[1] * (*dt)) - ((res.y * (*dt)*(*dt)) / 2) : t->position[1] += (velocity[1] * (*dt)) + ((res.y * (*dt)*(*dt)) / 2);
	(velocity[0] > 0) ? t->position[0] -= (velocity[0] * (*dt)) - ((res.x * (*dt)*(*dt)) / 2) : t->position[0] += (velocity[0] * (*dt)) + ((res.x * (*dt)*(*dt)) / 2);
	(velocity[2] > 0) ? t->position[2] -= (velocity[2] * (*dt)) - ((res.z * (*dt)*(*dt)) / 2) : t->position[2] += (velocity[2] * (*dt)) + ((res.z * (*dt)*(*dt)) / 2);
	
	velocity[0] = (t->position[0] - t->l_position[0])/(*lastdT);//instanteous velocity
	velocity[1] = (t->position[1] - t->l_position[1])/(*lastdT);//instanteous velocity
	velocity[2] = (t->position[2] - t->l_position[2])/(*lastdT);//instanteous velocity
	acceleration = {velocity[0]-last_velocity[0]/(*lastdT),velocity[1] - last_velocity[1] / (*lastdT),velocity[2] - last_velocity[2] / (*lastdT) };
	*lastdT = *dt;
}
void Physics_Body::ResetGravity() {
	gravity = 0;
}
void Physics_Body::AddForce(float x,float y,float z) {
	DirectX::XMFLOAT3 force = DirectX::XMFLOAT3(x, y, z);
	pendingForces.push_back(force);
}
void Physics_Body::ResetVelocity() {
	t->l_position[0] = t->position[0];
	t->l_position[1] = t->position[1];
	t->l_position[2] = t->position[2];
	velocity[0] =0;
	velocity[1] =0;
	velocity[2] =0;
	*lastdT =1;
}
ObjectProperties* Physics_Body::GetPropertyRef() {
	return this;
}
Physics_Body* CreatePhysicsBody(TransformStruct* t)
{
	return new Physics_Body(t);
}
