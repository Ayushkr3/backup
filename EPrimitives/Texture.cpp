#include "pch.h"
#include "Texture.h"
#include "Inc/DDSTextureLoader.h"
#include "Inc/WICTextureLoader.h"

using namespace Primitives;
using namespace Microsoft;

Texture::Texture(ID3D11Device*& pIDevice, ID3D11DeviceContext*& pIContext,std::string Name, short registerIndex):pIDevice(pIDevice),pIContext(pIContext),Name(Name),registerIndex(registerIndex){
	file = std::make_unique<PathToFile>();
}
void Texture::show() {
	ImGui::Text(Name.c_str()); ImGui::SameLine(); ImGui::Button(file->FileName.c_str());
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture")) {
			RefrencePassing* rf = (RefrencePassing*)payload->Data;
			PathToFile* pf = (PathToFile*)rf->ObjectPtr;
			std::unique_ptr<PathToFile> ptf(pf);
			//if file is dds return
			if (ptf->FileName.substr(ptf->FileName.size() - 4) == ".dds") {
				return;
			}
			CreateResource(ptf);
			ImGui::EndDragDropTarget();
		}
	}
}
void Texture::CreateResource(std::unique_ptr<PathToFile>& ptf) {
	if (pTexture != nullptr) {
		pTexture.Reset();
	}
	if (pSRV != nullptr) {
		pSRV.Reset();
	}
	file = std::move(ptf);
	DirectX::CreateWICTextureFromFile(pIDevice, std::wstring(file->Path.begin(), file->Path.end()).c_str(), (ID3D11Resource**)pTexture.GetAddressOf(), &pSRV);
	D3D11_TEXTURE2D_DESC desc = {};
	pTexture->GetDesc(&desc);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;

	pIDevice->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pSRV);
	Sampler.CreateResource(pIDevice,pIContext);
}
void Texture::Sampler::CreateResource(ID3D11Device*& pIDevice, ID3D11DeviceContext*& pIContext) {
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
	pIDevice->CreateSamplerState(&smpdesc, &pSmpler);
}
void Texture::Sampler::Bind(ID3D11DeviceContext*& pIContext, short Index) {
	pIContext->PSSetSamplers(Index, 1, pSmpler.GetAddressOf());
}
void Texture::Bind() {
	if (pTexture == nullptr) {
		CONSOLE_PRINT("Texture is Empty");
		return;
	}
	pIContext->PSSetShaderResources(registerIndex,1,pSRV.GetAddressOf());
	Sampler.Bind(pIContext, registerIndex);
}
void Texture3D::CreateResource(std::unique_ptr<PathToFile>& ptf) {
	if (pTexture != nullptr) {
		pTexture.Reset();
	}
	if (pSRV != nullptr) {
		pSRV.Reset();
	}
	file = std::move(ptf);
	DirectX::CreateDDSTextureFromFile(pIDevice, std::wstring(file->Path.begin(), file->Path.end()).c_str(), (ID3D11Resource**)pTexture.GetAddressOf(), &pSRV);
	D3D11_TEXTURE2D_DESC desc = {};
	pTexture->GetDesc(&desc);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;

	pIDevice->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pSRV);
	Sampler.CreateResource(pIDevice, pIContext);
}
void Texture3D::Bind() {
	pIContext->PSSetShaderResources(registerIndex, 1, pSRV.GetAddressOf());
	Sampler.Bind(pIContext, registerIndex);
}
std::string Texture::fileExtension() {
	return file->FileName.substr(file->FileName.size() - 4);
}
Texture3D::Texture3D(ID3D11Device*& pIDevice, ID3D11DeviceContext*& pIContext, std::string Name, short registerIndex):Texture(pIDevice, pIContext, Name, registerIndex) {

}
void Texture3D::show() {
	ImGui::Text((Name+"(3D)").c_str()); ImGui::SameLine(); ImGui::Button(file->FileName.c_str());
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture")) {
			RefrencePassing* rf = (RefrencePassing*)payload->Data;
			PathToFile* pf = (PathToFile*)rf->ObjectPtr;
			std::unique_ptr<PathToFile> ptf(pf);
			//if file is dds return
			if (ptf->FileName.substr(ptf->FileName.size() - 4) == ".jpeg") {
				return;
			}
			this->CreateResource(ptf);
			ImGui::EndDragDropTarget();
		}
	}
}
short Texture::GetIndex() {
	return registerIndex;
}
short Texture3D::GetIndex() {
	return Texture::registerIndex;
}
