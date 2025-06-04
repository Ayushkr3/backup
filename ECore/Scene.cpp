#include "pch.h"
#include "Scene.h"
#include "Shapes.h"
#include "ObjParser.h"
#include "EUI.h"
short Scene::currentOBJID = 10;
using namespace physx;
std::vector<short> Scene::globalCurrentOBJID = {};
Scene::Scene(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext)
	:cam(pDevice), pContext(pContext), pDevice(pDevice)

{
	pDumbDevice = pDevice.Get();
	pDumbContext = pContext.Get();
	NVPhysx::CreateNewScene(physScene);
	LoadSkyBox();
	cam.CamProperties.push_back(&dc);
	dc.SetCameraProp(cam.PosNrot);
	//-------------------------------------//
	UI::Button* testButton = new UI::Button(GetCurrentID());
	AllObject.push_back(testButton);
	UI::Button* testButton2 = new UI::Button(GetCurrentID());
	AllObject.push_back(testButton2);
	//-------------------------------------//
	AllObject.push_back(&cam);
}
Scene::~Scene() {
	physScene->release();
}
void Scene::Render() {
	dc.DebugCamMovement();
	cam.calculateProjection(pContext, &(cam.GetViewMatrix()));
	//TODO: Fix proper lighting
	pSkyBox->Draw();
	for (auto& obj : AllObject) {
		obj->Update();
		if (dynamic_cast<Renderable*>(obj) != nullptr) {
			Renderable* red = dynamic_cast<Renderable*>(obj);
			red->UpdateBuffers();
			red->Draw();
		}
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
	physScene->simulate(PxReal(1.0f / 60.0f));
	for (auto& Obj : AllObject) {
		Obj->inPlayMode();	//Call every object properties which want to act in play Mode
	}
	auto n = physScene->fetchResults(true);
	//CContoller->Update();
}
void Scene::InitalizePlayMode()
{
	inPlayMode = true;
	//CContoller = std::make_unique<ColliderController>();
	for (int i = 0; i < AllObject.size();i++) {
		Prefab* Triangle = dynamic_cast<Prefab*>(AllObject[i]);
		AllObject[i]->InitializePlayMode();
		if (!Triangle)continue;
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
	Prefab* t = dynamic_cast<Prefab*>(obj);
	if(t!=nullptr)
		globalCurrentOBJID.push_back(t->id); //local push back
	Objects::GlobalIdPool.push_back(obj->Id); //global push back
	obj->RemoveHeritence();
	AllObject.erase(LookUp(obj->Id, AllObject));
	if (t != nullptr) {
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
	//delete t;
	if (obj != nullptr) {
		delete obj;
		obj = nullptr;
	}
	t = nullptr;
}
void Scene::DeInitalizePlayMode() {
	inPlayMode = false;
	for (int i = 0; i < AllObject.size(); i++) {
		Prefab* Triangle = dynamic_cast<Prefab*>(AllObject[i]);
		AllObject[i]->DeInitializePlayMode();
		if (!Triangle)continue;
		for (auto& prop : Triangle->ObjProperties) {
			if (auto* rb = dynamic_cast<NVPhysx::RigidBody*>(prop)) {
				physScene->removeActor(*rb->GetCurrentActor());
			}
		}
	}
#ifdef WIN32_DEBUG
	for (int i = 0; i < DeleteAfterPlay.size(); i++) {
		DeleteObject(DeleteAfterPlay[i]);
	}
	DeleteAfterPlay.clear();
#endif 
}
std::vector<Objects*>::iterator Scene::LookUp(short id, std::vector<Objects*>& vec) {
	for (auto it = vec.begin(); it != vec.end(); it++) {
		if ((*it)->Id == id)
			return it;
	}
	return vec.end();
}
std::vector<Prefab*>::iterator Scene::LookUp(Prefab* Tri, std::vector<Prefab*>& vec) {
	for (auto it = vec.begin(); it != vec.end(); it++) {
		if ((*it)->id == Tri->id)
			return it;
	}
	return vec.end();
}
void Scene::LoadSkyBox() {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector <NormalPerObject> n;
	float color[3];
	indices.clear();
	vertices.clear();
	Primitives::BasicShapes::Circle::GetCircleofLongNLat(10, 10, vertices, indices);
	//Set position of sky box to Camera
	pSkyBox = std::make_unique<SkyBox>(pDumbDevice, pDumbContext, vertices, indices, currentOBJID++, color, ++Objects::count, n, &cam);
}
Objects* Scene::AddObject(Objects* ob,bool isRenderable) {
	short ObjectID;
	Objects* ret = nullptr;
	if (!Scene::globalCurrentOBJID.empty()) {
		ObjectID = Scene::globalCurrentOBJID[0];
		Scene::globalCurrentOBJID.erase(Scene::globalCurrentOBJID.begin());
	}
	else {
		ObjectID = Scene::currentOBJID++;
	}
	if (isRenderable) {
		Mesh* mesh = dynamic_cast<Mesh*>(ob);
		Prefab* obj = new Prefab(*mesh, ObjectID);
		Triangles.push_back(obj);
		AllObject.push_back(obj);
		ret = obj;
#ifdef WIN32_DEBUG
		if (inPlayMode) {
			DeleteAfterPlay.push_back(obj);
		}
#endif
	}
	else {
		AllObject.push_back(ob);
#ifdef WIN32_DEBUG
		if (inPlayMode) {
			DeleteAfterPlay.push_back(ob);
		}
#endif
	}
	return ret;
}
short Scene::GetCurrentID() {
	short globalID;
	if (!Objects::GlobalIdPool.empty()) {
		globalID = Objects::GlobalIdPool[0];
		Objects::GlobalIdPool.erase(Objects::GlobalIdPool.begin());
	}
	else {
		globalID = ++Objects::count;
	}
	return globalID;
}
Objects* Scene::ParseObject(Serialization::ObjectBlocks OB) {
	if (OB.ClassName == "Prefab") {
		std::istringstream stream(OB.blockBuffer);
		std::string line;
		std::getline(stream, line);
		std::getline(stream, line); //0
		auto path = line;
		int off = line.find(":");
		std::getline(stream, line);//1
		int off2 = line.find(":");

		PathToFile* ptf = new PathToFile(path.substr(off + 1, path.size() - 1), line.substr(off + 1, line.size() - 1));
		GetFile(ptf->Path);
		std::vector<Vertex> v;
		std::vector<unsigned int> indi;
		for (unsigned int i = 0; i < Vertexformated.size(); i++) {
			v.emplace_back(Vertexformated[i]);
			indi.push_back(i);
		}
		Mesh* newMesh = new Mesh(pDevice, pContext, GetCurrentID(),v,indi,ptf);
		short ObjectID;
		if (!Scene::globalCurrentOBJID.empty()) {
			ObjectID = Scene::globalCurrentOBJID[0];
			Scene::globalCurrentOBJID.erase(Scene::globalCurrentOBJID.begin());
		}
		else {
			ObjectID = Scene::currentOBJID++;
		}
		Prefab* newPrefab = new Prefab(*newMesh,ObjectID );
		for (auto& it : OB.propBlocks) {
			Serialization::DeSerializeObject(newPrefab,it);
		}
		dynamic_cast<Primitives::Material*>(newPrefab->ObjProperties[1])->CreateCBuffer(0, sizeof(CB::PerObjectData), Primitives::DOMAIN_SHADER);
		Triangles.push_back(newPrefab);
		return newPrefab;
	}
	if (OB.ClassName == "NObject") {
		NullObject* no = new NullObject(Scene::GetCurrentID());
		for (auto& it : OB.propBlocks) {
			Serialization::DeSerializeObject(no, it);
		}
		AddObject(no, false);
	}
	return nullptr;
}
void Scene::LoadScene() {
	std::vector<Serialization::ObjectBlocks> ObjectsBlocks = Serialization::ReadFromFile();
	for (auto& it : ObjectsBlocks) {
		Objects* o = ParseObject(it);
		if(o!=nullptr)
		AllObject.push_back(o);
	}
}
void Scene::SaveScene() {
	std::string buffer;
	for (auto &it : AllObject) {
		buffer +=Serialization::SerializeObject(*it);
	}
	Serialization::SaveToFile(buffer);
}