#pragma once
#include "VertexShader.h"

class HullShader_:public VertexShader_ {
public:
	HullShader_(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	void bind();
private:
	Microsoft::WRL::ComPtr<ID3D11HullShader> pHull;
};
