#include"errors.h"
#pragma comment(lib,"legacy_stdio_definitions.lib")
error::error(HRESULT hr,int line_number) {

	std::string templine = std::to_string(line_number);
	MessageBox(nullptr, GeterrorDescription(hr).c_str(), (GeterrorName(hr) + " at " + templine).c_str(), MB_OK);
	
}
std::string error::GeterrorDescription(HRESULT hr) {
	wchar_t* errorDescription = nullptr;
	// Allocate the buffer
	errorDescription = new wchar_t[512];

	// Retrieve the error description
	DXGetErrorDescription(hr, errorDescription,512);

	// Convert the wide string to a narrow string
	int utf8Size = WideCharToMultiByte(CP_UTF8, 0, errorDescription, -1, nullptr, 0, nullptr, nullptr);
	std::string utf8String(utf8Size, 0);
	WideCharToMultiByte(CP_UTF8, 0, errorDescription, -1, &utf8String[0], utf8Size, nullptr, nullptr);

	// Clean up the allocated memory
	delete[] errorDescription;
	return utf8String;
}
std::string error::GeterrorName(HRESULT hr) {
	const wchar_t* errorName = DXGetErrorString(hr);
	std::wstring ws(errorName);
	std::string str(ws.begin(), ws.end());
	return str;
}
