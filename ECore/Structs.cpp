#include "pch.h"
#include "Structs.h"
using namespace CB;
DirectX::XMFLOAT4X4 CB::ConvertMatrixToFloat4x4(DirectX::XMMATRIX mat) {
	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(&result, mat);
	return result;
}