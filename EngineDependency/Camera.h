#pragma once
#include "Global.h"
#include <wrl.h>
#include "Structs.h"
#include <d3d11.h>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "EInputManager.h"
#include "ECore.h"

class Camera :public Objects {
public:
	struct CameraProp :public ObjectProperties {
	public:
		float FoV = 60.0f; float Ratio = 1.333333333f;
		CORE_EXP CameraProp(Objects* obj);
		float postion[3] = { 0 + associatedObj->Inheritence.InheritedTrans->position[0],0,-5.0f };
		float rotation[2] = { 0,0 };
		CORE_EXP void show();
		CORE_EXP ObjectProperties* GetPropertyRef() { return this; }
		CORE_EXP const std::type_info& GetPropertyType() { return typeid(CameraProp); };
		DirectX::XMVECTOR forwardDirection;
	};
	CORE_EXP Camera(Microsoft::WRL::ComPtr<ID3D11Device> pDevice);
	CameraProp* PosNrot;
	std::vector<ObjectProperties*> CamProperties;
	CORE_EXP std::vector<ObjectProperties*>* GetProperties() { return &CamProperties; }
	float focus[3] = { 0,0,1 };
	CORE_EXP void calculateProjection(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, PerFrameData* ViewMatrix);
	CORE_EXP PerFrameData GetViewMatrix();
	DirectX::XMMATRIX viewmatrix;
	DirectX::XMMATRIX projection;

	CORE_EXP void SetFOVnAspectRatio(float FOV, float ratio);
	class DebugCamera :public ObjectProperties {
	private:
		CameraProp* Iprop = nullptr;
	public:
		CORE_EXP void SetCameraProp(CameraProp* prop);
		CORE_EXP void show() {};
		CORE_EXP void DebugCamMovement();
		ObjectProperties* GetPropertyRef();
		const std::type_info& GetPropertyType();
	};
private:
	PerFrameData viewXprojection;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pFrameConstantBuffer;
};