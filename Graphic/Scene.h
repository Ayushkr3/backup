#pragma once
#include <d3d11.h>
#include "PxPhysicsAPI.h"
#include <wrl.h>
#include "Triangle.h"
#include "Camera.h"
#include "Light.h"
#include "Phys.h"
#include "ColliderController.h"
class Scene {
public:
	void SaveScene();
	static short currentOBJID;
	static std::vector<short> globalCurrentOBJID;
	//std::unique_ptr<ColliderController> CContoller;
	Scene(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	void Render();
	void RenderWireFrame();
	void PlayMode();
	void InitalizePlayMode();
	Timer tim;
public:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	~Scene();
public:
	physx::PxScene* physScene;
	Camera cam;
	Light light;
	std::vector<Triangle*>Triangles;
	std::vector<Objects*>AllObject; 
};