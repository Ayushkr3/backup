#include "PixelShader.h"

PixelShader_::PixelShader_(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext)
	:
	VertexShader_(pDevice,pContext)
{

}

void PixelShader_::bind() {
	D3DReadFileToBlob(L"PixelShader.cso",&pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);
}