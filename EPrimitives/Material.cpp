#include "pch.h"
#include "Primitives.h"
#include "WIC.h"
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3DCompiler.lib")
using namespace Primitives;

const IID IID_ID3D11ShaderReflectionC = { 0x8d536ca1, 0x0cca, 0x4956, 0xa8, 0x37, 0x78, 0x69, 0x63, 0x75, 0x55, 0x84 };

Microsoft::WRL::ComPtr<ID3D10Blob> Material::pDVSBlob; // Default blob
Microsoft::WRL::ComPtr<ID3D10Blob> Material::pDPSBlob; // Default blob
Microsoft::WRL::ComPtr<ID3D10Blob> Material::pDHSBlob; // Default blob
Microsoft::WRL::ComPtr<ID3D10Blob> Material::pDDSBlob; // Default blob
Microsoft::WRL::ComPtr<ID3D10Blob> Material::pDGSBlob; // Default blob
ID3D11VertexShader* Material::pDVS; //Default Shader
ID3D11PixelShader* Material::pDPS; //Default Shader
ID3D11HullShader* Material::pDHS; //Default Shader
ID3D11DomainShader* Material::pDDS; //Default Shader
ID3D11GeometryShader* Material::pDGS; //Default Shader

ID3D11Device* Material::pIDevice;   //Internal Device
ID3D11DeviceContext* Material::pIContext; //Internal Context

ImGuiContext* Primitives::Material::privateCtx = nullptr;

