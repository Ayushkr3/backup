#include "WIC.h"

Microsoft::WRL::ComPtr<IWICImagingFactory> WIC::pImFac;
WIC::WIC() {
	
}
void WIC::SetImFac() {
	CoInitialize(NULL);
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(pImFac.GetAddressOf()));
}
Microsoft::WRL::ComPtr<IWICImagingFactory> WIC::GetImFac() {
	return pImFac;
}