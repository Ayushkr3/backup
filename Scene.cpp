#include "Scene.h"
#include "ConstantBuff.h"

Scene::Scene(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext)
	:cam(pDevice), pContext(pContext),light(pDevice)

{
	for (int i = 0; i < 1; i++) {
			Triangles.emplace_back(pDevice, pContext);
		
	}
}
void Scene::Render() {

	cam.calculateProjection(pContext, &(cam.GetViewMatrix()));
	light.UpdateBuffer(pContext);
	for (auto& Triangle : Triangles) {
		Triangle.Draw();
	}
	
}
