#include "Triangle.h"

Microsoft::WRL::ComPtr<ID3D11Device> pDevice_;
Triangle::Triangle(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext):
	pContext(pContext), vertices{
{-1.0f, -1.0f, -1.0f,255,0,0,0 },
{1.0f,  -1.0f, -1.0f,0,255,0,0 },
{-1.0f,  1.0f, -1.0f,0,255,255,0 },
{1.0f, 1.0f, -1.0f,0,255,0,0},
{-1.0f, -1.0f,  1.0f,0,0,255,0},
{1.0f,  -1.0f,  1.0f,0,255,0,0},
{ -1.0f,  1.0f,  1.0f,255,255,0,0},
{1.0f, 1.0f,  1.0f,255,255,255,0}
}
{
	D3D11_BUFFER_DESC VertexBuffer;
	VertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBuffer.Usage = D3D11_USAGE_DEFAULT;
	VertexBuffer.ByteWidth = (vertices.size()*sizeof(Vertex));
	VertexBuffer.CPUAccessFlags = 0u;
	VertexBuffer.StructureByteStride = sizeof(Vertex);
	VertexBuffer.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA sbr;
	sbr.pSysMem = vertices.data();

	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.CPUAccessFlags = 0u;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.ByteWidth = (index.size()*sizeof(unsigned int));
	IndexBufferDesc.StructureByteStride = sizeof(unsigned int);
	D3D11_SUBRESOURCE_DATA index_subr;
	index_subr.pSysMem = index.data();

	Transformation.Rotation = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixIdentity());
	Transformation.Translation = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixIdentity());
	Transformation.Scale = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixIdentity());
	pCB = std::make_unique<ConstantBuffer>(&Transformation,pDevice);

	CHECK_ERROR(pDevice->CreateBuffer(&VertexBuffer, &sbr, &pVertexBuffer));
	CHECK_ERROR(pDevice->CreateBuffer(&IndexBufferDesc, &index_subr,&pIndexBuffer));
	

	UINT strides = sizeof(Vertex);
	UINT offset = 0u;
	pContext->IASetVertexBuffers(0, 1,pVertexBuffer.GetAddressOf(), &strides, &offset);
	pContext->IASetIndexBuffer(pIndexBuffer.Get(),DXGI_FORMAT_R32_UINT,0);
	//pContext->VSSetConstantBuffers(0, 1, pConstBuffer.GetAddressOf());
	pCB->BindToVSshader(pContext);

	std::unique_ptr<VertexShader_> pVshader;
	pVshader = std::make_unique<VertexShader_>(pDevice,pContext);
	std::unique_ptr<VertexShader_> pPshader = std::make_unique<PixelShader_>(pDevice,pContext);

	pPshader->bind();
	pVshader->bind();
	
	Microsoft::WRL::ComPtr<ID3D11InputLayout>pIL;
	D3D11_INPUT_ELEMENT_DESC IL[] = {
		{ "Position",0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,12u,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	pDevice->CreateInputLayout(IL, std::size(IL), pVshader->rBlob()->GetBufferPointer(), pVshader->rBlob()->GetBufferSize(), &pIL);

	pContext->IASetInputLayout(pIL.Get());
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	
}
void Triangle::Draw() {
	pCB->BindToVSshader(pContext);
	pContext->DrawIndexed(index.size(), 0, 0);
}
void Triangle::UpdateBuffers() {
	Transformation.Rotation = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation[0]), XMConvertToRadians(rotation[1]), XMConvertToRadians(rotation[2])));
	Transformation.Translation = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixTranslation(position[0],position[1],position[2]));
	Transformation.Scale = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixScaling(Scale[0], Scale[1], Scale[2]));
	pCB->UpdateBuffer(pContext, &Transformation);
}