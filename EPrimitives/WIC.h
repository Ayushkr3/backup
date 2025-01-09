#pragma once
#include "EPrimitives.h"
#include <wincodec.h>
#include "Microsoft_wic.h"
#include <wrl.h>
#pragma comment(lib,"windowscodecs.lib")

class WIC {
public:
	static void SetImFac();
	static Microsoft::WRL::ComPtr<IWICImagingFactory> GetImFac();
	WIC();
public:
	static Microsoft::WRL::ComPtr<IWICImagingFactory> pImFac;
};