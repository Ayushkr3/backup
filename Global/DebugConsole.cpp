#include "pch.h"
#include "Global.h"
std::thread DebugConsole::DebugThread;
bool DebugConsole::isFinished = false;
std::mutex* DebugConsole::mute;
std::condition_variable DebugConsole::cv;
bool DebugConsole::Pushed;
std::queue<std::string>* DebugConsole::Message_q;
HANDLE DebugConsole::hMapFile = nullptr;
LPVOID DebugConsole::ptrToMem = nullptr;
DebugConsole::SharedData* DebugConsole::Data = nullptr;
void DebugConsole::InitDebugConsole() {
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitle(L"Console");
	hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(SharedData),L"DebugConsoleSharedMemory");
	ptrToMem = MapViewOfFile(
		hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(SharedData)
	);
	DebugConsole::mute = new std::mutex;
	DebugConsole::Message_q = new std::queue<std::string>;
	Data = new SharedData;
	Data->Message_q = Message_q;
	Data->mute = DebugConsole::mute;
	CopyMemory(ptrToMem, Data, sizeof(SharedData));
	DebugThread = std::thread(&DebugConsole::RunThread);
	DebugThread.detach();
	DebugConsole::Log("");
}
void DebugConsole::DestroyConsole() {
	isFinished = true;
	if (ptrToMem !=nullptr) {
		UnmapViewOfFile(ptrToMem);
	}
	if (hMapFile) {
		CloseHandle(hMapFile);
	}
	if (DebugConsole::Data != nullptr) {
		delete DebugConsole::Data;
	}
	FreeConsole();
}
void DebugConsole::RunThread() {
	while (!isFinished) {
		std::unique_lock<std::mutex> lock(*mute);
		cv.wait(lock, [] {return Pushed; });//lambda ?
		while (!Message_q->empty()) {
			std::cout << Message_q->front() << std::endl;
			Message_q->pop();
		}
	}
}
void DebugConsole::Log(std::string Message) {
	std::lock_guard<std::mutex> lock(*mute);
	Message_q->push(Message);
	cv.notify_one();
	Pushed = true;
}
DebugConsole* DebugConsole::GetInstance() {
	if(hMapFile == nullptr)
	hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, L"DebugConsoleSharedMemory");
	if (ptrToMem == nullptr) {
		ptrToMem = MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,sizeof(SharedData));
		if (DebugConsole::Message_q == nullptr || DebugConsole::mute == nullptr) {
			DebugConsole::Message_q = ((SharedData*)(ptrToMem))->Message_q;
			DebugConsole::mute = ((SharedData*)(ptrToMem))->mute;
		}
	}
	return reinterpret_cast<DebugConsole*>(ptrToMem);;
}