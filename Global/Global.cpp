#include "pch.h"
#include "Global.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui.h"

bool* Globals::inPlayMode = nullptr;
float* Globals::dT = 0;
bool Globals::isFullscreen = false;
std::multimap<std::string,std::function<ObjectProperties*(Objects*)>> ObjectProperties::GlobalPropertiesPool = {};
ObjectProperties::ObjectProperties(Objects* obj) :associatedObj(obj) {

}
TransformStruct::TransformStruct(Objects* obj) : ObjectProperties(obj) {
}
static TransformStruct* t = new TransformStruct(nullptr);
void ObjectProperties::PushToObjectPropertyPool(std::string name, std::function<ObjectProperties*(Objects*)> f, std::multimap<std::string, std::function<ObjectProperties*(Objects*)>>& GlobalPropertiesPoolL = ObjectProperties::GlobalPropertiesPool) {
	GlobalPropertiesPoolL.insert({name,f});
}
Objects::Objects(short id, std::string ObjName) :Id(id), ObjName(ObjName) {
	float i = 0;
	Inheritence.InheritedTrans = t;
	(Inheritence.InheritedTrans)->position[0] = 0;
	(Inheritence.InheritedTrans)->position[1] = 0;
	(Inheritence.InheritedTrans)->position[2] = 0;
};
DirectX::XMFLOAT4 TransformStruct::EulerToQuat(float yaw,float pitch,float roll) {
	yaw = DirectX::XMConvertToRadians(yaw);
	pitch = DirectX::XMConvertToRadians(pitch);
	roll = DirectX::XMConvertToRadians(roll);
	float cy = std::cos(yaw * 0.5f);
	float sy = std::sin(yaw * 0.5f);
	float cp = std::cos(pitch * 0.5f);
	float sp = std::sin(pitch * 0.5f);
	float cr = std::cos(roll * 0.5f);
	float sr = std::sin(roll * 0.5f);
	float x = sr * cp * cy - cr * sp * sy;
	float y = cr * sp * cy + sr * cp * sy;
	float z = cr * cp * sy - sr * sp * cy;
	float w = cr * cp * cy + sr * sp * sy;
	return DirectX::XMFLOAT4(x,y,z,w);
}
double normalizeAngle(double angle) {
	while (angle > 180.0) angle -= 360.0;
	while (angle < -180.0) angle += 360.0;
	return angle;
}
void TransformStruct::QuatToEuler(float x,float y ,float z, float w,float eulerAngles[3]) {

	double sinr_cosp = 2 * (w * x + y * z);
	double cosr_cosp = 1 - 2 * (x * x + y * y);
	eulerAngles[0] = std::atan2(sinr_cosp, cosr_cosp) * 180.0 / DirectX::XM_PI; // Convert to degrees

	double sinp = 2 * (w * y - z * x);
	if (std::abs(sinp) >= 1)
		eulerAngles[1] = std::copysign(90.0, sinp); // Gimbal lock, set to ±90 degrees
	else
		eulerAngles[1] = std::asin(sinp) * 180.0 / DirectX::XM_PI; // Convert to degrees

	double siny_cosp = 2 * (w * z + x * y);
	double cosy_cosp = 1 - 2 * (y * y + z * z);
	eulerAngles[2] = std::atan2(siny_cosp, cosy_cosp) * 180.0 / DirectX::XM_PI; // Convert to degrees

	eulerAngles[0] = normalizeAngle(eulerAngles[0]); // Roll
	eulerAngles[1] = normalizeAngle(eulerAngles[1]); // Pitch
	eulerAngles[2] = normalizeAngle(eulerAngles[2]); // Yaw
}
void TransformStruct::show() {
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_CollapsingHeader)) {
		ImGui::Button("Transform");
		if (ImGui::BeginDragDropSource()) {
			std::unique_ptr<RefrencePassing> ref = std::make_unique<RefrencePassing>((void*)this, typeid(*this));
			//RefrencePassing* ref = new RefrencePassing(this, typeid(*this));
			ImGui::SetDragDropPayload("Transform", ref.get(), sizeof(RefrencePassing));
			ImGui::EndDragDropSource();
		}
		
		if (ImGui::DragFloat3("Rotation",EulerRot, 0.1f))
		{
			isChangedExternally = true;
			auto vec = EulerToQuat(EulerRot[0], EulerRot[1], EulerRot[2]);
			rotation[0] = vec.x;
			rotation[1] = vec.y;
			rotation[2] = vec.z;
			rotation[3] = vec.w;
		}
		else {
			QuatToEuler(rotation[0], rotation[1], rotation[2], rotation[3], EulerRot);
			isChangedExternally = false;
		}
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
const std::type_info& TransformStruct::GetPropertyType() {
	return typeid(TransformStruct);
}
