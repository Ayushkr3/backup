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
	UIWindows(std::string className, HWND Phwnd, HINSTANCE hint, short x, short y, short w, short b, int windowN);
	
};
class UIElements{
protected:
	static Microsoft::WRL::ComPtr<ID3D11Device> pUIDevice;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> pUIContext;
	static Microsoft::WRL::ComPtr<IDXGISwapChain>      pUISwapChain;
	static Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  pUIRenderTarget;
	static DXGI_SWAP_CHAIN_DESC UIscd;
public:
	HWND cHwnd;
	UIElements(std::string className, HWND Phwnd, HINSTANCE hint, short x, short y, short w, short b, int windowsN);
	void UpdateUI();
	~UIElements();
	void Swap();
};
class SceneManager {
	int posX;
	int posY;
	int widthX;
	int widthY;
public:
	void SetSizenWidth();
	SceneManager(int posX,int posY,int widthX,int widthY);
	void Content();
};
class PropertiesWindow  {
	int posX;
	int posY;
	int widthX;
	int widthY;
public:
	void SetSizenWidth();
	PropertiesWindow(int posX, int posY, int widthX, int widthY);
	void Content();
};
class Files{
	
};