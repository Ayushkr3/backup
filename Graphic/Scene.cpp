#include "Scene.h"
#include "ConstantBuff.h"
short Scene::currentOBJID = 10;
using namespace physx;
std::vector<short> Scene::globalCurrentOBJID  = {};
Scene::Scene(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext)
	:cam(pDevice), pContext(pContext),light(pDevice),pDevice(pDevice)

{
	NVPhysx::CreateNewScene(physScene);
	AllObject.push_back(&cam);
}
Scene::~Scene() {
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
	physScene->simulate(PxReal(1.0f/60.0f));
	for (auto& Triangle : Triangles) {
		for (auto& Prop : *Triangle->GetProperties()) {
			Prop->inPlayMode();
		}
		Triangle->inPlayMode();	//Call every object properties which want to act in play Mode
	}
	physScene->fetchResults(true);
	//CContoller->Update();
}
void Scene::InitalizePlayMode()
{
	//CContoller = std::make_unique<ColliderController>();
	for (auto& Triangle : Triangles) {
		Triangle->InitializePlayMode();
		for (auto& prop : Triangle->ObjProperties) {
			if (auto* rb = dynamic_cast<NVPhysx::RigidBody*>(prop)) {
				physScene->addActor(*rb->GetCurrentActor());
			}
		}
		
		//CContoller->AddTriangle(Triangle);
		//CContoller->InitalizePosition(Triangle);  //Call initalization of all object which want to initalize in before play mode
	}
	//CContoller->ConstructTree();
}
