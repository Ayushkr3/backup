#include "ECoreUI.h"
#define CHECK_ERROR(hr) if(FAILED(hr)) throw error::error(hr,__LINE__)
Microsoft::WRL::ComPtr<ID3D11Device> UIElements::pUIDevice = nullptr;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> UIElements::pUIContext= nullptr;
Microsoft::WRL::ComPtr<IDXGISwapChain>      UIElements::pUISwapChain= nullptr;
Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  UIElements::pUIRenderTarget= nullptr;
DXGI_SWAP_CHAIN_DESC UIElements::UIscd;
ImGuiIO* UIElements::io = nullptr;
namespace  fs=std::filesystem ;
Scene* SceneManager::currentScene = nullptr;
UIWindows::UIWindows(std::string className, HWND Phwnd, HINSTANCE hint,short x ,short y,short w,short b,int windowsN) {
	cHwnd = CreateWindowEx(
		0,                          // Optional window styles.
		className.c_str(),                     // Window class
		std::to_string(windowsN).c_str(),				  // Window text
		WS_CHILD | WS_VISIBLE|WS_BORDER,     // Window style

							   // Size and position
		x, y, w,b,

		Phwnd,				// Parent window    
		(HMENU)windowsN,			   // Menu
		hint,			  // Instance handle
		NULL			 // Additional application data
	);
}
UIElements::UIElements(std::string className, HWND Phwnd, HINSTANCE hint, short x, short y, short w, short b, int windowsN) {
	if (pUIDevice == nullptr) {
		ImGui::CreateContext();
		UIElements::io = &ImGui::GetIO();
		io->FontGlobalScale = 1.4f;
		io->WantCaptureMouse = true;
		io->WantCaptureKeyboard = true;
		io->WantTextInput = true;
		io->Fonts->AddFontDefault();
		static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig config;
		config.MergeMode = true;
		config.GlyphMinAdvanceX = 13.0f;
		io->Fonts->AddFontFromFileTTF("D:/program/vs/graphic/Graphic/font/fontawesome-webfont.ttf",13.0f,&config, icon_ranges);
		io->ConfigFlags = ImGuiConfigFlags_NavEnableSetMousePos;
		cHwnd = CreateWindowEx(
			0,                          // Optional window styles.
			className.c_str(),                     // Window class
			std::to_string(windowsN).c_str(),				  // Window text
			WS_CHILD |WS_VISIBLE |WS_CLIPSIBLINGS | WS_BORDER,     // Window style

								   // Size and position
			x, y, w, b,

			Phwnd,				// Parent window    
			(HMENU)windowsN,			   // Menu
			hint,			  // Instance handle
			NULL			 // Additional application data
		);
		UIscd.BufferCount = 1;
		UIscd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		UIscd.BufferDesc.Height = 0;
		UIscd.BufferDesc.Width = 0;
		UIscd.BufferDesc.RefreshRate.Numerator = 0;
		UIscd.BufferDesc.RefreshRate.Denominator = 0;
		UIscd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		UIscd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		UIscd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		UIscd.Flags = 0;
		UIscd.OutputWindow = cHwnd;
		UIscd.SampleDesc.Count = 1;
		UIscd.SampleDesc.Quality = 0;
		UIscd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		UIscd.Windowed = true;
		CHECK_ERROR(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION, &UIscd, &pUISwapChain, &pUIDevice, nullptr, &pUIContext));
		ID3D11Resource* pBackBuffer = nullptr;
		CHECK_ERROR(pUISwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)));
		CHECK_ERROR(pUIDevice->CreateRenderTargetView(pBackBuffer, nullptr, pUIRenderTarget.GetAddressOf()));
		pBackBuffer->Release();
		ImGui_ImplWin32_Init(cHwnd);
		ImGui_ImplDX11_Init(pUIDevice.Get(), pUIContext.Get());
	}
}
void UIElements::UpdateUI() {
	float rgba[4] = { 0.0f,0.0f,0.0f,1.0f };
	pUIContext->OMSetRenderTargets(1, pUIRenderTarget.GetAddressOf(), nullptr);
	pUIContext->ClearRenderTargetView(pUIRenderTarget.Get(), rgba);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
UIElements::~UIElements() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
void UIElements::Swap() {
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	pUISwapChain->Present(1u, 0);
}
SceneManager::SceneManager(int posX, int posY, int widthX, int widthY):posX(posX),posY(posY),widthX(widthX),widthY(widthY)
{
}
void SceneManager::SetSizenWidth() {
	ImGui::Begin("Scene Objects");
	ImGui::SetWindowSize(ImVec2((float)widthX, (float)widthY));
	ImGui::SetWindowPos(ImVec2((float)posX, (float)posY));
	ImGui::End();
}
std::vector<Objects*>::iterator SceneManager::LookUp(short id, std::vector<Objects*>& vec ) {
	for (auto it = currentScene->AllObject.begin();it!= currentScene->AllObject.end(); it++) {
		if ((*it)->Id == id)
			return it;
	}
	return currentScene->AllObject.end();
}
std::vector<Triangle*>::iterator SceneManager::LookUp(Triangle* Tri, std::vector<Triangle*>& vec) {
	for (auto it = currentScene->Triangles.begin(); it != currentScene->Triangles.end(); it++) {
		if ((*it)->id == Tri->id)
			return it;
	}
	return currentScene->Triangles.end();
}
void SceneManager::Content() {
	static int selected = -1;
	ImGui::Begin("Scene Objects",nullptr,ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize);
	if (ImGui::BeginPopupContextItem("Scene Menu")) {
		if (ImGui::Button("Add Object")) {
			/*Triangle* newTriangle = new Triangle();
			currentScene->AllObject.push_back(newTriangle);
			currentScene->Triangles.push_back(newTriangle);
			currentScene->CContoller->AddTriangle(newTriangle);
			currentScene->CContoller->InitalizePosition();*/
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	for (auto& Tri : currentScene->AllObject) {
		if (ImGui::Selectable((Tri->ObjName + std::to_string(Tri->Id)).c_str(), selected == Tri->Id)) {
			PropertiesWindow::Obj = Tri;
			selected = Tri->Id;
		}
		if (ImGui::BeginPopupContextItem(std::to_string(Tri->Id).c_str())) {
			if (ImGui::Button("Delete")) {
				Triangle* t = dynamic_cast<Triangle*>(Tri);
				Scene::globalCurrentOBJID.push_back(t->id);
				Objects::GlobalIdPool.push_back(t->Id);
				if(Globals::inPlayMode)
					currentScene->CContoller->deleteObject(t);
				currentScene->AllObject.erase(LookUp(Tri->Id,currentScene->AllObject));
				currentScene->Triangles.erase(LookUp(t, currentScene->Triangles));
				delete t;
				t = nullptr;
				ImGui::CloseCurrentPopup();
				PropertiesWindow::Obj = nullptr;
			}
			ImGui::EndPopup();
		}
	}
	if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
		ImGui::OpenPopup("Scene Menu");
	ImGui::End();
}
Objects* PropertiesWindow::Obj = nullptr;
PropertiesWindow::PropertiesWindow(int posX, int posY, int widthX, int widthY) :posX(posX), posY(posY), widthX(widthX), widthY(widthY)
{
}
void PropertiesWindow::SetSizenWidth() {
	ImGui::Begin("Properties");
	ImGui::SetWindowSize(ImVec2((float)widthX, (float)widthY));
	ImGui::SetWindowPos(ImVec2((float)posX, (float)posY));
	ImGui::End();
}
void PropertiesWindow::Content() {
	ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	if (PropertiesWindow::Obj != nullptr) {
		for (auto& obj : *PropertiesWindow::Obj->GetProperties()) {
			obj->show();
		}
	}
	ImGui::End();
}

void Files::SetSizenWidth()
{
	ImGui::Begin("Files");
	ImGui::SetWindowSize(ImVec2((float)widthX, (float)widthY));
	ImGui::SetWindowPos(ImVec2((float)posX, (float)posY));
	ImGui::End();
}

Files::Files(int posX, int posY, int widthX, int widthY, Microsoft::WRL::ComPtr<ID3D11Device> pDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext): posX(posX), posY(posY), widthX(widthX), widthY(widthY),pDevice(pDevice),pContext(pContext)
{
	
}

void Files::Content()
{
	ImGui::Begin("Files",nullptr,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	static fs::directory_entry fileSelected;
	if (working != WorkingDirectory && ImGui::Selectable(ICON_FA_FOLDER_O " ../")) {
		working = working.parent_path();
	}
	for (auto& it : fs::directory_iterator(working)) {
		if (it.is_directory()) {
			if (ImGui::Selectable((ICON_FA_FOLDER_O" "+ it.path().filename().string()).c_str(), fileSelected == it)) {
				fileSelected = it;
			}
		}
		else if (it.is_regular_file()) {
			if (ImGui::Selectable((ICON_FA_FILE_O" " + it.path().filename().string()).c_str(), fileSelected == it)) {
				fileSelected = it;
			}
			if (ImGui::BeginPopupContextItem(it.path().string().c_str())) {
				if (ImGui::Button("Add to scene")) {
					GetFile(it.path().string());
					std::vector<Vertex> newVert;
					std::vector<unsigned int> indi;
					std::vector<NormalPerObject> n;
					for (unsigned int i = 0; i < Vertexformated.size(); i++) {
						newVert.emplace_back(Vertexformated[i]);
						indi.push_back(i);
						if (i < ObjNormals.size())
							n.emplace_back(ObjNormals[i][0], ObjNormals[i][1], ObjNormals[i][2]);
					}
					float rgba[3] = {1.0f,0.0f,0.0f};
					Triangle* newTriangle; 
					short globalID;
					short ObjectID;
					if (!Objects::GlobalIdPool.empty()) {
						globalID = Objects::GlobalIdPool[0];
						Objects::GlobalIdPool.erase(Objects::GlobalIdPool.begin());
					}
					else {
						globalID = ++Objects::count;
					}
					if (!Scene::globalCurrentOBJID.empty()) {
						ObjectID = Scene::globalCurrentOBJID[0];
						Scene::globalCurrentOBJID.erase(Scene::globalCurrentOBJID.begin());
					}
					else {
						ObjectID = SceneManager::currentScene->currentOBJID++;
					}
					newTriangle = new Triangle(pDevice, pContext, newVert, indi, ObjectID, rgba,globalID,n);
					if (Globals::inPlayMode) {
						SceneManager::currentScene->CContoller->AddTriangle(newTriangle);
						SceneManager::currentScene->CContoller->InitalizePosition(newTriangle);
					}
					SceneManager::currentScene->AllObject.push_back(newTriangle);
					SceneManager::currentScene->Triangles.push_back(newTriangle);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
	}
	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
		if (fileSelected.is_directory()) {
			working = fileSelected;
		}
	}
	ImGui::End();
}
ControlMenu::ControlMenu(int posX, int posY, int widthX, int widthY) :posX(posX), posY(posY), widthX(widthX), widthY(widthY)
{
}
void ControlMenu::SetSizenWidth()
{
	ImGui::Begin("Control");
	ImGui::SetWindowSize(ImVec2((float)widthX, (float)widthY));
	ImGui::SetWindowPos(ImVec2((float)posX, (float)posY));
	ImGui::End();
}
void ControlMenu::Content() {
	ImGui::Begin("Control",nullptr,ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize);
	if (ImGui::Button(ICON_FA_PLAY"Play/Pause")) {
		Globals::inPlayMode = !Globals::inPlayMode;
		if (Globals::inPlayMode) {
			SceneManager::currentScene->InitalizePlayMode();
		}
	}
	ImGui::SameLine();
	ImGui::Text(("inPlayMode"+std::to_string(Globals::inPlayMode)).c_str());
	ImGui::End();
}