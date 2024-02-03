#include "ConstantBuff.h"
#include "errors.h"

class Camera {
public:
	Camera(Microsoft::WRL::ComPtr<ID3D11Device> pDevice);
	float postion[3] = { 0,0,0 };
	float focus[3] = { 0,0,1 };
	float rotation[2] = { 0,0 };
	void calculateProjection(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, PerFrameData* ViewMatrix);
	PerFrameData GetViewMatrix();
private:
	PerFrameData viewXprojection;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pFrameConstantBuffer;
	XMMATRIX viewmatrix;
	XMMATRIX projection;
	
};