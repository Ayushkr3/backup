#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "Triangle.h"
#include "Camera.h"
class Scene {
public:
	Scene(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	void Render();
private:
	 Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	 Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
public:
	Camera cam;
	//
	//
	std::vector<Triangle> Triangles;
};