#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d11.h>
using namespace DirectX;
struct PerObjectData {
	XMFLOAT4X4 Rotation;
	XMFLOAT4X4 Translation;
	XMFLOAT4X4 Scale;
};
struct PerFrameData {
	XMFLOAT4X4 viewMat;
};
class ConstantBuffer {
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pObjectConstantBuffer;
public:
	 ConstantBuffer(PerObjectData* SubResource, Microsoft::WRL::ComPtr<ID3D11Device> pDevice);
	 void BindToVSshader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	 void UpdateBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, PerObjectData* obj);
	 static XMFLOAT4X4 ConvertMatrixToFloat4x4(XMMATRIX mat);

	
};
