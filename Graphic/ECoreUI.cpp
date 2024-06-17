#include "ECoreUI.h"
#define CHECK_ERROR(hr) if(FAILED(hr)) throw error::error(hr,__LINE__)
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
UIWindows::UIWindows(std::string className, HWND Phwnd, HINSTANCE hint,float x ,float y,float w,float b,int windowsN) {
	cHwnd = CreateWindowEx(
		0,                          // Optional window styles.
		className.c_str(),                     // Window class
		std::to_string(windowsN).c_str(),				  // Window text
		WS_CHILD | WS_VISIBLE|WS_BORDER,     // Window style

							   // Size and position
		(int)x, (int)y, (int)w,(int)b,

		Phwnd,				// Parent window    
		(HMENU)windowsN,			   // Menu
		hint,			  // Instance handle
		NULL			 // Additional application data
	);
}
SceneManager::SceneManager(std::string className, HWND Phwnd, HINSTANCE hint, float x, float y, float w, float b,int windowsN,Scene* InitalScene) :ImGuiUIWindow(className, Phwnd, hint, x, y, w, b,windowsN), sizeX(w), sizeY(b) {
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
	ctx = ImGui::CreateContext();
	currentScene = InitalScene;
	SetWindowLongPtr(cHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	ImGui::SetCurrentContext(ctx);
	ImGui_ImplWin32_Init(cHwnd);
	ImGui_ImplDX11_Init(pUIDevice.Get(),pUIContext.Get());
}
void SceneManager::ClearRenderTarget() {
	ImGui::SetCurrentContext(ctx);
	float rgba[4] = { 0.0f,0.0f,0.0f,1.0f };
	pUIContext->OMSetRenderTargets(1, pUIRenderTarget.GetAddressOf(),nullptr);
	pUIContext->ClearRenderTargetView(pUIRenderTarget.Get(),rgba);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImVec2 pos(0,0);
	ImVec2 size(sizeX, sizeY);
	ImGui::Begin("Scene Object");
	ImGui::Selectable("check");
	ImGui::SetWindowPos(pos);
	ImGui::SetWindowSize(size);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::SetCurrentContext(nullptr);
	pUISwapChain->Present(1u, 0);
}
LRESULT CALLBACK SceneManager::SceneProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	SceneManager* const Sc = reinterpret_cast<SceneManager*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	Sc->ClearRenderTarget();
	return CallWindowProc((WNDPROC)GetWindowLongPtr(GetParent(Sc->cHwnd), GWLP_WNDPROC), hWnd, uMsg, wParam, lParam);
}
SceneManager::~SceneManager() {
	ImGui::SetCurrentContext(ctx);
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
ImGuiContext* SceneManager::GetCtx() {
	return ctx;
}
PropertiesWindow::PropertiesWindow(std::string className, HWND Phwnd, HINSTANCE hint, float x, float y, float w, float b,int windowsN) :ImGuiUIWindow(className, Phwnd, hint, x, y, w, b, windowsN), sizeX(w), sizeY(b){
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
	SetWindowLongPtr(cHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	ctx = ImGui::CreateContext();
	ImGui::SetCurrentContext(ctx);
	ImGui_ImplWin32_Init(cHwnd);
	ImGui_ImplDX11_Init(pUIDevice.Get(), pUIContext.Get());
}
void PropertiesWindow::ClearRenderTarget() {
	ImGui::SetCurrentContext(ctx);
	float rgba[4] = { 0.0f,0.0f,0.0f,1.0f };
	pUIContext->OMSetRenderTargets(1, pUIRenderTarget.GetAddressOf(), nullptr);
	pUIContext->ClearRenderTargetView(pUIRenderTarget.Get(), rgba);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImVec2 pos(0, 0);
	ImVec2 size(sizeX, sizeY);
	ImGui::Begin("Properties");
	ImGui::SetWindowPos(pos);
	ImGui::SetWindowSize(size);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	pUISwapChain->Present(1u, 0);
}
LRESULT CALLBACK PropertiesWindow::ProperProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	PropertiesWindow* const PWin = reinterpret_cast<PropertiesWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	PWin->ClearRenderTarget();
	return CallWindowProc((WNDPROC)GetWindowLongPtr(GetParent(PWin->cHwnd), GWLP_WNDPROC),hWnd,uMsg,wParam,lParam);
}
ImGuiContext* PropertiesWindow::GetCtx() {
	return ctx;
}
PropertiesWindow::~PropertiesWindow() {
	ImGui::SetCurrentContext(ctx);
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

Files::Files(std::string className, HWND Phwnd, HINSTANCE hint, float x, float y, float w, float b, int windowsN) :ImGuiUIWindow(className, Phwnd, hint, x, y, w, b, windowsN), sizeX(w), sizeY(b) {
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
	ctx = ImGui::CreateContext();
	ImGui::SetCurrentContext(ctx);
	SetWindowLongPtr(cHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	ImGui_ImplWin32_Init(cHwnd);
	ImGui_ImplDX11_Init(pUIDevice.Get(), pUIContext.Get());
}
void Files::ClearRenderTarget() {
	ImGui::SetCurrentContext(ctx);
	float rgba[4] = { 0.0f,0.0f,0.0f,1.0f };
	pUIContext->OMSetRenderTargets(1, pUIRenderTarget.GetAddressOf(), nullptr);
	pUIContext->ClearRenderTargetView(pUIRenderTarget.Get(), rgba);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImVec2 pos(0, 0);
	ImVec2 size(sizeX, sizeY);
	ImGui::Begin("Files");
	ImGui::SetWindowPos(pos);
	ImGui::SetWindowSize(size);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	pUISwapChain->Present(1u, 0);
}
ImGuiContext* Files::GetCtx() {
	return ctx;
}
LRESULT CALLBACK Files::FileProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	SceneManager* const F = reinterpret_cast<SceneManager*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	F->ClearRenderTarget();
	return CallWindowProc((WNDPROC)GetWindowLongPtr(GetParent(F->cHwnd), GWLP_WNDPROC), hWnd, uMsg, wParam, lParam);
}
Files::~Files() {
	ImGui::SetCurrentContext(ctx);
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

