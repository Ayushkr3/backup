#pragma once
#include <Windows.h>
#include <optional>
#include <memory>
#include <windowsx.h>
#include"Graphic.h"
#include "Mouse.h"
#include "Keyboard.h"

#ifdef ImGUI_ENABLED
#include "ECoreUI.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#endif


class window {
private:
	HINSTANCE hint;
	WNDCLASSEX wndClass;
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Mouse mouse_cont;
	static std::optional<int> ProcessMessage();
	std::unique_ptr<Graphic> pGfx;
	std::unique_ptr<UIElements> UIwindow;
	std::unique_ptr<SceneManager> Scene;
	std::unique_ptr<PropertiesWindow> Properties;
	std::unique_ptr<Files> file;
	std::unique_ptr<UIWindows> RenderTargetWindows;
	std::unique_ptr<ControlMenu> Control;
	window();
	HWND hwnd;
	~window();
};
