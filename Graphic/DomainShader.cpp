#include "DomainShader.h"
#include "errors.h"
#define CHECK_ERROR(hr) if(FAILED(hr)) throw error::error(hr,__LINE__)
DomainShader_::DomainShader_(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext)
	:
	VertexShader_(pDevice, pContext)
{

}
void DomainShader_::bind() {
	CHECK_ERROR(D3DReadFileToBlob(L"DomainShader.cso", &pBlob));
	pDevice->CreateDomainShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pDomain);
	pContext->DSSetShader(pDomain.Get(), nullptr, 0);
}
