#pragma once
#ifdef ImGUI_ENABLED
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui.h"
#include <vector>
#include <string>
#endif
static std::string WorkingDirectory = "D:\\program\\Eng";
struct ObjectProperties {
	virtual void show() = 0;
};
class Objects {
public:
	inline static std::vector<short> GlobalIdPool;
	inline static int count = 0;
	Objects(short id,std::string ObjName) :Id(id),ObjName(ObjName) {};
	short Id;
	std::string ObjName;
	virtual std::vector<ObjectProperties*> GetProperties() = 0;
};
struct TransformStruct:public ObjectProperties {
	float rotation[3] = { 0,0,0 };
	float position[3] = { 0,0,0 };
	float Scale[3] = { 1,1,1 };
	void show() override {
		ImGui::DragFloat3("Rotation", rotation, 0.1f);
		ImGui::DragFloat3("Position", position, 0.1f);
		ImGui::DragFloat3("Scale", Scale, 0.1f);
	}
};
class Globals {
public:
	static float* dT;
};
struct Vertex {
	struct {
		float x;
		float y;
		float z;
	}position;
	struct {
		float u;
		float v;
	}texcoord;
	struct {
		float x;
		float y;
		float z;
	}Normal;
	Vertex() = default;
	Vertex(std::vector<float> val) :position{ val[0],val[1],val[2] }, texcoord{ val[3],val[4] }, Normal{val[5],val[6],val[7]} {};
	Vertex(std::initializer_list<float> val){
		auto it = val.begin();
		position = { *(it++), *(it++), *(it++) };
		texcoord = { *(it++), *(it++) };
		Normal = { *(it++), *(it++), *(it++) };
	};
};
struct NormalPerObject {
	float x;
	float y;
	float z;
	NormalPerObject(float x, float y, float z) :x(x), y(y), z(z) {}
};
