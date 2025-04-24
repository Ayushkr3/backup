#include "pch.h"
#include "Primitives.h"
using namespace Primitives;
ConstantBuffer::ConstantBuffer(PixelShader* PS,ID3D11Device*& pID):Ps(PS) {
	PixelShader::DefualtPixelShaderCB rgb;
	rgb.rgba[0] = 0.0f;
	rgb.rgba[1] = 0.0f;
	rgb.rgba[2] = 1.0f;
	rgb.rgba[3] = 1.0f;
	D3D11_BUFFER_DESC CbuffDesc;
	CbuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CbuffDesc.ByteWidth = sizeof(PixelShader::DefualtPixelShaderCB);
	CbuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	CbuffDesc.MiscFlags = 0;
	CbuffDesc.StructureByteStride = 0;
	CbuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA Csbr;
	Csbr.pSysMem = &Ps->PixelShaderData;
	pID->CreateBuffer(&CbuffDesc, &Csbr, &PS->CB.CBuffer);
}
void ConstantBuffer::UpdatePS(ID3D11DeviceContext*& pIC) {
	UpdateSubResource(&Ps->PixelShaderData, sizeof(PixelShader::DefualtPixelShaderCB),Ps->CB.CBuffer, pIC);
	pIC->PSSetConstantBuffers(3, 1, Ps->CB.CBuffer.GetAddressOf());
}
void ConstantBuffer::UpdateSubResource(void* Data, size_t sizeofData, Microsoft::WRL::ComPtr<ID3D11Buffer>& Buffer, ID3D11DeviceContext*& pIC) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = pIC->Map(Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (SUCCEEDED(hr)) {
		memcpy(mappedResource.pData, Data, sizeofData);
		pIC->Unmap(Buffer.Get(), 0);
	}
	else {
		CONSOLE_PRINT("D3D11 Map/Unmap Fails");
	}
}
ShaderConstantBuffer::ShaderConstantBuffer(ID3D11Device*& pID) {
	
}
