#include "VertexShader.h"
#include <iterator>
VertexShader_::VertexShader_(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext) :
	pDevice(pDevice),pContext(pContext)
{

}
void VertexShader_::bind()
{
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pvShader;
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pvShader);
	pContext->VSSetShader(pvShader.Get(), nullptr, 0);
}
Microsoft::WRL::ComPtr<ID3D10Blob>VertexShader_::rBlob() {
	return pBlob;
}
