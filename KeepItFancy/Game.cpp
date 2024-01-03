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

	DirectX11::ReleaseDirectX();

#ifdef _DEBUG
	ComPtr<IDXGIDebug> debugDev = nullptr;
	CHECK_HR(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debugDev)));
	CHECK_HR(debugDev->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL));
#endif // _DEBUG
}

void GAME::UpdateGame(float tick)
{
	g_fElapsedTime += TIME::GetFrameTime() * 0.001f;
	TIME::CalculateFrames();
	FPS::CountFrames();
	CPU::CountFrames();
	printf_s("%d FPS | CPU Usage: %d | Run Time: %.2f secs | %.3f (ms)\n", FPS::GetFPSCount(), CPU::GetCpuPercentage(),  g_fElapsedTime, tick * 1000);

	g_pScene->_update(tick);

	KEYINPUT::UpdateKeyInput();
}

void GAME::DrawGame()
{
	auto rtv = g_pScene->GetObj<RenderTarget>("RTV");
	auto dsv = g_pScene->GetObj<DepthStencil>("DSV");

	float color[4] = { 0.0f, 0.0f, 0.1f, 1.0f };
	rtv->Clear(color);
	dsv->Clear();

	g_pScene->_draw();

	DirectX11::Swap();
}
