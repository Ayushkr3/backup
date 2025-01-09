#pragma once
#include "Triangle.h"
#include "Camera.h"

class SkyBox :public Triangle{
	struct DSSkyBox:public Primitives::ConstantBufferData {
		XMMATRIX view;
		XMMATRIX projection;
	};
	DSSkyBox DSsb;
	Camera* cam;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DSS;
public:
	SkyBox(ID3D11Device*& pDevice, ID3D11DeviceContext*& pContext, std::vector<Vertex> vertice, std::vector<unsigned int>indi, short id, float rgba[3], short globalID, std::vector<NormalPerObject>nor,Camera* cam);
	void Draw();
};
