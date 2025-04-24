#pragma once
#include <Pdh.h>
#include <Windows.h>
#include "WindowsClass.h"
#include "timer.h"
#include <Pdh.h>
#include <iostream>
#include <thread>
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#pragma comment(lib,"pdh.lib")

class Metrice {
public:
	static float deltaTime;
	static float fps;
private:
#ifdef ImGUI_ENABLED
	static PDH_HCOUNTER hCounter;
	static PDH_HQUERY hQuery;
	static PDH_STATUS hStatus;
	static double Cusage;
	static void GetCPU();
	static inline void getFPS();
	static float dTs[100];
	static float FPS[100];
	static int counter;
	static std::thread cpuHandler;
#endif
	static Timer tim;
public:
	static bool shutdown;
	static void Destruct();
	static void MetriceTimeMark();
	static void GetWindows();
	static void ShowMetrices();
};

