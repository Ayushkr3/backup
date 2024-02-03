#include "ConstantBuff.h"
#include "errors.h"
#define CHECK_ERROR(hr) if(FAILED(hr)) throw error::error(hr,__LINE__)


ConstantBuffer::ConstantBuffer(PerObjectData* SubResource, Microsoft::WRL::ComPtr<ID3D11Device> pDevice) {
	D3D11_BUFFER_DESC CbuffDesc;
	CbuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CbuffDesc.ByteWidth = sizeof(PerObjectData);
	CbuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	CbuffDesc.MiscFlags = 0;
	CbuffDesc.StructureByteStride = 0;
	CbuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA Csbr;
	Csbr.pSysMem = SubResource;

	CHECK_ERROR(pDevice->CreateBuffer(&CbuffDesc,&Csbr,&pObjectConstantBuffer));
}
XMFLOAT4X4 ConstantBuffer::ConvertMatrixToFloat4x4(XMMATRIX mat) {
	XMFLOAT4X4 result;
	XMStoreFloat4x4(&result,mat);
	return result;
}
void ConstantBuffer::BindToVSshader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext) {
	pContext->VSSetConstantBuffers(0, 1, pObjectConstantBuffer.GetAddressOf());
}
void ConstantBuffer::UpdateBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext,PerObjectData* obj) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	pContext->Map(pObjectConstantBuffer.Get(),0, D3D11_MAP_WRITE_DISCARD,0,&mappedResource);
	memcpy(mappedResource.pData, obj, sizeof(PerObjectData));
	pContext->Unmap(pObjectConstantBuffer.Get(), 0);
}
