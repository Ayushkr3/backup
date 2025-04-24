#pragma once
#include "Light.h"

Light::Light(WRL::ComPtr<ID3D11Device>pDevice) {
	ld.position.x = 0;
	ld.position.y = 0;
	ld.position.z = -5;
	ld.position.w = 1.0f;

	pCB = std::make_unique<ConstantBuffer>(&ld,pDevice);
}
void Light::UpdateBuffer(WRL::ComPtr<ID3D11DeviceContext>pContext) {
	pCB->BindToPSshader(pContext);
}