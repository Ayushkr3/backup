#include "Factory.h"
HMODULE ObjectPropertiesFactory::Userlib;
void ObjectPropertiesFactory::Init() {
	HotReloading::Init();
	NVPhysx::RigidBody::RegisterFactory();
	NVPhysx::Collider::BoxCollider::RegisterFactory();
	NVPhysx::Collider::PlaneCollider::RegisterFactory();
	Primitives::Material::RegisterFactory();
	
}
void D3DFactory::Init(Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pContext) {
	Primitives::Material::InitalizeD3D11Factory(pDevice, pContext); //Shader Factory stuff
}
void D3DFactory::DeInit() {
	Primitives::Material::DeInitalizeD3D11Factory(); //Shader Factory stuff
}
void ImguiContextFactory::Init(Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pContext,ImGuiContext* ctx) {
	NVPhysx::privateCtx = ctx;
	Primitives::Material::SetImguiCtx(ctx);
	IPC::Init(pDevice,pContext);
	IPC::Data->ctx = ctx;
	//Add all module(.lib ,.dll) ctx here
}
void ObjectPropertiesFactory::DeInit() {
	FreeLibrary(Userlib);
	HotReloading::DeInit();
}

void ObjectPropertiesFactory::InitModule()
{
	Userlib = LoadLibrary("Module_1.dll");
}
