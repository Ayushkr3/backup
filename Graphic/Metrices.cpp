#include "Metrices.h"
#include "Global.h"
#ifdef ImGUI_ENABLED
#define ERROR_METRICES(_status,_query) if(_status != ERROR_SUCCESS){std::cerr <<"Failed"<<_status<<std::endl;PdhCloseQuery(_query);}
int Metrice::counter = -1;
float Metrice::dTs[100] = { 0 };
float Metrice::FPS[100] = { 0 };
PDH_HCOUNTER Metrice::hCounter;
PDH_HQUERY Metrice::hQuery;
PDH_STATUS Metrice::hStatus;
double Metrice::Cusage = 0;
std::thread Metrice::cpuHandler;
bool Metrice::shutdown = 0;
#endif
float Metrice::fps = 0;
Timer Metrice::tim;
float Metrice::deltaTime = 0.0;
float* Globals::dT = &Metrice::deltaTime;
#ifdef ImGUI_ENABLED
void Metrice::GetWindows() {
	cpuHandler = std::thread(Metrice::GetCPU);
}
void Metrice::GetCPU() {
	hStatus = PdhOpenQuery(nullptr, 0, &(hQuery));
	ERROR_METRICES(hStatus, hQuery);
	hStatus = PdhAddCounter(hQuery, "\\Processor(_Total)\\% Processor Time", 0, &hCounter);
	ERROR_METRICES(hStatus, hQuery);
	while (!shutdown) {
		PDH_FMT_COUNTERVALUE value;
		hStatus = PdhCollectQueryData(hQuery);
		if (hStatus == ERROR_SUCCESS) {
			hStatus = PdhGetFormattedCounterValue(hCounter, PDH_FMT_DOUBLE, NULL, &value);
			if (hStatus == ERROR_SUCCESS)
				Cusage = (value.doubleValue);
		}
		Sleep(1000);
	}

}
void Metrice::ShowMetrices() {
	getFPS();
	ImGui::Begin("Window data");
	ImGui::PlotLines(("FPS (" + std::to_string(fps) + ")").c_str(), FPS, 100);
	ImGui::PlotLines(("dT (" + std::to_string(deltaTime) + ")").c_str(), dTs, 100);
	ImGui::Text(("CPU (" + std::to_string(Cusage) + ")").c_str());
	ImGui::End();
}
void Metrice::Destruct() {
	PdhCloseQuery(hQuery);
	cpuHandler.join();
}
#endif
void Metrice::MetriceTimeMark() {
	deltaTime = tim.Mark();
	fps = 1 / deltaTime;
}
#ifdef ImGUI_ENABLED
void Metrice::getFPS() {
	FPS[counter] = fps;
	dTs[counter] = deltaTime;
	counter++;
	counter = (counter == 100) ? 0 : counter;
}
#endif