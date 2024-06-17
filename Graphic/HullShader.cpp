#include "HullShader.h"
#include "errors.h"
#define CHECK_ERROR(hr) if(FAILED(hr)) throw error::error(hr,__LINE__)
HullShader_::HullShader_(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext)
	:
	VertexShader_(pDevice, pContext)
{

}
void HullShader_::bind() {
	Microsoft::WRL::ComPtr<ID3D10Blob> pHullBlob;
	CHECK_ERROR(D3DReadFileToBlob(L"HullShader.cso",&pHullBlob));
	CHECK_ERROR(pDevice->CreateHullShader(pHullBlob->GetBufferPointer(), pHullBlob->GetBufferSize(),nullptr,&pHull));
	pContext->HSSetShader(pHull.Get(), nullptr, 0);
}
