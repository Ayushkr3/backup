#include "GeometryShader.h"
#include "errors.h"
#define CHECK_ERROR(hr) if(FAILED(hr)) throw error::error(hr,__LINE__)

GeometryShader_::GeometryShader_(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext)
	:
	VertexShader_(pDevice, pContext)
{

}

void GeometryShader_::bind() {
	CHECK_ERROR(D3DReadFileToBlob(L"GeometryShader.cso", &pBlob));
	pDevice->CreateGeometryShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pGeometryShader);
	pContext->GSSetShader(pGeometryShader.Get(), nullptr, 0);
}
void GeometryShader_::bind(Microsoft::WRL::ComPtr<ID3D11Buffer> soBuffer) {
	D3D11_SO_DECLARATION_ENTRY soEntry[] = {
		{0,"Position", 2, 0, 3, 0}
	};
	UINT offset = 0;
	const UINT stride = 12;
	CHECK_ERROR(D3DReadFileToBlob(L"GeometryShader.cso", &pBlob));
	CHECK_ERROR(pDevice->CreateGeometryShaderWithStreamOutput(pBlob->GetBufferPointer(),pBlob->GetBufferSize(),soEntry,1,&stride,1,0
	,nullptr,&pGeometryShader));
	pContext->GSSetShader(pGeometryShader.Get(), nullptr, 0);
	pContext->SOSetTargets(1,soBuffer.GetAddressOf(), &offset);
}