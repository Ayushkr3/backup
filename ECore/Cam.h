#pragma once
#include "ECore.h"
#include <DirectXMath.h>
#include "Global.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "Structs.h"
#include "Serialization.h"
class Camera :public Objects {
public:
	struct CameraProp :public ObjectProperties {
	private:
	public:
		float FoV = 60.0f; float Ratio = 1.333333333f;
		float postion[3] = { 0 + associatedObj->Inheritence.InheritedTrans->position[0],0,-5.0f };
		float rotation[2] = { 0,0 };
		REFLECT_BEGIN(CameraProp)
			REFLECT_VAR(FoV),
			REFLECT_VAR(Ratio)
		REFLECT_END()
	public:
		CORE_EXP CameraProp(Objects* obj);
		CORE_EXP void show();
		DirectX::XMVECTOR forwardDirection;
		CORE_EXP ObjectProperties* GetPropertyRef() { return this; }
		CORE_EXP const std::type_info& GetPropertyType() { return typeid(CameraProp); };
		CORE_EXP std::string Serialize();
		CORE_EXP std::string GetPropertyClassName();
	};
	CORE_EXP DirectX::XMVECTOR ProjectRayCast();
	CORE_EXP Camera(Microsoft::WRL::ComPtr<ID3D11Device> pDevice);
	CameraProp* PosNrot;
	std::vector<ObjectProperties*> CamProperties;
	CORE_EXP std::vector<ObjectProperties*>* GetProperties() { return &CamProperties; }
	float focus[3] = { 0,0,1 };
	CORE_EXP CB::PerFrameData __cdecl GetViewMatrix();
	CORE_EXP void __cdecl calculateProjection(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, CB::PerFrameData* ViewMatrix);
	DirectX::XMMATRIX viewmatrix;
	DirectX::XMMATRIX projection;

	CORE_EXP void SetFOVnAspectRatio(float FOV, float ratio);
	class DebugCamera :public ObjectProperties {
	private:
		CameraProp* Iprop = nullptr;
	public:
		CORE_EXP DebugCamera() :ObjectProperties() {};
		CORE_EXP std::string GetPropertyClassName() { return "DebugCamera"; }
		CORE_EXP void SetCameraProp(CameraProp* prop);
		CORE_EXP void show() {};
		CORE_EXP void DebugCamMovement();
		CORE_EXP ObjectProperties* GetPropertyRef();
		CORE_EXP const std::type_info& GetPropertyType();
		REFLECT_BEGIN(DebugCamera)
			REFLECT_VAR(Iprop)
		REFLECT_END()
	};
private:
	CB::PerFrameData viewXprojection;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pFrameConstantBuffer;
};