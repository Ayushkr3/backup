#pragma once
#include "VertexShader.h"

class PixelShader_ :public VertexShader_{
public:
	PixelShader_(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	void bind();
	~PixelShader_() = default;
private:
	
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>pPixelShader;
};