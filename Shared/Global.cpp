#include "pch.h"
#include "Global.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui.h"
bool Globals::inPlayMode = false;
float* Globals::dT = 0;
ImGuiContext* ObjectProperties::ctx = nullptr;
ObjectProperties::ObjectProperties(Objects* obj) :associatedObj(obj) {

}
TransformStruct::TransformStruct(Objects* obj) : ObjectProperties(obj) {

}
static TransformStruct* t = new TransformStruct(nullptr);
Objects::Objects(short id, std::string ObjName) :Id(id), ObjName(ObjName) {
	float i = 0;
	Inheritence.InheritedTrans = t;
	(Inheritence.InheritedTrans)->position[0] = 0;
	(Inheritence.InheritedTrans)->position[1] = 0;
	(Inheritence.InheritedTrans)->position[2] = 0;
};
void TransformStruct::show() {
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_CollapsingHeader)) {
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
ImGuiContext* GetImCtx() {
	return ImGui::GetCurrentContext();
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
void Objects::SetInheritence(Objects*& o) {
	for (auto& op : *o->GetProperties()) {
		if (dynamic_cast<TransformStruct*>(op) != nullptr) {
			TransformStruct* b = (dynamic_cast<TransformStruct*>(op));
			this->Inheritence.InheritedTrans = b;
		}
	}
	this->Inheritence.inheritedFrom = o;

}