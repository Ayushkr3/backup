#pragma once
#include<wrl.h>
#include <d3d11.h>
#include "EPrimitives.h"
#include "Global.h"
#include "PropertiesRequirment.h"
#include "WIC.h"
namespace Primitives {
	class Texture :public PropertiesRequirment {
		class Sampler {
		public:
			Microsoft::WRL::ComPtr<ID3D11SamplerState> pSmpler;
			void CreateResource(ID3D11Device*& pIDevice, ID3D11DeviceContext*& pIContext);
			void Bind(ID3D11DeviceContext*& pIContext,short Index);
		};
	private:
	protected:
		ID3D11Device*& pIDevice;
		ID3D11DeviceContext*& pIContext;
		std::unique_ptr<PathToFile> file;
		std::string Name;
		short registerIndex;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		Microsoft::WRL::ComPtr <ID3D11Resource> pR;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV;
	public:
		std::string fileExtension();
		virtual void Bind();
		virtual short GetIndex();
		Texture(ID3D11Device*& pIDevice, ID3D11DeviceContext*& pIContext,std::string Name,short registerIndex);
		Sampler Sampler;
		virtual void CreateResource(std::unique_ptr<PathToFile>& ptf);
		virtual void show();
	};
	class Texture3D :public Texture {
	public:
		Texture3D(ID3D11Device*& pIDevice, ID3D11DeviceContext*& pIContext, std::string Name, short registerIndex);
		void CreateResource(std::unique_ptr<PathToFile>& ptf);
		void Bind();
		void show();
		short GetIndex();
	};
}