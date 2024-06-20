#include "ECoreUI.h"
#define CHECK_ERROR(hr) if(FAILED(hr)) throw error::error(hr,__LINE__)
Microsoft::WRL::ComPtr<ID3D11Device> UIElements::pUIDevice = nullptr;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> UIElements::pUIContext= nullptr;
Microsoft::WRL::ComPtr<IDXGISwapChain>      UIElements::pUISwapChain= nullptr;
Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  UIElements::pUIRenderTarget= nullptr;
DXGI_SWAP_CHAIN_DESC UIElements::UIscd;
UIWindows::UIWindows(std::string className, HWND Phwnd, HINSTANCE hint,short x ,short y,short w,short b,int windowsN) {
	cHwnd = CreateWindowEx(
		0,                          // Optional window styles.
		className.c_str(),                     // Window class
		std::to_string(windowsN).c_str(),				  // Window text
		WS_CHILD | WS_VISIBLE|WS_BORDER,     // Window style

							   // Size and position
		x, y, w,b,

		Phwnd,				// Parent window    
		(HMENU)windowsN,			   // Menu
		hint,			  // Instance handle
		NULL			 // Additional application data
	);
}
UIElements::UIElements(std::string className, HWND Phwnd, HINSTANCE hint, short x, short y, short w, short b, int windowsN) {
	if (pUIDevice == nullptr) {
		ImGui::CreateContext();
		cHwnd = CreateWindowEx(
			0,                          // Optional window styles.
			className.c_str(),                     // Window class
			std::to_string(windowsN).c_str(),				  // Window text
			WS_CHILD |WS_VISIBLE |WS_CLIPSIBLINGS | WS_BORDER,     // Window style

								   // Size and position
			x, y, w, b,

			Phwnd,				// Parent window    
			(HMENU)windowsN,			   // Menu
			hint,			  // Instance handle
			NULL			 // Additional application data
		);
		UIscd.BufferCount = 1;
		UIscd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		UIscd.BufferDesc.Height = 0;
		UIscd.BufferDesc.Width = 0;
		UIscd.BufferDesc.RefreshRate.Numerator = 0;
		UIscd.BufferDesc.RefreshRate.Denominator = 0;
		UIscd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		UIscd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		UIscd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		UIscd.Flags = 0;
		UIscd.OutputWindow = cHwnd;
		UIscd.SampleDesc.Count = 1;
		UIscd.SampleDesc.Quality = 0;
		UIscd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		UIscd.Windowed = true;
		CHECK_ERROR(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION, &UIscd, &pUISwapChain, &pUIDevice, nullptr, &pUIContext));
		ID3D11Resource* pBackBuffer = nullptr;
		CHECK_ERROR(pUISwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)));
		CHECK_ERROR(pUIDevice->CreateRenderTargetView(pBackBuffer, nullptr, pUIRenderTarget.GetAddressOf()));
		pBackBuffer->Release();
		ImGui_ImplWin32_Init(cHwnd);
		ImGui_ImplDX11_Init(pUIDevice.Get(), pUIContext.Get());
	}
}
void UIElements::UpdateUI() {
	float rgba[4] = { 0.0f,0.0f,0.0f,1.0f };
	pUIContext->OMSetRenderTargets(1, pUIRenderTarget.GetAddressOf(), nullptr);
	pUIContext->ClearRenderTargetView(pUIRenderTarget.Get(), rgba);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
UIElements::~UIElements() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
void UIElements::Swap() {
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	pUISwapChain->Present(1u, 0);
}
SceneManager::SceneManager(int posX, int posY, int widthX, int widthY,Scene* InitalScene):posX(posX),posY(posY),widthX(widthX),widthY(widthY),currentScene(InitalScene)
{
}
void SceneManager::SetSizenWidth() {
	ImGui::Begin("Scene Objects");
	ImGui::SetWindowSize(ImVec2((float)widthX, (float)widthY));
	ImGui::SetWindowPos(ImVec2((float)posX, (float)posY));
	ImGui::End();
}
void SceneManager::Content() {
	static int selected = -1;
	ImGui::Begin("Scene Objects");
	for (auto& Tri : currentScene->Triangles) {
		if (ImGui::Selectable(("Test" + std::to_string(Tri.id)).c_str(), selected == Tri.id)) {
			selected = Tri.id;
			PropertiesWindow::Obj = &Tri;
		}
	}
	ImGui::End();
}
Triangle* PropertiesWindow::Obj = nullptr;
PropertiesWindow::PropertiesWindow(int posX, int posY, int widthX, int widthY) :posX(posX), posY(posY), widthX(widthX), widthY(widthY)
{
}
void PropertiesWindow::SetSizenWidth() {
	ImGui::Begin("Properties");
	ImGui::SetWindowSize(ImVec2((float)widthX, (float)widthY));
	ImGui::SetWindowPos(ImVec2((float)posX, (float)posY));
	ImGui::End();
}
void PropertiesWindow::Content() {
	ImGui::Begin("Properties");
	if (PropertiesWindow::Obj != nullptr) {
		for (auto& obj : PropertiesWindow::Obj->ObjProperties) {
			obj->show();
		}
	}
	ImGui::End();
}


