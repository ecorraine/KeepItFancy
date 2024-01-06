#include "Game.h"
#include "Profiler.h"
#include "Texture.h"
#include "RootScene.h"

std::shared_ptr<RootScene> g_pScene;

void GAME::InitGame(APPLICATION* pApp)
{
	HRESULT hr;
	// Initialize DirectX
	try
	{
		hr = DirectX11::InitializeDirectX(pApp, false);
	}
	catch (HRESULT hr)
	{
		hr;
		MessageBoxA(NULL, "Failed to initialize DirectX11.\nDirectX11の初期化に失敗。", "ERROR", MB_OK | MB_ICONERROR);
		throw hr;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(pApp->GetWindow());
	ImGui_ImplDX11_Init(DirectX11::GetDevice(), DirectX11::GetContext());

	KEYINPUT::InitKeyInput();

	TIME::InitTimer();
	FPS::InitFPS();
	CPU::InitCPU();

	// initialize scene
	g_pScene = std::make_shared<RootScene>();
	g_pScene->Init();

	// create RTV and DSV then send them to viewport
	auto rtv = g_pScene->CreateObj<RenderTarget>("RTV");
	rtv->CreateRTVFromScreen();
	auto dsv = g_pScene->CreateObj<DepthStencil>("DSV");
	hr = dsv->CreateDSV(false);
	DirectX11::SetRenderTargets(1, &rtv, dsv);
}

void GAME::ReleaseGame()
{
	CPU::EndQuery();

	g_pScene->Release();
	g_pScene.reset();

	KEYINPUT::ReleaseKeyInput();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	DirectX11::ReleaseDirectX();

#ifdef _DEBUG
	ComPtr<IDXGIDebug> debugDev = nullptr;
	CHECK_HR(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debugDev)));
	CHECK_HR(debugDev->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL));
#endif // _DEBUG
}

void GAME::UpdateGame(float tick)
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	g_fElapsedTime += TIME::GetFrameTime() * 0.001f;
	TIME::CalculateFrames();
	FPS::CountFrames();
	CPU::CountFrames();
	ImGui::Begin("System");
	ImGui::Text("%d FPS | CPU Usage: %d \nRun Time: %.2f secs | %.3f (ms)\n", FPS::GetFPSCount(), CPU::GetCpuPercentage(), g_fElapsedTime, tick * 1000);
	ImGui::End();
	g_pScene->_update(tick);

	KEYINPUT::UpdateKeyInput();
}

void GAME::DrawGame()
{
	auto rtv = g_pScene->GetObj<RenderTarget>("RTV");
	auto dsv = g_pScene->GetObj<DepthStencil>("DSV");

	ImGui::Render();

	float color[4] = { 0.0f, 0.0f, 0.1f, 1.0f };
	rtv->Clear(color);
	dsv->Clear();

	g_pScene->_draw();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// Update and Render additional Platform Windows
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	DirectX11::Swap();
}
