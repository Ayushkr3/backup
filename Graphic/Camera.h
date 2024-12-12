#include "ConstantBuff.h"
#include "errors.h"
#include "Global.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
class Camera:public Objects {
public:
	struct CameraProp:public ObjectProperties {
		CameraProp(Objects* obj);
		float postion[3] = { 0 + associatedObj->Inheritence.InheritedTrans->position[0],0,-5.0f };
		float rotation[2] = { 0,0 };
		void show() {
			ImGui::DragFloat2("Camera Rotation", rotation, 0.1f);
			ImGui::DragFloat3("Camera Position", postion, 0.1f);
		}
		ObjectProperties* GetPropertyRef() { return this; }
	};
	Camera(Microsoft::WRL::ComPtr<ID3D11Device> pDevice);
	CameraProp* PosNrot;
	std::vector<ObjectProperties*> CamProperties;
	std::vector<ObjectProperties*>* GetProperties() { return &CamProperties; }
	float focus[3] = { 0,0,1 };
	void calculateProjection(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, PerFrameData* ViewMatrix);
	PerFrameData GetViewMatrix();
private:
	PerFrameData viewXprojection;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pFrameConstantBuffer;
	XMMATRIX viewmatrix;
	XMMATRIX projection;
	
};