void Primitives::Material::InitalizeD3D11Factory(Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pContext)
{
	pIDevice = pDevice.Get();
	pIContext = pContext.Get();
	WIC::SetImFac();

	D3DReadFileToBlob(L"G:\\shard\\x64\\Debug\\VertexShader.cso", &pDVSBlob);
	pIDevice->CreateVertexShader(pDVSBlob->GetBufferPointer(), pDVSBlob->GetBufferSize(), nullptr, &pDVS);

	D3DReadFileToBlob(L"G:\\shard\\x64\\Debug\\PixelShader.cso", &pDPSBlob);
	pIDevice->CreatePixelShader(pDPSBlob->GetBufferPointer(), pDPSBlob->GetBufferSize(), nullptr, &pDPS);

	D3DReadFileToBlob(L"G:\\shard\\x64\\Debug\\GeometryShader.cso", &pDGSBlob);
	pIDevice->CreateGeometryShader(pDGSBlob->GetBufferPointer(), pDGSBlob->GetBufferSize(), nullptr, &pDGS);

	D3DReadFileToBlob(L"G:\\shard\\x64\\Debug\\HullShader.cso", &pDHSBlob);
	pIDevice->CreateHullShader(pDHSBlob->GetBufferPointer(), pDHSBlob->GetBufferSize(), nullptr, &pDHS);

	D3DReadFileToBlob(L"G:\\shard\\x64\\Debug\\DomainShader.cso", &pDDSBlob);
	pIDevice->CreateDomainShader(pDDSBlob->GetBufferPointer(), pDDSBlob->GetBufferSize(), nullptr, &pDDS);
}
void Primitives::Material::DeInitalizeD3D11Factory()
{
	//pDVSBlob->Release();
	//pDVS->Release();
	//pDPSBlob->Release();
	//pDPS->Release();
	//pDGSBlob->Release();
	//pDGS->Release();
	//pDHSBlob->Release();
	//pDHS->Release();
	//pDDSBlob->Release();
	//pDDS->Release();

	//I think Manual Release is not needed because 
	//DLL_PROCESS_DETACH is called when dll closes and delete everything
}
void Material::show() {
	ImGui::SetCurrentContext(privateCtx);
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_CollapsingHeader)) {
		ImGui::Button("Material P");
		if (ImGui::BeginDragDropSource()) {
			std::unique_ptr<RefrencePassing> ref = std::make_unique<RefrencePassing>((void*)this, typeid(*this));
			//RefrencePassing* ref = new RefrencePassing(this, typeid(*this));
			ImGui::SetDragDropPayload("Material", ref.get(), sizeof(RefrencePassing));
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Shader")) {
				UpdateDependency(payload->Data);
				ImGui::EndDragDropTarget();
			}
		}
		if(ImGui::TreeNode("VertexShader")) {
			ImGui::Text("VertexShader"); ImGui::SameLine(); ImGui::Button(vs.path.get()->FileName.c_str());
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("PixelShader")) {
			ImGui::Text("PixelShader"); ImGui::SameLine(); ImGui::Button(ps.path.get()->FileName.c_str());
			for (short i = 0; i < ps.Textures.size(); i++) {
				ps.Textures[i]->show();
			}
			ImGui::DragFloat4("Color", ps.PixelShaderData.rgba, 0.01f, 0.0f, 1.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("DomainShader")) {
			ImGui::Text("DomainShader"); ImGui::SameLine(); ImGui::Button(ps.path.get()->FileName.c_str());
			ImGui::TreePop();
		}
	}
}
Material::Material(Objects* obj):ObjectProperties(obj){
	vs.AssignShader(pDVS, pDVSBlob);

	ps.AssignShader(pDPS,pDPSBlob);

	hs.AssignShader(pDHS,pDHSBlob);

	ds.AssignShader(pDDS,pDDSBlob);

	gs.AssignShader(pDGS,pDGSBlob);

	pCB = std::make_unique<ConstantBuffer>(&ps,pIDevice);
}
void Material::CreateCBuffer(short index, size_t size, SHADER_CONSTANT_BUFFER_TYPE shadertype) {
	
	D3D11_BUFFER_DESC CbuffDesc;
	CbuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	if (size < 16) {
		CbuffDesc.ByteWidth = (UINT)16;
	}
	else {
		CbuffDesc.ByteWidth = (UINT)size;
	}
	CbuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	CbuffDesc.MiscFlags = 0;
	CbuffDesc.StructureByteStride = 0;
	CbuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA Csbr;
	Csbr.pSysMem = 0;
	if (shadertype == DOMAIN_SHADER) {
		ds.GetBuffer(index)->size = size;
		pIDevice->CreateBuffer(&CbuffDesc, nullptr, &ds.GetBuffer(index)->CBuffer);
	}
}
void Material::BindAllShaders()
{
	vs.BindShaderToPipeLine(pIContext);
	hs.BindShaderToPipeLine(pIContext);
	ds.BindShaderToPipeLine(pIContext);
	gs.BindShaderToPipeLine(pIContext);
	ps.BindShaderToPipeLine(pIContext);
	pCB->UpdatePS((pIContext));
}
void Material::UpdateDependency(const void* ptr) {
	RefrencePassing* rf = (RefrencePassing*)ptr;
	PathToFile* pf = (PathToFile*)rf->ObjectPtr;
	std::unique_ptr<PathToFile> ptf(pf);
	std::string s = ptf->FileName.substr(0,2);
	if (s == "PS") {
		ps.path = std::move(ptf);
		ps.ReleaseRef();
		ps.CreateShader(pIDevice,pIContext);
	}
	else if (s == "VS") {
		vs.path = std::move(ptf);
		vs.ReleaseRef();
		vs.CreateShader(pIDevice);
	}
	else if (s == "DS") {
		ds.path = std::move(ptf);
		ds.ReleaseRef();
		ds.CreateShader(pIDevice);
	}
}
void Material::SetImguiCtx(ImGuiContext* Ctx) {
	privateCtx = Ctx;
}
ObjectProperties* Material::GetPropertyRef() {
	return this;
}
const std::type_info& Material::GetPropertyType() {
	return typeid(Material);
}
std::string Material::Serialize() {
	std::string mat;
	mat += "0:vs:" + vs.path->Path+"\n";
	mat += "1:vs:" + vs.path->FileName + "\n";
	mat += "2:ps:" + ps.path->Path + "\n";
	mat += "3:ps:" + ps.path->FileName + "\n";
	mat += "4:hs:" + hs.path->Path + "\n";
	mat += "5:hs:" + hs.path->FileName + "\n";
	mat += "6:ds:" + ds.path->Path + "\n";
	mat += "7:ds:" + ds.path->FileName + "\n";
	mat += "8:gs:" + gs.path->Path + "\n";
	mat += "9:gs:" + gs.path->FileName + "\n";
	return Serialization::SerializeProperty(*this, mat);
}
void Material::DeSerialize(std::string block) {
	std::istringstream iss(block);
	std::string line;
	std::getline(iss, line);
	while (std::getline(iss, line)) {
		size_t off = line.find(":");
		std::string type = line.substr(0, off);
		if (line._Starts_with("{/")) break;
		switch (std::stoi(type))
		{
		case 0:
			off = line.find(":",off+1);
			vs.path->Path = line.substr(off + 1, line.size() - 2);
			break;
		case 1:
			off = line.find(":",off+1);
			vs.path->FileName = line.substr(off + 1, line.size() - 2);
			break;
		case 2:
			off = line.find(":",off+1);
			ps.path->Path = line.substr(off + 1, line.size() - 2);
			break;
		case 3:
			off = line.find(":",off+1);
			ps.path->FileName = line.substr(off + 1, line.size() - 2);
			break;
		case 4:
			off = line.find(":",off+1);
			hs.path->Path = line.substr(off + 1, line.size() - 2);
			break;
		case 5:
			off = line.find(":",off+1);
			hs.path->FileName = line.substr(off + 1, line.size() - 2);
			break;
		case 6:
			off = line.find(":",off+1);
			ds.path->Path = line.substr(off + 1, line.size() - 2);
			break;
		case 7:
			off = line.find(":",off+1);
			ds.path->FileName = line.substr(off + 1, line.size() - 2);
			break;
		case 8:
			off = line.find(":",off+1);
			gs.path->Path = line.substr(off + 1, line.size() - 2);
			break;
		case 9:
			off = line.find(":",off+1);
			gs.path->FileName = line.substr(off + 1, line.size() - 2);
			break;
		}
	}
	bool found = false;
	for (auto it = associatedObj->GetProperties()->begin(); it != associatedObj->GetProperties()->end();it++) {
		if ((*it)->GetPropertyType() == typeid(Material)) {
			Material* m = dynamic_cast<Material*>(*it);
			delete m;
			*it = this;
			found = true;
			break;
		}
		else {
			found = false;
		}
	}
	if (!found) associatedObj->GetProperties()->push_back(this);
}
void Primitives::Material::RegisterFactory(std::multimap<std::string, std::function<ObjectProperties*(Objects*)>>& GlobalPropertiesPoolL)
{
	std::function<ObjectProperties*(Objects*)> f = CreateMaterial;
	ObjectProperties::PushToObjectPropertyPool("Material", f, GlobalPropertiesPoolL);
}
ObjectProperties* CreateMaterial(Objects* obj) {
	return new Material(obj);
}