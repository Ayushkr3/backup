#pragma once
#include<d3d11.h>
#include<wrl.h>
#include <array>
#include "Scene.h"
#include "errors.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui.h"
#include "WIC.h"

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
	std::unique_ptr<Scene> pSc;
	void EndFrame();
	void ClearBuffer(float rgba[4]);
	//void ToDoFrame();
	void TestFrames(int x,int y);

public:

	bool showvalue = false;
	int t = 0;
	void UpdateImGui();
	//Test public code //
};