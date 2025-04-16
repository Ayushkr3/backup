#pragma once
#include "Mesh.h"
#include "Cam.h"
#include "Structs.h"
#include "ECore.h"
class SkyBox :public Prefab {
	struct DSSkyBox :public Primitives::ConstantBufferData {
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	DSSkyBox DSsb;
	Camera* cam;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DSS;
public:
	CORE_EXP SkyBox(ID3D11Device*& pDevice, ID3D11DeviceContext*& pContext, std::vector<Vertex> vertice, std::vector<unsigned int>indi, short id, float rgba[3], short globalID, std::vector<NormalPerObject>nor, Camera* cam);
	CORE_EXP void Draw();
};
