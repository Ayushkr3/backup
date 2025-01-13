#include <Windows.h>
#include <d3d11.h>
#include <string>
#include <wrl.h>
#include "errors.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "Scene.h"
#include "ObjParser/ObjParser.h"
#include <iterator>
#include <filesystem>
#include "Global.h"
#include <typeinfo>
#include "Graphic.h"
#include "EInputManager.h"
#include "Factory.h"
#include "font/IconsFontAwesome4.h"
class UIWindows {
public:
	HWND cHwnd;
	UIWindows(std::string className, HWND Phwnd, HINSTANCE hint, short x, short y, short w, short b, int windowN);
	void SetFullScreen(LPRECT rect);
};
class UIElements{
protected:
	static Microsoft::WRL::ComPtr<ID3D11Device> pUIDevice;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> pUIContext;
	static Microsoft::WRL::ComPtr<IDXGISwapChain>      pUISwapChain;
	static Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  pUIRenderTarget;
	static DXGI_SWAP_CHAIN_DESC UIscd;
public:
	bool isFullscreen = false;
	void ContainerStart();
	void ContainerEnd();
	static ImGuiContext* ctx;
	HWND cHwnd;
	UIElements(std::string className, HWND Phwnd, HINSTANCE hint, short x, short y, short w, short b, int windowsN);
	void UpdateUI();
	~UIElements();
	void Swap();
	void SetSizenWidth(int w,int b);
	static ImGuiIO* io;
};
class SceneManager {
	int posX;
	int posY;
	int widthX;
	int widthY;
	std::vector<Objects*>::iterator LookUp(short id, std::vector<Objects*>& vec);
	std::vector<Triangle*>::iterator LookUp(Triangle* Tri, std::vector<Triangle*>& vec);
public:
	static Scene* currentScene;
	void SetSizenWidth();
	void SetSizenWidth(UINT width, UINT height);
	SceneManager(int posX,int posY,int widthX,int widthY);
	void Content();
};
class PropertiesWindow  {
	int posX;
	int posY;
	int widthX;
	int widthY;
public:
	static bool closable;
	static Objects* Obj;
	void SetSizenWidth();
	void SetSizenWidth(UINT width, UINT height);
	PropertiesWindow(int posX, int posY, int widthX, int widthY);
	void Content();
};
class Files{
	int posX;
	int posY;
	int widthX;
	int widthY;
	std::filesystem::path working = WorkingDirectory;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
public:
	void SetSizenWidth();
	void SetSizenWidth(int width, int height);
	Files(int posX, int posY, int widthX, int widthY, Microsoft::WRL::ComPtr<ID3D11Device> pDevice,Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	void Content();
};
class ControlMenu {
	int posX;
	int posY;
	int widthX;
	int widthY;
	std::unique_ptr<UIElements>& UIWindow;
	std::unique_ptr<UIWindows>& win;
	std::unique_ptr<Graphic>& pgfx;
public:
	void SetSizenWidth();
	void SetSizenWidth(UINT width,UINT height);
	ControlMenu(int posX, int posY, int widthX, int widthY, std::unique_ptr<UIElements>& UIWindow, std::unique_ptr<UIWindows>& win, std::unique_ptr<Graphic>& pgfx);
	void Content();
};