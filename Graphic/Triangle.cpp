#include "Triangle.h"
Triangle::Triangle(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, std::vector<Vertex> vertice, std::vector<unsigned int>indi, short id, float rgba[3], short globalID, std::vector<NormalPerObject>nor) :
	pContext(pContext), pDevice(pDevice), vertices(
		vertice
	), index(indi
	), Objects(globalID, "Object"), id(id), color{ rgba[0],rgba[1],rgba[2] }, n{ nor }, last_color{ rgba[0],rgba[1],rgba[2] }, rn{ nor }
{
	Objects* obj = dynamic_cast<Objects*>(this);
	Trans = new TransformStruct(obj);
	Mat = new Primitives::Material;
	ObjProperties.push_back(Trans);
	ObjProperties.push_back(Mat);

	D3D11_BUFFER_DESC IndexBufferDesc;
	D3D11_BUFFER_DESC VertexBuffer;
	VertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER ;
	VertexBuffer.Usage = D3D11_USAGE_DYNAMIC;
	VertexBuffer.ByteWidth = (vertices.size() * sizeof(Vertex));
	VertexBuffer.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
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

	Transformation.Rotation = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixRotationRollPitchYaw(XMConvertToRadians(Trans->rotation[0]), XMConvertToRadians(Trans->rotation[1]), XMConvertToRadians(Trans->rotation[2])));
	Transformation.Translation = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixTranslation(Trans->position[0]+(Inheritence.InheritedTrans)->position[0], Trans->position[1] + (Inheritence.InheritedTrans)->position[1], Trans->position[2] + (Inheritence.InheritedTrans)->position[2]));
	Transformation.Scale = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixScaling(Trans->Scale[0], Trans->Scale[1], Trans->Scale[2]));
	//pCB = std::make_unique<ConstantBuffer>(&Transformation, pDevice,n);
	//pCB->GetData(vertices);
	//pCB->Transform(Trans,rn);
	Mat->CreateCBuffer(0, sizeof(PerObjectData), Primitives::DOMAIN_SHADER);
	pContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &strides, &offset);

	CHECK_ERROR(pDevice->CreateBuffer(&VertexBuffer, &sbr, &pVertexBuffer));
	CHECK_ERROR(pDevice->CreateBuffer(&IndexBufferDesc, &index_subr, &pIndexBuffer));
	///////////////////////////////////////////////////////////
	
	//pCB->BindToVSshader(pContext);
	Microsoft::WRL::ComPtr<ID3D11InputLayout>pIL;
	D3D11_INPUT_ELEMENT_DESC IL[] = {
		{ "Position",0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Texcoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12u,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0, DXGI_FORMAT_R32G32B32_FLOAT,0,20u,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	pDevice->CreateInputLayout(IL, std::size(IL),(Mat->vs.GetBlob())->GetBufferPointer(), (Mat->vs.GetBlob())->GetBufferSize(), &pIL);
	pContext->IASetInputLayout(pIL.Get());
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
}
void Triangle::Draw() {
	
	pContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &strides, &offset);
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	Mat->BindAllShaders();
	//Have to bind other wise it will not be rendered ,before every draw call
	//pCB->BindToVSshader(pContext);
	//pContext->DrawIndexed(index.size(), 0, 0);
	pContext->Draw(vertices.size(), 0);
}
void Triangle::UpdateBuffers() {
	Transformation.Rotation = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixRotationRollPitchYaw((Trans->rotation[0]), (Trans->rotation[1]), (Trans->rotation[2])));
	Transformation.Translation = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixTranslation(Trans->position[0] + (Inheritence.InheritedTrans)->position[0], Trans->position[1] + (Inheritence.InheritedTrans)->position[1], Trans->position[2] + (Inheritence.InheritedTrans)->position[2]));
	Transformation.Scale = ConstantBuffer::ConvertMatrixToFloat4x4(XMMatrixScaling(Trans->Scale[0], Trans->Scale[1], Trans->Scale[2]));
	//pCB->UpdateBuffer(pContext, &Transformation);
	Mat->ds.BindAssociatedBuffer((void*)&Transformation,0,pContext.Get());
}
void Triangle::inPlayMode() {
	Trans->Update();
	if (Trans->isMoving) {
		//pCB->Transform(Trans, rn);
	}
	for (auto& o : ObjProperties) {
		o->inPlayMode();
	}
}
void Triangle::ReCalculatePosition() {
	//pCB->Transform(Trans, rn);
}
void Triangle::InitializePlayMode()
{
	for (auto& o : ObjProperties) {
		o->InitPlayMode();
	}
	//pCB->Transform(Trans, rn);
}
void Triangle::Highlight(){
	color[0] = 0.0f;
	color[1] = 1.0f;
}
void Triangle::Restore()
{
	color[0] = last_color[0];
	color[1] = last_color[1];
}
std::vector<Vertex> Triangle::GetVertices() {
	return vertices;
}
bool Triangle::operator<(const Triangle& secondObj) const {
	return (id < secondObj.id) ? id:secondObj.id;
}
bool Triangle::operator==(const Triangle& secondObj) const {
	return (id == secondObj.id) ? true : false;
}
std::vector<ObjectProperties*>* Triangle::GetProperties() {
	return &ObjProperties;
}
Triangle::~Triangle() {
	delete Mat;
	delete Trans;
}