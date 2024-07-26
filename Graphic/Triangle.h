#pragma once
#include <vector>
#include "VertexShader.h"
#include "PixelShader.h"
#include <d3dcompiler.h>
#include <d3d11.h>
#include <iterator>
#include<wrl.h>
#include "errors.h"
#include "timer.h"
#include "ConstantBuff.h"
#include "Mouse.h"
#include "WIC.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "Colliders.h"
#include "Phys.h"
#include "GeometryShader.h"
#include "EPhysics.h"

#define CHECK_ERROR(hr) if(FAILED(hr)) throw error::error(hr,__LINE__)

class Triangle:public Objects
{
public:
	short id;
	std::vector<Vertex> vertices; // = { {0.0f,0.0f, 255, 0, 0, 0 }, { 0.5f,-0.5f,0,255,0,0 }, { -0.5f,-0.5f,0,0,255,0 } };
	std::vector<unsigned int> index;
	Triangle(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, std::vector<Vertex> vertice, std::vector<unsigned int> indi, short id, float rgba[3],short globalID, std::vector<NormalPerObject>nor = {});
	bool isMoving;
	void Draw();
	void UpdateBuffers();
	void inPlayMode();
	PerObjectData Transformation;
	bool operator<(const Triangle& secondObj)const;
	bool operator==(const Triangle& secondObj)const;
	std::unique_ptr<ConstantBuffer> pCB;
	std::vector<Vertex> GetVertices();
private:
	UINT strides = sizeof(Vertex);
	UINT offset = 0u;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	//TODO: Move Shader to SCENE class
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11HullShader> pHull;
	Microsoft::WRL::ComPtr<ID3D11DomainShader> pDSS;
	std::vector<NormalPerObject> n;
public:
	TransformStruct* Trans;
	std::vector<NormalPerObject> rn; //Transformed normals
	std::vector<ObjectProperties*> ObjProperties;
	std::unique_ptr<PixelShader_> pPshader;
	//std::unique_ptr<HullShader_> pHull;
	//std::unique_ptr<DomainShader_> pDomain;
	std::unique_ptr<GeometryShader_> pGeo;
	std::unique_ptr<VertexShader_> pVshader;
public:
	void InitializePlayMode();
	void Highlight();
	void Restore();
	void UpdateCollider();
	BoxCollider coll;
	Physics_Body phy;
	float color[3];
	float last_color[3];
public:
	std::vector<ObjectProperties*>* GetProperties();
};


