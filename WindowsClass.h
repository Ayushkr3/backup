#pragma once
#include <Windows.h>
#include <optional>
#include <memory>
#include <windowsx.h>
#include"Graphic.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"



class window {
private:
	HINSTANCE hint;
	WNDCLASSEX wndClass;
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	int x=0;
	int y =0;
	Mouse mouse_cont;
	static std::optional<int> ProcessMessage();
	std::unique_ptr<Graphic> pGfx;
	window();
	HWND hwnd;
	~window();
};
