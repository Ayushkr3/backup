#pragma once
#include <DirectXMath.h>
struct PerFrameData {
	DirectX::XMFLOAT4X4 viewMat;
};
struct PerObjectData {
	DirectX::XMFLOAT4X4 Rotation;
	DirectX::XMFLOAT4X4 Translation;
	DirectX::XMFLOAT4X4 Scale;
};
DirectX::XMFLOAT4X4 ConvertMatrixToFloat4x4(DirectX::XMMATRIX mat);