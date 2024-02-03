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
#define CHECK_ERROR(hr) if(FAILED(hr)) throw error::error(hr,__LINE__)

class Triangle
{
public:
	Triangle(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	void Draw();
	void UpdateBuffers();
	float rotation[3] = { 0,0,0 };
	float position[3] = { 0,0,0 };
	float Scale[3] = { 1,1,1 };
private:
	struct Vertex {
		float x;
		float y;
		float z;
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;
	std::unique_ptr<ConstantBuffer> pCB;
	PerObjectData Transformation;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	
public:
	std::vector<Vertex> vertices; // = { {0.0f,0.0f, 255, 0, 0, 0 }, { 0.5f,-0.5f,0,255,0,0 }, { -0.5f,-0.5f,0,0,255,0 } };
	std::vector<unsigned int> index = {
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4 };
};


