#pragma once
#include <vector>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <iterator>
#include <wrl.h>
#include "errors.h"
#include "Structs.h"
#include "EInputManager.h"
#include "Primitives.h"
#include "ECore.h"
class Prefab :public Objects
{
public:
	short id;
	std::vector<Vertex> vertices; // = { {0.0f,0.0f, 255, 0, 0, 0 }, { 0.5f,-0.5f,0,255,0,0 }, { -0.5f,-0.5f,0,0,255,0 } };
	std::vector<unsigned int> index;
	CORE_EXP Prefab(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, std::vector<Vertex> vertice, std::vector<unsigned int> indi, short id, float rgba[3], short globalID, std::vector<NormalPerObject>nor = {});
	CORE_EXP ~Prefab();
	bool isMoving;
	CORE_EXP virtual void Draw();
	CORE_EXP void UpdateBuffers();
	CORE_EXP void inPlayMode();
	PerObjectData Transformation;
	CORE_EXP bool operator<(const Prefab& secondObj)const;
	CORE_EXP bool operator==(const Prefab& secondObj)const;
	CORE_EXP std::vector<Vertex> GetVertices();
protected:
	UINT strides = sizeof(Vertex);
	UINT offset = 0u;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	//TODO: Move Shader to SCENE class
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	std::vector<NormalPerObject> n;
public:
	TransformStruct* Trans;
	std::vector<NormalPerObject> rn; //Transformed normals
	std::vector<ObjectProperties*> ObjProperties;
	Primitives::Material* Mat;
public:
	CORE_EXP void ReCalculatePosition();
	CORE_EXP void InitializePlayMode();
	CORE_EXP void Highlight();
	CORE_EXP void Restore();
	float color[3];
	float last_color[3];
public:
	//
	//
	CORE_EXP std::vector<ObjectProperties*>* GetProperties();
};


