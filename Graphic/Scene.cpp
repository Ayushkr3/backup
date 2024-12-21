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
#ifndef WIREFRAME_ENABLED
		Triangle->UpdateBuffers();
#endif // !WIREFRAME_ENABLED
	}
}
void Scene::RenderWireFrame() {
	for (auto& Triangle : Triangles) {
		Triangle->Draw();
#ifdef WIREFRAME_ENABLED
		Triangle->UpdateBuffers();
#endif 
		//Triangle->UpdateBuffers();
		//CContoller->Update(Triangle);
	}
}

void Scene::PlayMode()
{
	physScene->simulate(PxReal(1.0f/60.0f));
	for (auto& Triangle : Triangles) {
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

void Scene::DeleteObject(Objects* obj) {
	AllObject.erase(LookUp(obj->Id, AllObject));
	Triangle* t = dynamic_cast<Triangle*>(obj);
	if (t!= nullptr) {
		for (auto& prop : t->ObjProperties) {
			if (dynamic_cast<NVPhysx::RigidBody*>(prop)) {
				auto p = dynamic_cast<NVPhysx::RigidBody*>(prop);
				if (p->DynamicActor != nullptr) {
					if (p->DynamicActor->getScene() != nullptr) {
						physScene->removeActor(*p->DynamicActor);
					}
				}
				if (p->StaticActor != nullptr) {
					if (p->StaticActor->getScene() != nullptr) {
						physScene->removeActor(*p->StaticActor);
					}
				}
				break;
			}
		}
		Triangles.erase(LookUp(t, Triangles));
	}
	delete t;
	t = nullptr;
}
void Scene::DeInitalizePlayMode() {
	for (auto& Triangle : Triangles) {
		for (auto& prop : Triangle->ObjProperties) {
			prop->DeInitPlayMode();
			if (auto* rb = dynamic_cast<NVPhysx::RigidBody*>(prop)) {
				physScene->removeActor(*rb->GetCurrentActor());
			}
		}
	}
}
std::vector<Objects*>::iterator Scene::LookUp(short id, std::vector<Objects*>& vec) {
	for (auto it = vec.begin(); it != vec.end(); it++) {
		if ((*it)->Id == id)
			return it;
	}
	return vec.end();
}
std::vector<Triangle*>::iterator Scene::LookUp(Triangle* Tri, std::vector<Triangle*>& vec) {
	for (auto it = vec.begin(); it != vec.end(); it++) {
		if ((*it)->id == Tri->id)
			return it;
	}
	return vec.end();
}