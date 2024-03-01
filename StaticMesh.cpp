#include "StaticMesh.h"
#include "WIC.h"
#include "VertexShader.h"
#include "PixelShader.h"
using namespace Microsoft;

std::vector<std::vector<Vertex>> StaticMesh::AllStaticMeshVertex = {};
std::vector<std::vector<unsigned int>> StaticMesh::AllStaticMeshIndex = {};
std::vector<ConstantBuffer> StaticMesh::pCB = {};


StaticMesh::StaticMesh(Microsoft::WRL::ComPtr<ID3D11Device>pDevice):pDevice(pDevice) {

}

void StaticMesh::AddToStaticMesh(std::vector<Vertex> vertices,std::vector<unsigned int> Index,PerObjectData* Transformation) {
	AllStaticMeshVertex.push_back(vertices);
	AllStaticMeshIndex.push_back(Index);
	pCB.emplace_back(Transformation,pDevice);
	sbr.push_back({});
	sbrIndex.push_back({});
	VertexBufferArray.push_back({});
	IndexBufferArray.push_back({});
}
void StaticMesh::BindStaticMesh(WRL::ComPtr<ID3D11DeviceContext>pContext) {
	D3D11_BUFFER_DESC VertexBuffer;
	VertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBuffer.Usage = D3D11_USAGE_IMMUTABLE;
	VertexBuffer.ByteWidth = (AllStaticMeshVertex[0].size() * sizeof(Vertex));
	VertexBuffer.CPUAccessFlags = 0u;
	VertexBuffer.StructureByteStride = sizeof(Vertex);
	VertexBuffer.MiscFlags = 0;

	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	IndexBufferDesc.CPUAccessFlags = 0u;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.ByteWidth = (AllStaticMeshIndex[0].size() * sizeof(unsigned int));
	IndexBufferDesc.StructureByteStride = sizeof(unsigned int);
	
	for (int t = 0; t < (int)AllStaticMeshVertex.size(); t++) {
		sbr[t].pSysMem = AllStaticMeshVertex[t].data();
		sbrIndex[t].pSysMem = AllStaticMeshIndex[t].data();

		CHECK_ERROR(pDevice->CreateBuffer(&VertexBuffer,&sbr[t],&VertexBufferArray[t]));
		CHECK_ERROR(pDevice->CreateBuffer(&IndexBufferDesc, &sbrIndex[t],&IndexBufferArray[t]));
		pCB[t].BindToVSshader(pContext);
	}
	const UINT stride[] = { sizeof(Vertex) };
	const UINT offset[] = { 0 };
	pContext->IASetVertexBuffers(0, VertexBufferArray.size(),VertexBufferArray.data()->GetAddressOf(),stride,offset);
	pContext->IASetIndexBuffer(IndexBufferArray.data()->Get(), DXGI_FORMAT_R32_UINT,0);
	////////////////////////////////////////////////////////////////////
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	Microsoft::WRL::ComPtr <ID3D11Resource>pR;
	CreateWICTextureFromFile(pDevice.Get(), pContext.Get(), L"rick.jpg", pR.GetAddressOf(), pSRV.GetAddressOf());
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = 128;
	desc.Height = 128;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	CHECK_ERROR(pDevice->CreateTexture2D(&desc, nullptr, pTexture.GetAddressOf()));
	pContext->UpdateSubresource(pTexture.Get(), 0, nullptr, pR.Get(), 0, 0);

	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSmpler;
	D3D11_SAMPLER_DESC smpdesc;
	smpdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	smpdesc.MipLODBias = 0;
	smpdesc.MaxLOD = D3D11_FLOAT32_MAX;
	smpdesc.MinLOD = 0;
	smpdesc.MaxAnisotropy = 1;
	smpdesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

	CHECK_ERROR(pDevice->CreateSamplerState(&smpdesc, &pSmpler));

	std::unique_ptr<VertexShader_> pVshader;
	pVshader = std::make_unique<VertexShader_>(pDevice, pContext);

	std::unique_ptr<PixelShader_> pPshader;
	pPshader = std::make_unique<PixelShader_>(pDevice, pContext);
	pPshader->bind(pSRV, pSmpler);
	pVshader->bind();

	Microsoft::WRL::ComPtr<ID3D11InputLayout>pIL;
	D3D11_INPUT_ELEMENT_DESC IL[] = {
		{ "Position",0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Texcoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12u,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0, DXGI_FORMAT_R32G32B32_FLOAT,0,20u,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	pDevice->CreateInputLayout(IL, std::size(IL), pVshader->rBlob()->GetBufferPointer(), pVshader->rBlob()->GetBufferSize(), &pIL);

	pContext->IASetInputLayout(pIL.Get());
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/////////////////////////////////////////////////////////////////////

}
void StaticMesh::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext>pContext) {
	for (int i = 0; i < (int)AllStaticMeshIndex.size(); i++) {
		pContext->DrawIndexed(AllStaticMeshIndex[i].size(),0,0);
	}
}