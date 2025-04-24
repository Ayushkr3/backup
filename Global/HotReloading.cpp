#include "pch.h"
#include "Global.h"

HANDLE HotReloading::hMapFile;
LPVOID HotReloading::ptrToMem;
std::multimap<std::string, std::function<ObjectProperties*(Objects*)>>* HotReloading::PropertiesPool = nullptr;
HotReloading::SharedData* HotReloading::Data = nullptr;
ImGuiContext* HotReloading::IMctx = nullptr;
std::multimap<std::string, std::function<ObjectProperties*(Objects*)>> HotReloading::InternalPool;


void HotReloading::Init(std::multimap<std::string, std::function<ObjectProperties*(Objects*)>>* GlobalPropertiesPool, ImGuiContext* ctx) {
	hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, sizeof(SharedData), L"HotReload");
	ptrToMem = MapViewOfFile(hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(SharedData));
	Data = new SharedData;
	Data->GlobalPropertiesPool = GlobalPropertiesPool;
	Data->ctx = ctx;
	CopyMemory(ptrToMem, Data, sizeof(SharedData));
}
void HotReloading::GetInstance() {
	hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, L"HotReload");
	if (hMapFile == nullptr)
		return;
	if (ptrToMem == nullptr) {
		ptrToMem = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedData));
		PropertiesPool = ((SharedData*)(ptrToMem))->GlobalPropertiesPool;
		IMctx = ((SharedData*)(ptrToMem))->ctx;
		ImGui::SetCurrentContext(IMctx);
	}
}
void HotReloading::DeInit() {
	if (Data != nullptr) {
		delete Data;
		Data = 0x0;
	}
	if (ptrToMem != nullptr) {
		UnmapViewOfFile(ptrToMem);
	}
	if (hMapFile) {
		CloseHandle(hMapFile);
	}
}
void HotReloading::RegClass(std::string ClassName, std::function<ObjectProperties*(Objects*)> ptrToFactoryFunction) {
	InternalPool.insert({ ClassName,ptrToFactoryFunction });
	PropertiesPool->insert({ClassName,ptrToFactoryFunction});
}
void HotReloading::NukeClass() {
	for (auto& it : InternalPool) {
		PropertiesPool->erase(it.first);
	}
	InternalPool.clear();
}