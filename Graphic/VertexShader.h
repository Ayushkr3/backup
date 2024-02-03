#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include "ConstantBuff.h"
class VertexShader_ {
public:
	VertexShader_(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	virtual void bind();
	Microsoft::WRL::ComPtr<ID3D10Blob> rBlob();
	virtual ~VertexShader_() = default;
protected:
	Microsoft::WRL::ComPtr<ID3D10Blob> pBlob;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
};
