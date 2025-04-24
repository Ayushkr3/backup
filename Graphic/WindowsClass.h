#pragma once
#include <Windows.h>
#include <optional>
#include <memory>
#include <windowsx.h>
#include"Graphic.h"
#include "EInputManager.h"
#include <thread>
#ifdef ImGUI_ENABLED
#include "ECoreUI.h"
#include "Factory.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#endif
struct WindowRef;
class window {
private:
	HINSTANCE hint;
	WNDCLASSEX wndClass;
	int scaledWidth;
	int scaledHeight;
	WindowRef* winRef;
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
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
struct WindowRef {
	struct {
		int* widthX;
		int* heightY;
		std::unique_ptr<Files>* ptrFiles;
		std::unique_ptr<PropertiesWindow>* ptrProp;
		std::unique_ptr<UIElements>* ptrElem;
	}Additional_Param;
	window* ContructParam = nullptr;
};