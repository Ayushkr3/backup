#include "Scene.h"
#include "ConstantBuff.h"
#include "Shapes.h"
short Scene::currentOBJID = 10;
using namespace physx;
std::vector<short> Scene::globalCurrentOBJID  = {};
Scene::Scene(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext)
	:cam(pDevice), pContext(pContext),light(pDevice),pDevice(pDevice)

{
	pDumbDevice = pDevice.Get();
	pDumbContext = pContext.Get();
	NVPhysx::CreateNewScene(physScene);
	LoadSkyBox();
	AllObject.push_back(&cam);
}
Scene::~Scene() {
}
void Scene::Render() {
	cam.calculateProjection(pContext, &(cam.GetViewMatrix()));
	//TODO: Fix proper lighting
	pSkyBox->Draw();
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
	auto n = physScene->fetchResults(true);
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
				//physScene->removeActor(*rb->GetCurrentActor());
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
void Scene::LoadSkyBox() {
	std::vector<Vertex> vertices = { {-1.0f, -1.0f, -1.0f, 0.0f, 1.0f}, // Bottom-left
	{ 1.0f, -1.0f, -1.0f, 1.0f, 1.0f}, // Bottom-right
	{ 1.0f,  1.0f, -1.0f, 1.0f, 0.0f}, // Top-right
	{-1.0f,  1.0f, -1.0f, 0.0f, 0.0f}, // Top-left

	// Back face
	{-1.0f, -1.0f,  1.0f, 1.0f, 1.0f}, // Bottom-left
	{ 1.0f, -1.0f,  1.0f, 0.0f, 1.0f}, // Bottom-right
	{ 1.0f,  1.0f,  1.0f, 0.0f, 0.0f}, // Top-right
	{-1.0f,  1.0f,  1.0f, 1.0f, 0.0f}, // Top-left

	// Left face
	{-1.0f, -1.0f,  1.0f, 0.0f, 1.0f}, // Bottom-left
	{-1.0f, -1.0f, -1.0f, 1.0f, 1.0f}, // Bottom-right
	{-1.0f,  1.0f, -1.0f, 1.0f, 0.0f}, // Top-right
	{-1.0f,  1.0f,  1.0f, 0.0f, 0.0f}, // Top-left

	// Right face
	{ 1.0f, -1.0f, -1.0f, 0.0f, 1.0f}, // Bottom-left
	{ 1.0f, -1.0f,  1.0f, 1.0f, 1.0f}, // Bottom-right
	{ 1.0f,  1.0f,  1.0f, 1.0f, 0.0f}, // Top-right
	{ 1.0f,  1.0f, -1.0f, 0.0f, 0.0f}, // Top-left

	// Top face
	{-1.0f,  1.0f, -1.0f, 0.0f, 1.0f}, // Bottom-left
	{ 1.0f,  1.0f, -1.0f, 1.0f, 1.0f}, // Bottom-right
	{ 1.0f,  1.0f,  1.0f, 1.0f, 0.0f}, // Top-right
	{-1.0f,  1.0f,  1.0f, 0.0f, 0.0f}, // Top-left

	// Bottom face
	{-1.0f, -1.0f,  1.0f, 0.0f, 1.0f}, // Bottom-left
	{ 1.0f, -1.0f,  1.0f, 1.0f, 1.0f}, // Bottom-right
	{ 1.0f, -1.0f, -1.0f, 1.0f, 0.0f}, // Top-right
	{-1.0f, -1.0f, -1.0f, 0.0f, 0.0f} };
	std::vector<unsigned int> indices = { 0, 2, 1,  0, 3, 2,
		// Back face
		4, 5, 6,  4, 6, 7,
		// Left face
		8, 10, 9, 8, 11, 10,
		// Right face
		12, 14, 13, 12, 15, 14,
		// Top face
		16, 18, 17, 16, 19, 18,
		// Bottom face
		20, 22, 21, 20, 23, 22 };
	std::vector <NormalPerObject> n;
	float color[3];
	indices.clear();
	vertices.clear();
	Primitives::BasicShapes::Circle::GetCircleofLongNLat(10, 10, vertices, indices);
	//Set position of sky box to Camera
	pSkyBox = std::make_unique<SkyBox>(pDumbDevice,pDumbContext,vertices,indices,currentOBJID++, color, ++Objects::count,n,&cam);
}