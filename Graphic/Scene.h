#pragma once
#include <d3d11.h>
#include "PxPhysicsAPI.h"
#include <wrl.h>
#include "SkyBox.h"
#include "Triangle.h"
#include "Camera.h"
#include "Light.h"
#include "Phys.h"
#include "ColliderController.h"
class Scene {
private:
	std::vector<Objects*>::iterator LookUp(short id, std::vector<Objects*>& vec);
	std::vector<Triangle*>::iterator LookUp(Triangle* Tri, std::vector<Triangle*>& vec);
	std::unique_ptr<SkyBox> pSkyBox;
	void LoadSkyBox();
	Camera::DebugCamera dc;
public:
	void SaveScene();
	static short currentOBJID;
	static std::vector<short> globalCurrentOBJID;
	Scene(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	void Render();
	void RenderWireFrame();
	void PlayMode();
	void InitalizePlayMode();
	void DeInitalizePlayMode();
	void DeleteObject(Objects* obj);
	Timer tim;
public:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	ID3D11Device* pDumbDevice;
	ID3D11DeviceContext* pDumbContext;
	~Scene();
public:
	physx::PxScene* physScene;
	Camera cam;
	Light light;
	std::vector<Triangle*>Triangles;
	std::vector<Objects*>AllObject; 
};