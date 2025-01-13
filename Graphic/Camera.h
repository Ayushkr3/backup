#pragma once
#include "ConstantBuff.h"
#include "errors.h"
#include "Global.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
class Camera:public Objects {
public:
	struct CameraProp:public ObjectProperties {
	public:
		float FoV = 60.0f; float Ratio = 1.333333333f;
		CameraProp(Objects* obj);
		float postion[3] = { 0 + associatedObj->Inheritence.InheritedTrans->position[0],0,-5.0f };
		float rotation[2] = { 0,0 };
		void show();
		ObjectProperties* GetPropertyRef() { return this; }
		const std::type_info& GetPropertyType() { return typeid(CameraProp); };
	};
	Camera(Microsoft::WRL::ComPtr<ID3D11Device> pDevice);
	CameraProp* PosNrot;
	std::vector<ObjectProperties*> CamProperties;
	std::vector<ObjectProperties*>* GetProperties() { return &CamProperties; }
	float focus[3] = { 0,0,1 };
	void calculateProjection(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, PerFrameData* ViewMatrix);
	PerFrameData GetViewMatrix();
	XMMATRIX viewmatrix;
	XMMATRIX projection;
	void SetFOVnAspectRatio(float FOV,float ratio);
private:
	PerFrameData viewXprojection;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pFrameConstantBuffer;
};