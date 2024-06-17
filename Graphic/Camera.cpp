#include"Camera.h"
#define CHECK_ERROR(hr) if(FAILED(hr)) throw error::error(hr,__LINE__)

Camera::Camera(Microsoft::WRL::ComPtr<ID3D11Device> pDevice)
{
	viewmatrix =XMMatrixLookAtLH(
		XMVectorSet(postion[0], postion[1], postion[2],1.0f),
		XMVectorSet(focus[0],focus[1],focus[2],1.0f),
		XMVectorSet(0.0f,1.0f,0.0f,0.0f));
#ifdef ImGUI_ENABLED
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), 1.791225416036309f, 0.1f, 100.0f);
#else
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), 1.33333333f, 0.1f, 100.0f);
#endif // ImGUI_ENABLED

	viewXprojection.viewMat = ConstantBuffer::ConvertMatrixToFloat4x4(viewmatrix * projection);

	D3D11_BUFFER_DESC CbuffDesc;
	CbuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CbuffDesc.ByteWidth = sizeof(PerObjectData);
	CbuffDesc.Usage = D3D11_USAGE_DEFAULT;
	CbuffDesc.MiscFlags = 0;
	CbuffDesc.StructureByteStride = 0;
	CbuffDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA Csbr;
	Csbr.pSysMem = &viewXprojection;

	CHECK_ERROR(pDevice->CreateBuffer(&CbuffDesc, &Csbr, &pFrameConstantBuffer));
}
void Camera::calculateProjection(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, PerFrameData* ViewMatrix) {
	pContext->UpdateSubresource(pFrameConstantBuffer.Get(), 0, nullptr,ViewMatrix,0,0 );
	pContext->GSSetConstantBuffers(1, 1, pFrameConstantBuffer.GetAddressOf());
}
PerFrameData Camera::GetViewMatrix() {
	//////////////////////////
	if (rotation[0] > 90) {
		rotation[0] = 89;
	}
	if (rotation[0] <-90) {
		rotation[0] = -89;
	}
	if (rotation[1] > 360) {
		rotation[1] = 0;
	}
	if (rotation[1] < 0) {
		rotation[1] = 359;
	}
	//////////////////////////
	XMVECTOR pos = XMVectorSet(postion[0], postion[1], postion[2], 1.0f);
	XMMATRIX Rotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation[0]), XMConvertToRadians(rotation[1]), 0.0f);
	XMVECTOR forwardDirection = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	forwardDirection = XMVector3TransformCoord(forwardDirection, Rotation);

	XMMATRIX viewmatrix = XMMatrixTranspose(XMMatrixLookAtLH(
		pos,
		pos + forwardDirection,
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));

	XMMATRIX projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), 1.33333333f, 0.1f, 100.0f));

	viewXprojection.viewMat = ConstantBuffer::ConvertMatrixToFloat4x4(projection*viewmatrix );
	return viewXprojection;
	
}