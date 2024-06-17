#pragma once
#include"WindowsClass.h"
#include"Graphic.h"
#include "timer.h"
#include "Mouse.h"

void DoFrame(window* wg) {
	float color[4] = {0.0,0.0,0.0,1.0 };
	wg->pGfx->ClearBuffer(color);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	Mouse::ncd_calc();
	//ImGui::ShowDemoWindow();
	wg->pGfx->TestFrames();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	wg->pGfx->EndFrame();

}

int ApplicationGo(window* wg) {
	
	while (true) {
		if (const auto ecode = window::ProcessMessage()) {
			return *ecode;
		}
		DoFrame(wg);
	}
}
