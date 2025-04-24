#pragma once
#include<d3d11.h>
#include<wrl.h>
#include <array>
#include "Scene.h"
#include "errors.h"
#include "Global.h"
#include "Phys.h"
#ifdef ImGUI_ENABLED
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#endif
#include <dxgidebug.h>

#define CHECK_ERROR(hr) if(FAILED(hr)) throw error::error(hr,__LINE__)

class Graphic {
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
public:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>pDs;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	DXGI_SWAP_CHAIN_DESC scd;
	Graphic(HWND hwnd);
	~Graphic();
	Scene* pSc;
	void EndFrame();
	void ClearBuffer(float rgba[4]);
	//void ToDoFrame();
	void TestFrames();
public:
	Scene* GetCurrentScene();
	bool showvalue = true;
	int t = 0;
	void Resize(short sizeX,short sizeY);
#ifdef ImGUI_ENABLED
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> WireFrame;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> Solid;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pWireFrameSolid;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pLastShader;
#endif // ImGUI_ENABLED

	//Test public code //
};