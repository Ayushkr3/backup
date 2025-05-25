#pragma once
#include <d3d11.h>
#include "Global.h"
#include "ECore.h"
namespace UI {
	struct UIVertex {
		struct {
			float x;
			float y;
			float z;
		}position;
		struct {
			float u;
			float v;
		}UV;
		constexpr UIVertex(float x, float y, float z, float u, float v) :position{x,y,z},UV{u, v} {};
	};
	struct UIPosition{
		short x;
		short y;
	};
	class BaseUI {
	public:
		static constexpr std::array<uint16_t, 6> indices = { 0, 2,1, 2,3,0 };
		static constexpr std::array<UIVertex,6> enclosingQuad = { 
			UIVertex(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f),
			UIVertex(0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			UIVertex(0.5f,  0.5f, 0.0f, 1.0f, 1.0f),
			UIVertex(0.5f,  0.5f, 0.0f, 1.0f, 1.0f),
			UIVertex(-0.5f,  0.5f, 0.0f, 0.0f, 1.0f),
			UIVertex(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f)
		};
	public:
		CORE_EXP virtual void Action();
		static Microsoft::WRL::ComPtr<ID3D10Blob> pVSBlob;
		static Microsoft::WRL::ComPtr<ID3D10Blob> pPSBlob;
		static Microsoft::WRL::ComPtr<ID3D11InputLayout> pIL;
		static short Count;
		static Microsoft::WRL::ComPtr<ID3D11VertexShader>pVS;
		static Microsoft::WRL::ComPtr<ID3D11PixelShader>pPS;
		static Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuff;
		static Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuff;
		static Microsoft::WRL::ComPtr<ID3D11Buffer> orthoBuffer;
		static Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
		static DirectX::XMMATRIX ortho;
		static short sizeX;
		static short sizeY;
	};
	class Button : public Objects, public BaseUI {
	public:
		//CORE_EXP void Action();
		CORE_EXP Button(short id);
		std::vector<ObjectProperties*> properties;
		CORE_EXP std::vector<ObjectProperties*>* GetProperties();
	};
	CORE_EXP void InitalizeUI(Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pContext);
	CORE_EXP void RenderUI(Microsoft::WRL::ComPtr<ID3D11Device>& pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pContext);
	CORE_EXP void Resize(short sizeX,short sizeY, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pContext);
};