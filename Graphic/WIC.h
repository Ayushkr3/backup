#pragma once
#include <wincodec.h>
#include "Microsoft_wic.h"
#include <wrl.h>
#pragma comment(lib,"windowscodecs.lib")

class WIC {
public:
	static void SetImFac();
	static Microsoft::WRL::ComPtr<IWICImagingFactory> GetImFac();
	WIC();
private:
	static Microsoft::WRL::ComPtr<IWICImagingFactory> pImFac;
};