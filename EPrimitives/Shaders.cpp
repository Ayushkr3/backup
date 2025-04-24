#include "pch.h"
#include "Primitives.h"
#include <d3d11shader.h>
#pragma comment(lib,"D3DCompiler.lib")
using namespace Primitives;
const IID IID_ID3D11ShaderReflectionC = { 0x8d536ca1, 0x0cca, 0x4956, 0xa8, 0x37, 0x78, 0x69, 0x63, 0x75, 0x55, 0x84 }; //Custom GUID of Interface(direct X) (shader reflection)
VertexShader::VertexShader() {
	path = std::make_unique<PathToFile>();
}
void Primitives::VertexShader::CreateShader(ID3D11Device*& pIDevice)
{
	if (pVS != nullptr)
		pVS.Reset();
	if (pVSBlob != nullptr)
		pVSBlob.Reset();
	D3DReadFileToBlob(std::wstring(path->Path.begin(), path->Path.end()).c_str(),&pVSBlob);
	pIDevice->CreateVertexShader(pVSBlob->GetBufferPointer(),pVSBlob->GetBufferSize(),nullptr,&pVS);
	D3DReflect(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), IID_ID3D11ShaderReflectionC,&pSReflector);
}
void VertexShader::AssignShader(ID3D11VertexShader* VertexShader, Microsoft::WRL::ComPtr<ID3D10Blob>& ppBlob) {
	pVS = VertexShader;
	pVSBlob = ppBlob;
}
void Primitives::VertexShader::BindShaderToPipeLine(ID3D11DeviceContext*& pIContext)
{
	(pIContext)->VSSetShader(pVS.Get(),nullptr,0);
}
Microsoft::WRL::ComPtr<ID3D10Blob> Primitives::VertexShader::GetBlob()
{
	return pVSBlob;
}
void Primitives::VertexShader::ReleaseRef()
{
	pVS.Reset();
	pVSBlob.Reset();
	pSReflector.Reset();
}
Primitives::PixelShader::PixelShader()
{
	path = std::make_unique<PathToFile>();
}
void Primitives::PixelShader::CreateShader(ID3D11Device *& pIDevice, ID3D11DeviceContext*& pIContext)
{
	if (pPS != nullptr)
		pPS.Reset();
	if (pPSBlob != nullptr)
		pPSBlob.Reset();
	Textures.clear();
	D3DReadFileToBlob(std::wstring(path->Path.begin(), path->Path.end()).c_str(), &pPSBlob);
	D3DReflect(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), IID_ID3D11ShaderReflectionC, &pSReflector);
	D3D11_SHADER_DESC Sd;
	pSReflector->GetDesc(&Sd);
	for (UINT i = 0; i < Sd.BoundResources; i++) {
		D3D11_SHADER_INPUT_BIND_DESC IBD;
		pSReflector->GetResourceBindingDesc(i,&IBD);
		if (IBD.Type == D3D_SIT_TEXTURE) {
			if(IBD.Dimension == D3D_SRV_DIMENSION_TEXTURE2D)
				Textures.push_back(std::make_unique<Texture>(pIDevice, pIContext, (std::string)IBD.Name, IBD.BindPoint));
			if(IBD.Dimension == D3D_SRV_DIMENSION_TEXTURECUBE)
				Textures.push_back(std::make_unique<Texture3D>(pIDevice, pIContext, (std::string)IBD.Name, IBD.BindPoint));
		}
	}
	pIDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPS);
}
void Primitives::PixelShader::CreateShader(std::unique_ptr<PathToFile>& ptf, ID3D11Device*& pIDevice, ID3D11DeviceContext*& pIContext)
{
	path = std::move(ptf);
	Textures.clear();
	D3DReadFileToBlob(std::wstring(path->Path.begin(), path->Path.end()).c_str(), &pPSBlob);
	D3DReflect(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), IID_ID3D11ShaderReflectionC, &pSReflector);
	D3D11_SHADER_DESC Sd;
	pSReflector->GetDesc(&Sd);
	for (UINT i = 0; i < Sd.BoundResources; i++) {
		D3D11_SHADER_INPUT_BIND_DESC IBD;
		pSReflector->GetResourceBindingDesc(i, &IBD);
		if (IBD.Type == D3D_SIT_TEXTURE) {
			if (IBD.Dimension == D3D_SRV_DIMENSION_TEXTURE2D)
				Textures.push_back(std::make_unique<Texture>(pIDevice, pIContext, (std::string)IBD.Name, IBD.BindPoint));
			if (IBD.Dimension == D3D_SRV_DIMENSION_TEXTURECUBE)
				Textures.push_back(std::make_unique<Texture3D>(pIDevice, pIContext, (std::string)IBD.Name, IBD.BindPoint));
		}
	}
	pIDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPS);
}
void PixelShader::SetAssociatedTexture(short index,std::unique_ptr<PathToFile>& ptf) {
	for (std::unique_ptr<Texture>& Tex : Textures) {
		if (Tex->GetIndex()==index) {
			Tex->CreateResource(ptf);
			return;
		}
	}
	CONSOLE_PRINT("Cannot find texture index");
}
void PixelShader::AssignShader(ID3D11PixelShader* PixelShader, Microsoft::WRL::ComPtr<ID3D10Blob>& ppBlob)
{
	pPS = PixelShader;
	pPSBlob = ppBlob;
}
void Primitives::PixelShader::BindShaderToPipeLine(ID3D11DeviceContext*& pIContext)
{
	(pIContext)->PSSetShader(pPS.Get(), nullptr, 0);
	for (auto& Tex : Textures) {
		Tex->Bind();
	}
}
 void Primitives::PixelShader::ReleaseRef()
{
	 pPS.Reset();
	 pPSBlob.Reset();
	 pSReflector.Reset();
}
 GeometryShader::GeometryShader() {
	 path = std::make_unique<PathToFile>();
	 //path->Path = "D:\\program\\vs\\graphic\\x64\\Debug\\GeometryShader.cso"; //remove this
 }
 void GeometryShader::CreateShader(ID3D11Device*& pIDevice) {
	 buffersAssociated.clear();
	 if (pGS != nullptr)
		 pGS.Reset();
	 if (pGSBlob != nullptr)
		 pGSBlob.Reset();
	 D3DReadFileToBlob(std::wstring(path->Path.begin(), path->Path.end()).c_str(), &pGSBlob);
	 pIDevice->CreateGeometryShader(pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), nullptr, &pGS);
	 D3DReflect(pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), IID_ID3D11ShaderReflectionC, &pSReflector);
	 D3D11_SHADER_DESC desc;
	 pSReflector->GetDesc(&desc);
	 for (UINT i = 0; i < desc.BoundResources; i++) {
		 D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		 pSReflector->GetResourceBindingDesc(i, &bindDesc);
		 if(bindDesc.Type == D3D_SIT_CBUFFER)
			buffersAssociated.emplace((short)bindDesc.BindPoint, std::make_unique<ConstantBufferData>());
		//buffersAssociated.push_back(std::make_unique<ConstantBufferData>());
	 }
	 pIDevice->CreateGeometryShader(pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), nullptr, &pGS);
 }
 //Called every frame to update buffer
 void GeometryShader::BindAssociatedBuffer(ConstantBufferData*& Data, short index) {
	 buffersAssociated[index] = std::unique_ptr<ConstantBufferData>(Data);
 }
