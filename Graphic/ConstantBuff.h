#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d11.h>
#include "Global.h"
#include <vector>

using namespace DirectX;
struct AABB {
	float minI = FLT_MAX;
	float maxI = -FLT_MIN;
	float maxJ = -FLT_MIN;
	float minJ = FLT_MAX;
	float maxK = -FLT_MIN;
	float minK = FLT_MAX;
};
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
	XMVECTOR ivec;
	XMVECTOR jvec;
	XMVECTOR kvec;
	struct rgbaS {
		float rgb[3] = {1.0f,1.0f,1.0f};
		rgbaS(float r, float g, float b) :rgb{r,g,b} {}
	};
	std::vector<float> dataVector;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pObjectConstantBuffer;
public:
	 void BindToPSshader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	 void BindToPSshader(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, float rgba[3]);
	 ConstantBuffer(PerObjectData* SubResource, Microsoft::WRL::ComPtr<ID3D11Device> pDevice);
	 ConstantBuffer(LightData* SubResource, Microsoft::WRL::ComPtr<ID3D11Device> pDevice);
	 void BindToVSshader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	 void UpdateBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, PerObjectData* obj);
	 void UpdateBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer);
	 static XMFLOAT4X4 ConvertMatrixToFloat4x4(XMMATRIX mat);
public:
	AABB aabb;
	void Transform(TransformStruct* t,std::vector<NormalPerObject>& n);
	XMFLOAT4X4 viewmat;
	void GetData(std::vector<Vertex> vertices);
	std::vector<Vertex> vertice_m;
	std::vector<Vertex> vertice_f;
};
