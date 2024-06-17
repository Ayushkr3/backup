#include "Scene.h"
#include "ConstantBuff.h"

Scene::Scene(Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext)
	:cam(pDevice),pContext(pContext)
{
	std::vector<float> testangle = { 0.0f,0.0};
	for (auto& angles:testangle) {
		Triangles.emplace_back(pDevice,pContext);
	}
}
void Scene::Render() {
	cam.calculateProjection(pContext,&(cam.GetViewMatrix()));

	for (auto& Triangle : Triangles) {
		Triangle.Draw();
	}
}
