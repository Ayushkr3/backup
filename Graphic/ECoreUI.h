#include <Windows.h>
#include <d3d11.h>
#include <string>
#include <wrl.h>
#include "errors.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "Scene.h"
class UIWindows {
public:
	HWND cHwnd;
	UIWindows(std::string className, HWND Phwnd, HINSTANCE hint, float x, float y, float w, float b, int windowN);
	
};
class ImGuiUIWindow:public UIWindows{
public:
	ImGuiUIWindow(std::string className, HWND Phwnd, HINSTANCE hint, float x, float y, float w, float b, int windowN) :UIWindows(className, Phwnd, hint,x,y,w,b,windowN) {}
	virtual ImGuiContext* GetCtx() = 0;
	virtual void ClearRenderTarget() = 0;
};
class SceneManager:public ImGuiUIWindow {
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pUIDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pUIContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>      pUISwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  pUIRenderTarget;
	DXGI_SWAP_CHAIN_DESC UIscd;
	float sizeX;
	float sizeY;
	ImGuiContext* ctx;
public:
	static LRESULT CALLBACK SceneProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	Scene* currentScene;
	void ClearRenderTarget();
	~SceneManager();
	SceneManager(std::string className, HWND Phwnd, HINSTANCE hint, float x, float y, float w, float b, int windowN, Scene* InitalScene);
	ImGuiContext* GetCtx();
};
class PropertiesWindow :public ImGuiUIWindow {
	Microsoft::WRL::ComPtr<ID3D11Device> pUIDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pUIContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>      pUISwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  pUIRenderTarget;
	DXGI_SWAP_CHAIN_DESC UIscd;
	float sizeX;
	float sizeY;
	ImGuiContext* ctx;
public:
	static LRESULT CALLBACK ProperProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	~PropertiesWindow();
	void ClearRenderTarget();
	PropertiesWindow(std::string className, HWND Phwnd, HINSTANCE hint, float x, float y, float w, float b, int windowN);
	ImGuiContext* GetCtx();
};
class Files:public ImGuiUIWindow {
	Microsoft::WRL::ComPtr<ID3D11Device> pUIDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pUIContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>      pUISwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  pUIRenderTarget;
	DXGI_SWAP_CHAIN_DESC UIscd;
	float sizeX;
	float sizeY;
	ImGuiContext* ctx;
public:
	static LRESULT CALLBACK FileProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	~Files();
	void ClearRenderTarget();
	Files(std::string className, HWND Phwnd, HINSTANCE hint, float x, float y, float w, float b, int windowN);
	ImGuiContext* GetCtx();
};