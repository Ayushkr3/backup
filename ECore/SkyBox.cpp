#include "pch.h"
#include "SkyBox.h"

SkyBox::SkyBox(ID3D11Device*& pDevice, ID3D11DeviceContext*& pContext, std::vector<Vertex> vertice, std::vector<unsigned int>indi, short id, float rgba[3], short globalID, std::vector<NormalPerObject>nor, Camera* cam) :Prefab(pDevice, pContext, vertice, indi, id, rgba, globalID), cam(cam) {
	D3D11_RASTERIZER_DESC rasDesc;
	rasDesc.CullMode = D3D11_CULL_NONE;
	rasDesc.FillMode = D3D11_FILL_SOLID;
	rasDesc.FrontCounterClockwise = false;
	rasDesc.DepthClipEnable = false;
	rasDesc.DepthBias = 0;
	rasDesc.SlopeScaledDepthBias = 0;
	rasDesc.DepthBiasClamp = 0;
	rasDesc.ScissorEnable = FALSE;
	rasDesc.MultisampleEnable = false;  //should be off
	rasDesc.AntialiasedLineEnable = true;
	auto hr = pDevice->CreateRasterizerState(&rasDesc, &rasterState);

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace = dsDesc.FrontFace;

	hr = pDevice->CreateDepthStencilState(&dsDesc, &(this->DSS));
	Primitives::Material* Mat = dynamic_cast<Primitives::Material*>(ObjProperties[1]);
	std::unique_ptr<PathToFile> ptf = std::make_unique<PathToFile>();
	ptf->Path = "D:\\program\\Eng\\Shaders\\PSSkyBox.cso";
	ptf->FileName = "PSSkyBox.cso";
	Mat->ps.CreateShader(ptf, pDevice, pContext);
	ptf.reset();
	ptf = std::make_unique<PathToFile>();
	ptf->Path = "D:\\program\\Eng\\Texture\\skybox.dds";
	ptf->FileName = "skybox.dds";
	Mat->ps.SetAssociatedTexture(1, ptf);
	ptf.reset();
	ptf = std::make_unique<PathToFile>();
	ptf->Path = "D:\\program\\Eng\\Shaders\\DSSkyBox.cso";
	ptf->FileName = "DSSkyBox.cso";
	Mat->ds.CreateShader(ptf, pDevice, pContext);
	Mat->CreateCBuffer(1, sizeof(DSSkyBox), Primitives::DOMAIN_SHADER);
	Mat->CreateCBuffer(0, sizeof(CB::PerObjectData), Primitives::DOMAIN_SHADER);
	Mat->gs.AssignShader(nullptr, nullptr);
}

void SkyBox::Draw() {
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pPreviousState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pPreviousDSS;
	pContext->RSGetState(&pPreviousState); //Get last state
	pContext->RSSetState(rasterState.Get()); // Set it to counter ClockWise
	pContext->OMGetDepthStencilState(&pPreviousDSS, 0);
	//-------------------------------------------------------------------//
	//TODO:Remove this when Triangle start calling DrawIndexed instead of Draw and Replace this part with Triangle::Draw()
	//--------------------------------------------------------------------//
	Primitives::Material* Mat = dynamic_cast<Primitives::Material*>(ObjProperties[1]);
	pContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &strides, &offset);
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	DSsb.view = cam->viewmatrix;
	DSsb.projection = cam->projection;
	Mat->ds.BindAssociatedBuffer((void*)&DSsb, 1, pContext.Get());
	Mat->ds.BindAssociatedBuffer((void*)&Transformation, 0, pContext.Get());
	Mat->BindAllShaders();
	pContext->OMSetDepthStencilState(this->DSS.Get(), 0);
	pContext->DrawIndexed((UINT)index.size(), 0, 0);
	//-------------------------------------------------------------------//
	pContext->RSSetState(pPreviousState.Get()); // Revert back to last to state
	pContext->OMSetDepthStencilState(pPreviousDSS.Get(), 0);
}
