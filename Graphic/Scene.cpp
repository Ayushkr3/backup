#include "Scene.h"
#include "ConstantBuff.h"
short Scene::currentOBJID = 10;
std::vector<short> Scene::globalCurrentOBJID  = {};
Scene::Scene(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext)
	:cam(pDevice), pContext(pContext),light(pDevice),pDevice(pDevice)

{
	AllObject.push_back(&cam);
}
void Scene::Render() {
	cam.calculateProjection(pContext, &(cam.GetViewMatrix()));
	//light.UpdateBuffer(pContext);
	//TODO: Fix proper lighting
	//TODO: BroadPhase collision update on rotation
	for (auto& Triangle : Triangles) {
		Triangle->Draw();
		//Triangle->UpdateBuffers();
		//
	}
}
void Scene::RenderWireFrame() {
	for (auto& Triangle : Triangles) {
		Triangle->Draw();
		Triangle->UpdateBuffers();
		//CContoller->Update(Triangle);
	}
}

void Scene::PlayMode()
{
	for (auto& Triangle : Triangles) {
		for (auto& Prop : *Triangle->GetProperties()) {
			Prop->inPlayMode();
		}
		Triangle->inPlayMode();
	}
	CContoller->Update();
}
void Scene::InitalizePlayMode()
{
	CContoller = std::make_unique<ColliderController>();
	for (auto& Triangle : Triangles) {
		Triangle->InitializePlayMode();
		CContoller->AddTriangle(Triangle);
		CContoller->InitalizePosition(Triangle);
	}
	CContoller->ConstructTree();
}
