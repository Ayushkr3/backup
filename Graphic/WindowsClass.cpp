#include"WindowsClass.h"
#ifdef ImGUI_ENABLED
#include "Metrices.h"
#include <string>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
window::window():
	hint(GetModuleHandle(nullptr))
{
	std::string className = "Engine";
#ifdef ImGUI_ENABLED

#endif
	wndClass.hIcon = nullptr;
	wndClass.hIconSm = nullptr;
	wndClass.lpszMenuName = nullptr;
	wndClass.hCursor = nullptr;
	wndClass.hbrBackground = nullptr;
	wndClass.lpszMenuName = nullptr;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.hInstance = hint;
	wndClass.lpfnWndProc = HandleMsgSetup;
	wndClass.lpszClassName = className.c_str();
	wndClass.style = CS_OWNDC;
	RegisterClassEx(&wndClass);

#ifdef ImGUI_ENABLED
	hwnd = CreateWindowEx(
		0,                          // Optional window styles.
		className.c_str(),                     // Window class
		"Engine",				  // Window text
		WS_OVERLAPPEDWINDOW,     // Window style

							   // Size and position
		0, 0, 1600, 900,

		NULL,				// Parent window    
		NULL,			   // Menu
		hint,			  // Instance handle
		NULL			 // Additional application data
	);
#else 
	hwnd = CreateWindowEx(
		0,                          // Optional window styles.
		className.c_str(),                     // Window class
		"Start",				  // Window text
		WS_OVERLAPPEDWINDOW,     // Window style

							   // Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,

		NULL,				// Parent window    
		NULL,			   // Menu
		hint,			  // Instance handle
		NULL			 // Additional application data
	);
#endif // ImGUI_ENABLED

	RenderTargetWindows = std::make_unique<UIWindows>(className, hwnd, hint,300,0, 1024, 576,2);
	pGfx = std::make_unique<Graphic>(RenderTargetWindows.get()->cHwnd);
	UIwindow = std::make_unique<UIElements>(className, hwnd, hint, 0, 0, 1600, 900, 1);
	Scene = std::make_unique<SceneManager>(0, 0, 300, 900);
	Properties = std::make_unique<PropertiesWindow>(1324, 0, 276, 900);
	UIwindow->UpdateUI();
	Scene->SetSizenWidth();
	Properties->SetSizenWidth();
	UIwindow->Swap();
	SetWindowPos(UIwindow->cHwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetWindowPos(RenderTargetWindows->cHwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	ShowWindow(hwnd, SW_SHOWDEFAULT);
}
std::optional<int> window::ProcessMessage() {
	MSG msg;
	
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}

		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

LRESULT CALLBACK window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		window* const pWnd = (window*)pCreate->lpCreateParams;
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&window::HandleMsgThunk));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window instance
	window* const pWnd = reinterpret_cast<window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}
LRESULT window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
		return true;
}
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_INPUT:
#ifdef ImGUI_ENABLED
		/*if (io.WantCaptureMouse) {
			Mouse::deltaMouseX = 0;
			Mouse::deltaMouseY = 0;*/
			break;
	//}
#endif // ImGUI_ENABLED

		// x = mouse_cont.GetRawData(&raw, &lParam);
		// y = mouse_cont.GetRawData(&raw, &lParam)[1];
		
		return 0;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
#ifdef ImGUI_ENABLED
		/*if (!io.WantCaptureKeyboard){
			if (wParam == VK_UP) {
				Keyboard::isPressedUP = true;
				break;
			}
			if (wParam == VK_DOWN) {
				Keyboard::isPressedDown = true;
				break;
			}
			if (wParam == VK_LEFT) {
				Keyboard::isPressedLeft = true;
				break;
			}
			if (wParam == VK_RIGHT) {
				Keyboard::isPressedRight = true;
				break;
			}*/
			if (wParam == VK_SPACE) {
				PostQuitMessage(0);
			}/* 
			if (wParam == VK_SHIFT) {
				Keyboard::isPressedYPositive = true;
			}
			if (wParam == VK_CONTROL) {
				Keyboard::isPressedYNegative = true;
			}
			if (wParam == VK_RETURN) {
				if (!Keyboard::WantCamControl) {
					Keyboard::WantCamControl = true;
					break;
				}
				else {
					Keyboard::WantCamControl = false;
					break;
				}
			}
		}*/
#endif
		return 0;
	case WM_KEYUP:
	case WM_SYSKEYUP:
#ifdef ImGUI_ENABLED
		/*if (!io.WantCaptureKeyboard) {
			if (wParam == VK_UP) {
				Keyboard::isPressedUP = false;
				break;
			}
			if (wParam == VK_DOWN) {
				Keyboard::isPressedDown = false;
				break;
			}
			if (wParam == VK_LEFT) {
				Keyboard::isPressedLeft = false;
				break;
			}
			if (wParam == VK_RIGHT) {
				Keyboard::isPressedRight = false;
				break;
			}
			if (wParam == VK_SHIFT) {
				Keyboard::isPressedYPositive = false;
			}
			if (wParam == VK_CONTROL) {
				Keyboard::isPressedYNegative = false;
			}
			if (wParam == VK_RETURN) {
				Keyboard::WantCamControl = false;
			}
		}*/
#endif
		return 0;
	case WM_CHAR:
#ifdef ImGUI_ENABLED
		/*if (!io.WantCaptureKeyboard) {
			break;
		}*/
#endif
		return 0;
	/*case WM_MOUSEMOVE:
			Mouse::mouse_xpos = GET_X_LPARAM(lParam);
			Mouse::mouse_ypos = GET_Y_LPARAM(lParam);
		
		return 0;*/
	case WM_LBUTTONDOWN:
		Mouse::mouse_button[0] = true;
		return 0;
	case WM_LBUTTONUP:
		Mouse::mouse_button[0] = false;
		return 0;
	case WM_RBUTTONDOWN:
		Mouse::mouse_button[1] = true;
		return 0;
	case WM_RBUTTONUP:
		Mouse::mouse_button[1] = false;
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

window::~window() {
#ifdef ImGUI_ENABLED
	//ImGui_ImplWin32_Shutdown();
	//Metrice::shutdown = 1;
	//Metrice::Destruct();
#endif // ImGUI_ENABLED
	DestroyWindow(hwnd);
	UnregisterClass("OI",hint);
	//mouse_cont.DestroyRawInput();
}
