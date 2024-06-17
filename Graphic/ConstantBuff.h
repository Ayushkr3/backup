#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d11.h>
#include "Global.h"
#include <vector>
using namespace DirectX;
struct PerObjectData {
	XMFLOAT4X4 Rotation;
	XMFLOAT4X4 Translation;
	XMFLOAT4X4 Scale;
};
struct LightData {
	XMFLOAT4 position;
};
struct PerFrameData {
	XMFLOAT4X4 viewMat;
};
class ConstantBuffer {
private:
	std::vector<float> dataVector;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pObjectConstantBuffer;
public:
	 void BindToPSshader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	 ConstantBuffer(PerObjectData* SubResource, Microsoft::WRL::ComPtr<ID3D11Device> pDevice);
	 ConstantBuffer(LightData* SubResource, Microsoft::WRL::ComPtr<ID3D11Device> pDevice);
	 void BindToVSshader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	 void UpdateBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, PerObjectData* obj);
	 void UpdateBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer);
	 static XMFLOAT4X4 ConvertMatrixToFloat4x4(XMMATRIX mat);
public:
	void Transform(float position[3],float rotation[3],float Scale[3]);
	XMFLOAT4X4 viewmat;
	void GetData(std::vector<Vertex> vertices);
	std::vector<Vertex> vertice_m;
	std::vector<Vertex> vertice_f;
};
