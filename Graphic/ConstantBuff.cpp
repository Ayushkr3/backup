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
	XMFLOAT3 icap = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3 jcap = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3 kcap = XMFLOAT3(0.0f, 0.0f, 1.0f);
	ivec = XMLoadFloat3(&icap);
	jvec = XMLoadFloat3(&jcap);
	kvec = XMLoadFloat3(&kcap);
}
ConstantBuffer::ConstantBuffer(LightData* SubResource, Microsoft::WRL::ComPtr<ID3D11Device> pDevice) {
	D3D11_BUFFER_DESC CbuffDesc;
	CbuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CbuffDesc.ByteWidth = sizeof(PerObjectData);
	CbuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	CbuffDesc.MiscFlags = 0;
	CbuffDesc.StructureByteStride = 0;
	CbuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA Csbr;
	Csbr.pSysMem = SubResource;

	CHECK_ERROR(pDevice->CreateBuffer(&CbuffDesc, &Csbr, &pObjectConstantBuffer));
}
XMFLOAT4X4 ConstantBuffer::ConvertMatrixToFloat4x4(XMMATRIX mat) {
	XMFLOAT4X4 result;
	XMStoreFloat4x4(&result,mat);
	return result;
}
////////////////////Provisional code//////////////////////////
void ConstantBuffer::Transform(TransformStruct* t, std::vector<NormalPerObject>& n)
{
	AABB aabbijk;
	XMMATRIX ro = XMMatrixTranspose(XMMatrixRotationRollPitchYaw(XMConvertToRadians(t->rotation[0]), XMConvertToRadians(t->rotation[1]), XMConvertToRadians(t->rotation[2])));
	XMMATRIX po = XMMatrixTranspose(XMMatrixTranslation(t->position[0], t->position[1], t->position[2]));
	XMMATRIX so = XMMatrixTranspose(XMMatrixScaling(t->Scale[0], t->Scale[1], t->Scale[2]));
	XMMATRIX view = (XMLoadFloat4x4(&viewmat));
	XMMATRIX fina = XMMatrixMultiply(XMMatrixMultiply(ro, po),so);
	XMFLOAT4X4 mat;
	XMFLOAT4X4 rota;
	XMStoreFloat4x4(&rota,ro);
	XMStoreFloat4x4(&mat, fina);
	for (unsigned int i = 0; i < vertice_m.size();i++) {
		XMFLOAT4 indi_vert = XMFLOAT4(vertice_m[i].position.x, vertice_m[i].position.y, vertice_m[i].position.z, 1.0f);
		XMFLOAT4 indi_norm = XMFLOAT4(vertice_m[i].Normal.x, vertice_m[i].Normal.y, vertice_m[i].Normal.z, 1.0f);
		XMVECTOR vec = XMVector4Transform(XMLoadFloat4(&indi_vert),XMLoadFloat4x4(&mat));
		XMVECTOR normal_rota = XMVector4Transform(XMLoadFloat4(&indi_norm),XMLoadFloat4x4(&rota));
		XMFLOAT4 returnpos;
		XMFLOAT4 returnnor;
		XMStoreFloat4(&returnpos, vec);
		XMStoreFloat4(&returnnor, normal_rota);
		XMVECTOR posvec = XMLoadFloat3(&XMFLOAT3(returnpos.x,returnpos.y,returnpos.z));
		float proji = XMVectorGetX(XMVector3Dot(posvec, ivec))+t->position[0];
		float projj = XMVectorGetX(XMVector3Dot(posvec, jvec))+ t->position[1];
		float projk = XMVectorGetX(XMVector3Dot(posvec, kvec))+ t->position[2];
		aabbijk.maxI = max(proji, aabbijk.maxI);
		aabbijk.minI = min(proji, aabbijk.minI);
		aabbijk.maxJ = max(projj, aabbijk.maxJ);
		aabbijk.minJ = min(projj, aabbijk.minJ);
		aabbijk.maxK = max(projk, aabbijk.maxK);
		aabbijk.minK = min(projk, aabbijk.minK);
		vertice_f[i].position.x = returnpos.x+(t->position[0]*t->Scale[0]);
		vertice_f[i].position.y = returnpos.y+ (t->position[1] * t->Scale[1]);
		vertice_f[i].position.z = returnpos.z+ (t->position[2] * t->Scale[2]);
		vertice_f[i].Normal.x = returnnor.x;
		vertice_f[i].Normal.y = returnnor.y;
		vertice_f[i].Normal.z = returnnor.z;

		if (i < n.size()) {
			XMFLOAT4 coll_norm = XMFLOAT4(n[i].x, n[i].y, n[i].z, 1.0f);
			XMVECTOR normal_rotation = XMVector4Transform(XMLoadFloat4(&coll_norm), XMLoadFloat4x4(&rota));
			XMStoreFloat4(&returnnor, normal_rotation);
			n[i].x = returnnor.x;
			n[i].y = returnnor.y;
			n[i].z = returnnor.z;
		}
	}
	aabb = aabbijk;
}
void ConstantBuffer::GetData(std::vector<Vertex> vertices)
{
	vertice_m = vertices;
	for (unsigned int i = 0; i < vertice_m.size(); i++) {
		vertice_f.push_back({0,0,0,0,0,0,0,0});
	}
}
void ConstantBuffer::BindToVSshader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext) {
	pContext->DSSetConstantBuffers(0, 1, pObjectConstantBuffer.GetAddressOf());
}
void ConstantBuffer::UpdateBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext,PerObjectData* obj) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	pContext->Map(pObjectConstantBuffer.Get(),0, D3D11_MAP_WRITE_DISCARD,0,&mappedResource);
	memcpy(mappedResource.pData, obj, sizeof(PerObjectData));
	pContext->Unmap(pObjectConstantBuffer.Get(), 0);
}
void ConstantBuffer::UpdateBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer) {
	CHECK_ERROR(pContext->Map(pBuffer.Get(), 0, D3D11_MAP_READ_WRITE, 0, &mappedResource));
	const size_t dataSize = mappedResource.DepthPitch;
	dataVector.resize(dataSize);
	memcpy(dataVector.data(), mappedResource.pData, dataSize);
	pContext->Unmap(pBuffer.Get(), 0);
}
void ConstantBuffer::BindToPSshader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext) {
	pContext->PSSetConstantBuffers(2, 1, pObjectConstantBuffer.GetAddressOf());
}

void ConstantBuffer::BindToPSshader(Microsoft::WRL::ComPtr<ID3D11Device> pDevice,Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext,float rgba[3]) {
	Microsoft::WRL::ComPtr<ID3D11Buffer> Color;
	rgbaS rgb(rgba[0],rgba[1],rgba[2]);
	D3D11_BUFFER_DESC CbuffDesc;
	CbuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CbuffDesc.ByteWidth = sizeof(PerObjectData);
	CbuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	CbuffDesc.MiscFlags = 0;
	CbuffDesc.StructureByteStride = 0;
	CbuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA Csbr;
	Csbr.pSysMem = &rgb;

	CHECK_ERROR(pDevice->CreateBuffer(&CbuffDesc, &Csbr, &Color));
	pContext->PSSetConstantBuffers(3, 1, Color.GetAddressOf());
}
