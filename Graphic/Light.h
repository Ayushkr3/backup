#pragma once
#include <d3d11.h>
#include "ConstantBuff.h"
#include <memory>
#include <wrl.h>
using namespace Microsoft;
class Light {
	//struct LightData{
	//	struct {
	//		float x;
	//		float y;
	//		float z;
	//	}position;
	//	struct {
	//		float x;
	//		float y;
	//		float z;
	//	}Direction;
	//};
public:
	Light(WRL::ComPtr<ID3D11Device>pDevice);
	LightData ld;
	void UpdateBuffer(WRL::ComPtr<ID3D11DeviceContext>pContext);
private:
	std::unique_ptr<ConstantBuffer> pCB;
};
