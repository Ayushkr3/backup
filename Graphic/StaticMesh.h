//#pragma once
//#include <wrl.h>
//#include <vector>
//#include <d3d11.h>
//#include "errors.h"
//#include "ConstantBuff.h"
//#define CHECK_ERROR(hr) if(FAILED(hr)) throw error::error(hr,__LINE__)
//
//struct Vertex {
//	struct {
//		float x;
//		float y;
//		float z;
//	}position;
//	struct {
//		float u;
//		float v;
//	}texcoord;
//	struct {
//		float x;
//		float y;
//		float z;
//	}Normal;
//};
//class StaticMesh {
//private:
//	Microsoft::WRL::ComPtr<ID3D11Device>pDevice;
//public:
//	StaticMesh(Microsoft::WRL::ComPtr<ID3D11Device>pDevice);
//	void AddToStaticMesh(std::vector<Vertex> vertices, std::vector<unsigned int> Index, PerObjectData* Transformation);
//	void BindStaticMesh(Microsoft::WRL::ComPtr<ID3D11DeviceContext>pContext);
//	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext>pContext);
//private:
//	std::vector<D3D11_SUBRESOURCE_DATA> sbr;
//	std::vector<D3D11_SUBRESOURCE_DATA> sbrIndex;
//	static std::vector<std::vector<Vertex>> AllStaticMeshVertex;
//	static std::vector<std::vector<unsigned int>> AllStaticMeshIndex;
//	static std::vector<ConstantBuffer> pCB;
//	std::vector<D3D11_BUFFER_DESC> AllStaticMeshBuffer;
//	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> VertexBufferArray;
//	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> IndexBufferArray;
//};