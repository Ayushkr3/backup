#include "Factory.h"
void ObjectPropertiesFactory::Init() {
	NVPhysx::RigidBody::RegisterFactory();
	NVPhysx::BoxCollider::RegisterFactory();
}
void D3DFactory::Init(Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pContext) {
	Primitives::Material::InitalizeD3D11Factory(pDevice, pContext); //Shader Factory stuff
}
void D3DFactory::DeInit() {
	Primitives::Material::DeInitalizeD3D11Factory(); //Shader Factory stuff
}
void ImguiContextFactory::Init(ImGuiContext* ctx) {
	NVPhysx::privateCtx = ctx;
	Primitives::Material::SetImguiCtx(ctx);
}