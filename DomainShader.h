#pragma once
#include "VertexShader.h"

class DomainShader_:public VertexShader_ {
public:
	DomainShader_(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	void bind();
private:
	Microsoft::WRL::ComPtr<ID3D11DomainShader> pDomain;
};