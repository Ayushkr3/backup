#include "Scene.h"
#include "ConstantBuff.h"

Scene::Scene(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext)
	:cam(pDevice), pContext(pContext),light(pDevice)

{
	CContoller = std::make_unique<ColliderController>();
	for (int i = 0; i <2; i++) {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> index;
		if (i == 0) {
			 vertices = { {-1.0f, -1.0f, -1.0f, 1,1, 0.0f,0.0f,-1.0f},
	{1.0f,  -1.0f, -1.0f,0,1, 0.0f,0.0f,-1.0f},
	{-1.0f,  1.0f, -1.0f,1,0, 0.0f,0.0f,-1.0f },
	{1.0f,   1.0f, -1.0f,0,0, 0.0f,0.0f,-1.0f},

	{-1.0f,  1.0f, -1.0f,0,1, 0.0f,1.0f,0.0f},
	{1.0f,  1.0f, 1.0f,1,0,  0.0f,1.0f,0.0f},
	{-1.0f,  1.0f, 1.0f,1,1, 0.0f,1.0f,0.0f},
	{1.0f,  1.0f, -1.0f,0,0, 0.0f,1.0f,0.0f},

	{-1.0f,-1.0f,-1.0f,1,1,0.0f,-1.0f,0.0f},
	{-1.0f,-1.0f,1.0f,0,1,0.0f,-1.0f,0.0f},
	{1.0f,-1.0f,1.0f,1,1,0.0f,-1.0f,0.0f},
	{1.0f,-1.0f,-1.0f,1,0,0.0f,-1.0f,0.0f},

	{1.0f,1.0f,-1.0f,1,0,1.0f,0.0f,0.0f },
	{1.0f,1.0f,1.0f,1,0,1.0f,0.0f,0.0f},
	{1.0f,-1.0f,-1.0f,1,1,1.0f,0.0f,0.0f},
	{1.0f,-1.0f,1.0f,1,0,1.0f,0.0f,0.0f},

	{-1.0f,1.0f,-1.0f,0,0,-1.0f,0.0f,0.0f},
	{-1.0f,1.0f,1.0f,0,1,-1.0f,0.0f,0.0f},
	{-1.0f,-1.0f,-1.0f,1,0,-1.0f,0.0f,0.0f},
	{-1.0f,-1.0f,1.0f,1,1,-1.0f,0.0f,0.0f},

	{-1.0f, -1.0f, 1.0f, 1,1, 0.0f,0.0f,1.0f},
	{1.0f,  -1.0f, 1.0f,0,1, 0.0f,0.0f,1.0f},
	{-1.0f,  1.0f, 1.0f,1,0, 0.0f,0.0f,1.0f },
	{1.0f,   1.0f, 1.0f,0,0, 0.0f,0.0f,1.0f}, };
			 index = { 0,2,1,2,3,1,
		4,6,5,4,5,7,
		8,10,9,8,10,11,
		12,13,14,14,15,13,
		16,17,18,17,18,19,
		20,22,21,22,23,21 };
		}
		else if(i==1) {
			 vertices = { {-5.0f,0.0f,-5.0f,0,0,0.0f,1.0f,0.0f},
											 {5.0f,0.0f,-5.0f,0,0,0.0f,1.0f,0.0f},
											 {-5.0f,0.0f,5.0f,0,0,0.0f,1.0f,0.0f},
											 {5.0f,0.0f,5.0f,0,0,0.0f,1.0f,0.0f} };
			 index = {0,1,2,1,2,3};
		}
		else {
			vertices = { {-1.0f, -1.0f, -1.0f, 1,1, 0.0f,0.0f,-1.0f},
	{1.0f,  -1.0f, -1.0f,0,1, 0.0f,0.0f,-1.0f},
	{-1.0f,  1.0f, -1.0f,1,0, 0.0f,0.0f,-1.0f },
	{1.0f,   1.0f, -1.0f,0,0, 0.0f,0.0f,-1.0f},

	{-1.0f,  1.0f, -1.0f,0,1, 0.0f,1.0f,0.0f},
	{1.0f,  1.0f, 1.0f,1,0,  0.0f,1.0f,0.0f},
	{-1.0f,  1.0f, 1.0f,1,1, 0.0f,1.0f,0.0f},
	{1.0f,  1.0f, -1.0f,0,0, 0.0f,1.0f,0.0f},

	{-1.0f,-1.0f,-1.0f,1,1,0.0f,-1.0f,0.0f},
	{-1.0f,-1.0f,1.0f,0,1,0.0f,-1.0f,0.0f},
	{1.0f,-1.0f,1.0f,1,1,0.0f,-1.0f,0.0f},
	{1.0f,-1.0f,-1.0f,1,0,0.0f,-1.0f,0.0f},

	{1.0f,1.0f,-1.0f,1,0,1.0f,0.0f,0.0f },
	{1.0f,1.0f,1.0f,1,0,1.0f,0.0f,0.0f},
	{1.0f,-1.0f,-1.0f,1,1,1.0f,0.0f,0.0f},
	{1.0f,-1.0f,1.0f,1,0,1.0f,0.0f,0.0f},

	{-1.0f,1.0f,-1.0f,0,0,-1.0f,0.0f,0.0f},
	{-1.0f,1.0f,1.0f,0,1,-1.0f,0.0f,0.0f},
	{-1.0f,-1.0f,-1.0f,1,0,-1.0f,0.0f,0.0f},
	{-1.0f,-1.0f,1.0f,1,1,-1.0f,0.0f,0.0f},

	{-1.0f, -1.0f, 1.0f, 1,1, 0.0f,0.0f,1.0f},
	{1.0f,  -1.0f, 1.0f,0,1, 0.0f,0.0f,1.0f},
	{-1.0f,  1.0f, 1.0f,1,0, 0.0f,0.0f,1.0f },
	{1.0f,   1.0f, 1.0f,0,0, 0.0f,0.0f,1.0f}, };
			index = { 0,2,1,2,3,1,
	   4,6,5,4,5,7,
	   8,10,9,8,10,11,
	   12,13,14,14,15,13,
	   16,17,18,17,18,19,
	   20,22,21,22,23,21 };
		}
		Triangles.emplace_back(pDevice, pContext, vertices, index, i);
	}
	//-------------------dummy--------------//
	Triangles[1].Phys.isAffectedByGravity = false;
	Triangles[0].Trans->position[1] = 4.0f;
	Triangles[1].Trans->position[1] = -1.0f;
	//-------------------------------------//
	for (auto& Triangle : Triangles) {
		CContoller->AddTriangle(&Triangle);
	}
	CContoller->InitalizePosition();
	CContoller->ConstructTree();
}
void Scene::Render() {
	cam.calculateProjection(pContext, &(cam.GetViewMatrix()));
	//light.UpdateBuffer(pContext);
	//TODO: Fix proper lighting
	//TODO: BroadPhase collision update on rotation
	for (auto& Triangle : Triangles) {
		Triangle.Draw();
		Triangle.UpdateBuffers();
		CContoller->Update(&Triangle);
	}
}
