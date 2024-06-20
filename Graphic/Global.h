#pragma once
#ifdef ImGUI_ENABLED
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui.h"
#endif
struct ObjectProperties{
	virtual void show() = 0;
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
};
