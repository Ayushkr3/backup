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
#include "Physics.h"
#include "Collider.h"
#include "Global.h"
#include "GeometryShader.h"
#define CHECK_ERROR(hr) if(FAILED(hr)) throw error::error(hr,__LINE__)

class Triangle
{
public:
	short id;
	Triangle(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext,std::vector<Vertex> vertice,std::vector<unsigned int> indi, short id);
	Physics_Body Phys;
	bool isMoving;
	void Draw();
	void UpdateBuffers();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> objectTex;
	PerObjectData Transformation;
	bool operator<(const Triangle& secondObj)const;
	bool operator==(const Triangle& secondObj)const;
	std::unique_ptr<ConstantBuffer> pCB;
	std::vector<Vertex> GetVertices();
private:
	UINT strides = sizeof(Vertex);
	UINT offset = 0u;
	D3D11_BUFFER_DESC IndexBufferDesc;
	D3D11_BUFFER_DESC VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	//TODO: Move Shader to SCENE class
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11HullShader> pHull;
	Microsoft::WRL::ComPtr<ID3D11DomainShader> pDSS;
	
public:
	TransformStruct* Trans;
	std::vector<ObjectProperties*> ObjProperties;
	std::unique_ptr<PixelShader_> pPshader;
	//std::unique_ptr<HullShader_> pHull;
	//std::unique_ptr<DomainShader_> pDomain;
	std::unique_ptr<GeometryShader_> pGeo;
	std::unique_ptr<VertexShader_> pVshader;
	std::vector<Vertex> vertices; // = { {0.0f,0.0f, 255, 0, 0, 0 }, { 0.5f,-0.5f,0,255,0,0 }, { -0.5f,-0.5f,0,0,255,0 } };
	std::vector<unsigned int> index;
public:
	void UpdateCollider();
	BoxCollider coll;
};


