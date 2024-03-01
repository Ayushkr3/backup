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
#define CHECK_ERROR(hr) if(FAILED(hr)) throw error::error(hr,__LINE__)

class Triangle
{
struct Vertex {
	struct {
		float x;
		float y;
		float z;
	}position;
	struct {
		float u;
		float v;
	}texcoord;
	struct {
		float x;
		float y;
		float z;
	}Normal;
	};
public:
	Triangle(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);

	void Draw();
	void UpdateBuffers();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> objectTex;
	PerObjectData Transformation;
private:
	D3D11_BUFFER_DESC IndexBufferDesc;
	D3D11_BUFFER_DESC VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	std::unique_ptr<ConstantBuffer> pCB;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11HullShader> pHull;
	Microsoft::WRL::ComPtr<ID3D11DomainShader> pDSS;
	
public:
	float rotation[3] = { 0,0,45 };
	float position[3] = { 0,0,0 };
	float Scale[3] = { 1,1,1 };
	//std::unique_ptr<PixelShader_> pPshader;
	//std::unique_ptr<HullShader_> pHull;
	//std::unique_ptr<DomainShader_> pDomain;
	std::unique_ptr<VertexShader_> pVshader;
	std::vector<Vertex> vertices; // = { {0.0f,0.0f, 255, 0, 0, 0 }, { 0.5f,-0.5f,0,255,0,0 }, { -0.5f,-0.5f,0,0,255,0 } };
	std::vector<unsigned int> index = { 0,2,1 ,2,3,1,
										/*4,6,5,6,7,4*/};
};


