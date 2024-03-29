#include"WindowsClass.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

window::window():
	hint(GetModuleHandle(nullptr))
{
	ImGui::CreateContext();
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
	wndClass.lpszClassName = "OI";
	wndClass.style = CS_OWNDC;
	RegisterClassEx(&wndClass);
	


	hwnd = CreateWindowEx(
		0,                          // Optional window styles.
		"OI",                     // Window class
		"Start",				  // Window text
		WS_OVERLAPPEDWINDOW,     // Window style

							   // Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, 1200, 700,

		NULL,				// Parent window    
		NULL,			   // Menu
		hint,			  // Instance handle
		NULL			 // Additional application data
	);
	ImGui_ImplWin32_Init(hwnd);
	pGfx = std::make_unique<Graphic>(hwnd);
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
		window* const pWnd = static_cast<window*>(pCreate->lpCreateParams);
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
	ImGuiIO& io = ImGui::GetIO();
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!io.WantCaptureKeyboard){
			if (wParam == VK_SPACE) {
				PostQuitMessage(0);
			}
		}
		return 0;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (!io.WantCaptureKeyboard) {
			break;
		}
		return 0;
	case WM_CHAR:
		if (!io.WantCaptureKeyboard) {
			break;
		}
		return 0;
	case WM_MOUSEMOVE:
			Mouse::mouse_xpos = GET_X_LPARAM(lParam);
			Mouse::mouse_ypos = GET_Y_LPARAM(lParam);
		
		return 0;
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
	ImGui_ImplWin32_Shutdown();
	//pGfx->~Graphic();
	DestroyWindow(hwnd);
	UnregisterClass("OI",hint);
}