void GeometryShader::AssignShader(ID3D11GeometryShader* GeometryShader, Microsoft::WRL::ComPtr<ID3D10Blob> ppBlob)
{
	pGS = GeometryShader;
	pGSBlob = ppBlob;
	if (pGSBlob == nullptr) return;
	D3DReflect(pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), IID_ID3D11ShaderReflectionC, &pSReflector);
	D3D11_SHADER_DESC desc;
	pSReflector->GetDesc(&desc);
	for (UINT i = 0; i < desc.BoundResources; i++) {
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		pSReflector->GetResourceBindingDesc(i, &bindDesc);
		if (bindDesc.Type == D3D_SIT_CBUFFER){}//Dont want projectionXmatrix to take up space
			//buffersAssociated.emplace((short)bindDesc.BindPoint, std::make_unique<ConstantBufferData>());
		//buffersAssociated.push_back(std::make_unique<ConstantBufferData>());
	}
}
void Primitives::GeometryShader::BindShaderToPipeLine(ID3D11DeviceContext*& pIContext)
{
	(pIContext)->GSSetShader(pGS.Get(), nullptr, 0);
	/*for (auto& it : buffersAssociated) {
		it.second->bind(it.first);
	}*/
}
 void Primitives::GeometryShader::ReleaseRef()
{
	 pGS.Reset();
	 pGSBlob.Reset();
	 pSReflector.Reset();
}
void HullShader::AssignShader(ID3D11HullShader* HullShader, Microsoft::WRL::ComPtr<ID3D10Blob>& ppBlob)
{
	pHS = HullShader;
	pHSBlob = ppBlob;
}
void Primitives::HullShader::BindShaderToPipeLine(ID3D11DeviceContext*& pIContext)
{
	(pIContext)->HSSetShader(pHS.Get(), nullptr, 0);
}
 void Primitives::HullShader::ReleaseRef()
{
	 pHS.Reset();
	 pHSBlob.Reset();
}
void DomainShader::AssignShader(ID3D11DomainShader* DomainShader, Microsoft::WRL::ComPtr<ID3D10Blob>& ppBlob)
{
	pDS = DomainShader;
	pDSBlob = ppBlob;
	D3DReflect(pDSBlob->GetBufferPointer(), pDSBlob->GetBufferSize(), IID_ID3D11ShaderReflectionC, &pSReflector);
	D3D11_SHADER_DESC desc;
	pSReflector->GetDesc(&desc);
	for (UINT i = 0; i < desc.BoundResources; i++) {
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		pSReflector->GetResourceBindingDesc(i, &bindDesc);
		if (bindDesc.Type == D3D_SIT_CBUFFER) {
			buffersAssociated.emplace((short)bindDesc.BindPoint, std::make_unique<ShaderConstantBuffer>());
		}
	}
}
DomainShader::DomainShader() {
	path = std::make_unique <PathToFile>();
}
void DomainShader::CreateShader(ID3D11Device*& pIDevice){
	buffersAssociated.clear();
	if (pDS != nullptr)
		pDS.Reset();
	if (pDSBlob != nullptr)
		pDSBlob.Reset();
	D3DReadFileToBlob(std::wstring(path->Path.begin(), path->Path.end()).c_str(), &pDSBlob);
	pIDevice->CreateDomainShader(pDSBlob->GetBufferPointer(), pDSBlob->GetBufferSize(), nullptr, &pDS);
	D3DReflect(pDSBlob->GetBufferPointer(), pDSBlob->GetBufferSize(), IID_ID3D11ShaderReflectionC, &pSReflector);
	D3D11_SHADER_DESC desc;
	pSReflector->GetDesc(&desc);
	for (UINT i = 0; i < desc.BoundResources; i++) {
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		pSReflector->GetResourceBindingDesc(i, &bindDesc);
		if (bindDesc.Type == D3D_SIT_CBUFFER)
			buffersAssociated.emplace((short)bindDesc.BindPoint, std::make_unique<ShaderConstantBuffer>());
	}
	pIDevice->CreateDomainShader(pDSBlob->GetBufferPointer(), pDSBlob->GetBufferSize(), nullptr, &pDS);
}
void Primitives::DomainShader::CreateShader(std::unique_ptr<PathToFile>& ptf, ID3D11Device*& pIDevice, ID3D11DeviceContext*& pIContext)
{
	buffersAssociated.clear();
	path = std::move(ptf);
	if (pDS != nullptr)
		pDS.Reset();
	if (pDSBlob != nullptr)
		pDSBlob.Reset();
	D3DReadFileToBlob(std::wstring(path->Path.begin(), path->Path.end()).c_str(), &pDSBlob);
	pIDevice->CreateDomainShader(pDSBlob->GetBufferPointer(), pDSBlob->GetBufferSize(), nullptr, &pDS);
	D3DReflect(pDSBlob->GetBufferPointer(), pDSBlob->GetBufferSize(), IID_ID3D11ShaderReflectionC, &pSReflector);
	D3D11_SHADER_DESC desc;
	pSReflector->GetDesc(&desc);
	for (UINT i = 0; i < desc.BoundResources; i++) {
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		pSReflector->GetResourceBindingDesc(i, &bindDesc);
		if (bindDesc.Type == D3D_SIT_CBUFFER)
			buffersAssociated.emplace((short)bindDesc.BindPoint, std::make_unique<ShaderConstantBuffer>());
	}
	pIDevice->CreateDomainShader(pDSBlob->GetBufferPointer(), pDSBlob->GetBufferSize(), nullptr, &pDS);
}
void DomainShader::BindAssociatedBuffer(void* Data, short index,ID3D11DeviceContext *pIC) {
	if (buffersAssociated[index] == nullptr) {
		CONSOLE_PRINT("buffer not found");
		return;
	}
	ConstantBuffer::UpdateSubResource(Data,buffersAssociated[index]->size,buffersAssociated[index]->CBuffer,pIC);
}
std::unique_ptr<ShaderConstantBuffer>& DomainShader::GetBuffer(short index) {
	return buffersAssociated[index];
}
void Primitives::DomainShader::BindShaderToPipeLine(ID3D11DeviceContext*& pIContext)
{
	(pIContext)->DSSetShader(pDS.Get(), nullptr, 0);
	for (auto& it : buffersAssociated) {
		pIContext->DSSetConstantBuffers(it.first, 1, it.second->CBuffer.GetAddressOf());
	}
}

 void Primitives::DomainShader::ReleaseRef()
{
	 pDS.Reset();
	 pDSBlob.Reset();
}
 HullShader::HullShader() {
	 path = std::make_unique<PathToFile>();
	 //path->Path = "D:\\program\\vs\\graphic\\x64\\Debug\\GeometryShader.cso"; //remove this
 }