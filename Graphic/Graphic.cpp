#include "Graphic.h"
#include <d3dcompiler.h>
//////////////////////
#include "Mouse.h"
#include "Keyboard.h"
/////////////////////
#pragma comment(lib,"D3DCompiler.lib")
#pragma comment(lib,"d3d11.lib")

Graphic::Graphic(HWND hwnd){
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.Height = 0;
	scd.BufferDesc.Width = 0;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.Flags = 0;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = true;
	//TODO: Create Class for device and context for global availiblity
	CHECK_ERROR(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION, &scd, &pSwap, &pDevice, nullptr,&pContext));
	WIC::SetImFac();
	ID3D11Resource* pBackBuffer = nullptr;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
	pDevice->CreateRenderTargetView(pBackBuffer, nullptr, pTarget.GetAddressOf());
	pBackBuffer->Release();
	pSc = std::make_unique<Scene>(pDevice,pContext);

	D3D11_VIEWPORT vp;
	vp.Width = 1024;
	vp.Height = 576;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> WireFrame;
	D3D11_RASTERIZER_DESC rasDesc;
	rasDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasDesc.CullMode = D3D11_CULL_NONE;
	rasDesc.FrontCounterClockwise = true;;
	rasDesc.DepthClipEnable = TRUE;
	rasDesc.DepthBias = 0;
	rasDesc.DepthBiasClamp = 0.0f;
	rasDesc.ScissorEnable = FALSE;
	rasDesc.MultisampleEnable = FALSE;
	rasDesc.AntialiasedLineEnable = FALSE;
	CHECK_ERROR(pDevice->CreateRasterizerState(&rasDesc, &WireFrame));

	pContext->RSSetState(WireFrame.Get());

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	

	CHECK_ERROR(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));
	pContext->OMSetDepthStencilState(pDSState.Get(),1u);


	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
#ifdef ImGUI_ENABLED
	descDepth.Width = 1022;
	descDepth.Height = 574;
#else
	descDepth.Width = 784;
	descDepth.Height = 561;
#endif // ImGUI_ENABLED
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	CHECK_ERROR(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));


	D3D11_DEPTH_STENCIL_VIEW_DESC stencil_view_desc;
	stencil_view_desc.Flags = 0;
	stencil_view_desc.Format = DXGI_FORMAT_D32_FLOAT;
	stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	stencil_view_desc.Texture2D.MipSlice = 0u;

	CHECK_ERROR(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &stencil_view_desc, &pDs));
	pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(),pDs.Get());
#ifdef ImGUI_ENABLED
	//ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
#endif
}
Graphic::~Graphic() {
#ifdef ImGUI_ENABLED
	/*ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();*/
#endif
}
void Graphic::EndFrame() {
	pSwap->Present(1u,0);
}
void Graphic::ClearBuffer(float rgba[4]) {
	pContext->ClearRenderTargetView(pTarget.Get(),rgba);
	pContext->ClearDepthStencilView(pDs.Get(), D3D11_CLEAR_DEPTH , 1.0f, 0);
}
void Graphic::TestFrames() {
#ifdef ImGUI_ENABLED
//	UpdateImGui();
#endif // ImGUI_ENABLED
	pSc->Render();

}
#ifdef ImGUI_ENABLED
//void Graphic::UpdateImGui() {
//	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove);
//	for (size_t i = 0; i < pSc->Triangles.size(); i++) {
//		if (ImGui::Selectable(("Test " + std::to_string(i)).c_str())) {
//			//showvalue = !showvalue;
//			showvalue = true;
//			t = i;
//		};
//	}
//	ImGui::End();
//	ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove);
//	if (ImGui::CollapsingHeader("Object")) {
//		if (showvalue) {
//			/*pSc->Triangles[t].UpdateBuffers();*/
//			ImGui::DragFloat3("Rotation", pSc->Triangles[t].rotation, 0.2f);
//			ImGui::DragFloat3("Position", pSc->Triangles[t].position, 0.2f);
//			//ImGui::Text(("Coll " + (std::to_string((pSc->check_collision)))).c_str());
//			ImGui::DragFloat3("Scale", pSc->Triangles[t].Scale, 0.2f);
//			ImGui::Text(("Moving" + (std::to_string((pSc->CContoller->GlobalCollide)))).c_str());
//		}
//	}
//	if (ImGui::CollapsingHeader("Camera")==false) {
//		ImGui::DragFloat2("Camera Rotation", pSc->cam.rotation, 0.1f);
//		ImGui::DragFloat3("Camera Position", pSc->cam.postion, 0.1f);
//	}
//	ImGui::End();
//	
//	ImGui::EndFrame();
//	
//}
#endif
Scene* Graphic::GetCurrentScene() {
	return pSc.get();
}