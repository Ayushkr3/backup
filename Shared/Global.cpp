#include "Global.h"
#include "ImGui/imgui_impl_dx11.h"
bool Globals::inPlayMode = false;
float* Globals::dT = 0;
void TransformStruct::show() {
	if (ImGui::CollapsingHeader("Transform",ImGuiTreeNodeFlags_DefaultOpen|ImGuiTreeNodeFlags_CollapsingHeader)) {
		ImGui::DragFloat3("Rotation", rotation, 0.1f);
		if (ImGui::DragFloat3("Position", position, 0.1f)) {
			isChangedexternally = true;
		}
		else {
			isChangedexternally = false;
		}
		ImGui::DragFloat3("Scale", Scale, 0.1f);
	}
}

ObjectProperties* TransformStruct::GetPropertyRef() {
	return this;
}
void TransformStruct::Update() {
	if (l_position[0] != position[0] || l_position[1] != position[1] || l_position[2] != position[2]) {
		l_position[0] = position[0];
		l_position[1] = position[1];
		l_position[2] = position[2];
		isMoving = true;
		return;
	}
	isMoving = false;
}