#include "pch.h"
#include "Global.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui.h"

bool* Globals::inPlayMode = nullptr;
float* Globals::dT = 0;
std::multimap<std::string,std::function<std::unique_ptr<ObjectProperties>(Objects*)>> ObjectProperties::GlobalPropertiesPool = {};
ObjectProperties::ObjectProperties(Objects* obj) :associatedObj(obj) {

}
TransformStruct::TransformStruct(Objects* obj) : ObjectProperties(obj) {

}
static TransformStruct* t = new TransformStruct(nullptr);
void ObjectProperties::PushToObjectPropertyPool(std::string name, std::function<std::unique_ptr<ObjectProperties>(Objects*)> f, std::multimap<std::string, std::function<std::unique_ptr<ObjectProperties>(Objects*)>>& GlobalPropertiesPoolL = ObjectProperties::GlobalPropertiesPool) {
	GlobalPropertiesPoolL.insert({name,f});
}
Objects::Objects(short id, std::string ObjName) :Id(id), ObjName(ObjName) {
	float i = 0;
	Inheritence.InheritedTrans = t;
	(Inheritence.InheritedTrans)->position[0] = 0;
	(Inheritence.InheritedTrans)->position[1] = 0;
	(Inheritence.InheritedTrans)->position[2] = 0;
};
void TransformStruct::show() {
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_CollapsingHeader)) {
		ImGui::Button("Transform");
		if (ImGui::BeginDragDropSource()) {
			std::unique_ptr<RefrencePassing> ref = std::make_unique<RefrencePassing>((void*)this, typeid(*this));
			//RefrencePassing* ref = new RefrencePassing(this, typeid(*this));
			ImGui::SetDragDropPayload("Transform", ref.get(), sizeof(RefrencePassing));
			ImGui::EndDragDropSource();
		}
		ImGui::DragFloat3("Rotation", rotation, 0.1f);
		if(ImGui::DragFloat3("Position", position, 0.1f))
		{
			isChangedExternally = true;
		}
		else {
			isChangedExternally = false;
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
void Objects::SetInheritence(Objects*& o) {
	for (auto& op : *o->GetProperties()) {
		if (dynamic_cast<TransformStruct*>(op) != nullptr) {
			TransformStruct* b = (dynamic_cast<TransformStruct*>(op));
			this->Inheritence.InheritedTrans = b;
		}
	}
	this->Inheritence.inheritedFrom = o;

}
std::string GetMemAddress(void* ptr) {
	std::ostringstream oss; oss << ptr; return oss.str().substr(oss.str().size()-7, 7);
}