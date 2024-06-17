#pragma once
#include "VertexShader.h"
class GeometryShader_:public VertexShader_{
public:
	GeometryShader_(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	void bind();
	void bind(Microsoft::WRL::ComPtr<ID3D11Buffer> soBuffer);
	~GeometryShader_() = default;
private:
private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>pGeometryShader;
};
