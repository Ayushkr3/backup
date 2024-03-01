#include "Triangle.h"


Triangle::Triangle(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext):
	pContext(pContext), vertices{
	{-1.0f, -1.0f, -1.0f, 1,1, 0.0f,0.0f,-1.0f},
	{1.0f,  -1.0f, -1.0f,0,1, 0.0f,0.0f,-1.0f},
	{-1.0f,  1.0f, -1.0f,1,0, 0.0f,0.0f,-1.0f },
	{1.0f,   1.0f, -1.0f,0,0, 0.0f,0.0f,-1.0f}

//	{-1.0f,  1.0f, -1.0f,/*0,1, 0.0f,1.0f,0.0f*/},
//	{1.0f,  1.0f, 1.0f,/*1,0,  0.0f,1.0f,0.0f*/},
//	{-1.0f,  1.0f, 1.0f,/*1,1, 0.0f,1.0f,0.0f*/},
//	{1.0f,  1.0f, -1.0f,/*0,0, 0.0f,1.0f,0.0f*/},
}
{
	VertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBuffer.Usage = D3D11_USAGE_IMMUTABLE;
	VertexBuffer.ByteWidth = (vertices.size() * sizeof(Vertex));
	VertexBuffer.CPUAccessFlags = 0u;
	VertexBuffer.StructureByteStride = sizeof(Vertex);
	VertexBuffer.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA sbr;
	sbr.pSysMem = vertices.data();


	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	IndexBufferDesc.CPUAccessFlags = 0u;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.ByteWidth = (index.size() * sizeof(unsigned int));
	IndexBufferDesc.StructureByteStride = sizeof(unsigned int);
	D3D11_SUBRESOURCE_DATA index_subr;
	index_subr.pSysMem = index.data();
	UINT strides = sizeof(Vertex);
	UINT offset = 0u;
	pContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &strides, &offset);
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	Transformation.Rotation = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation[0]), XMConvertToRadians(rotation[1]), XMConvertToRadians(rotation[2])));
	Transformation.Translation = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixTranslation(position[0], position[1], position[2]));
	Transformation.Scale = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixScaling(Scale[0], Scale[1], Scale[2]));
	pCB = std::make_unique<ConstantBuffer>(&Transformation, pDevice);

	CHECK_ERROR(pDevice->CreateBuffer(&VertexBuffer, &sbr, &pVertexBuffer));
	CHECK_ERROR(pDevice->CreateBuffer(&IndexBufferDesc, &index_subr, &pIndexBuffer));
	///////////////////////////////////////////////////////////
	
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	Microsoft::WRL::ComPtr <ID3D11Resource> pR;
	CHECK_ERROR(CreateWICTextureFromFile(pDevice.Get(), pContext.Get(), L"rick.jpg",pR.GetAddressOf(), pSRV.GetAddressOf()));
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
	pContext->UpdateSubresource(pTexture.Get(), 0, nullptr,pR.Get(), 0, 0);

	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	CHECK_ERROR(pDevice->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pSRV));
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

	CHECK_ERROR(pDevice->CreateSamplerState(&smpdesc,&pSmpler));

	/////////////////////////////////////////////////////////
	pCB->BindToVSshader(pContext);
	
	pVshader = std::make_unique<VertexShader_>(pDevice,pContext);
	//pHull = std::make_unique<HullShader_>(pDevice, pContext);
	//pDomain = std::make_unique<DomainShader_>(pDevice, pContext);
	std::unique_ptr<PixelShader_>pPshader = std::make_unique<PixelShader_>(pDevice, pContext);
	//Microsoft::WRL::ComPtr<ID3D10Blob> pHullBlob;
	//CHECK_ERROR(D3DReadFileToBlob(L"HullShader.cso",&pHullBlob));
	//pDevice->CreateHullShader(pHullBlob->GetBufferPointer(), pHullBlob->GetBufferSize(), nullptr, &pHull);
	//pContext->HSSetShader(pHull.Get(), nullptr, 0);
	//Microsoft::WRL::ComPtr<ID3D10Blob> pDSBlob;
	//CHECK_ERROR(D3DReadFileToBlob(L"DomainShader.cso", &pDSBlob));
	//pDevice->CreateDomainShader(pDSBlob->GetBufferPointer(), pDSBlob->GetBufferSize(), nullptr, &pDSS);
	//pContext->DSSetShader(pDSS.Get(), nullptr, 0);
	//pHull->bind();
	//pDomain->bind();
	pPshader->bind(pSRV,pSmpler);
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
	
}
void Triangle::Draw() {
	UINT strides = sizeof(Vertex);
	UINT offset = 0u;
	pContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &strides, &offset);
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	//Have to bind other wise it will not be rendered ,before every draw call
	pCB->BindToVSshader(pContext);
	//pContext->HSSetShader(pHull.Get(), nullptr, 0);
	//pContext->DSSetShader(pDSS.Get(), nullptr, 0);
	pContext->DrawIndexed(index.size(), 0, 0);
}
void Triangle::UpdateBuffers() {
	Transformation.Rotation = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation[0]), XMConvertToRadians(rotation[1]), XMConvertToRadians(rotation[2])));
	Transformation.Translation = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixTranslation(position[0],position[1],position[2]));
	Transformation.Scale = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixScaling(Scale[0], Scale[1], Scale[2]));
	// is there even any point to update if i have to bind it every frame ?
	pCB->UpdateBuffer(pContext, &Transformation);
}