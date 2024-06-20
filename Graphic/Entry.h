#pragma once
#include"WindowsClass.h"
#include"Graphic.h"
#include "timer.h"
#include "Mouse.h"
#include <string>
#include "Metrices.h"

void DoFrame(window* Skeleton) {
	Metrice::MetriceTimeMark();
	float color[4] = {1.0,0.0,0.0,1.0 };
	Skeleton->pGfx->ClearBuffer(color);
#ifdef ImGUI_ENABLED
#endif // ImGUI_ENABLED
	Skeleton->pGfx->TestFrames();
#ifdef ImGUI_ENABLED
#endif
	Skeleton->pGfx->EndFrame();

}

int ApplicationGo(window* Skeleton) {
#ifdef ImGUI_ENABLED
	//Metrice::GetWindows();
	
#endif
	while (true) {
		if (const auto ecode = window::ProcessMessage()) {
			return *ecode;
		}
		DoFrame(Skeleton);
	}
}
