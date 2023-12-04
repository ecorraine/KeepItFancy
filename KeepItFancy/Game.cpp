#include "Game.h"
#include "Texture.h"
#include "RootScene.h"

std::shared_ptr<RootScene> g_pScene;

void GAME::InitGame(APPLICATION* pApp)
{
	HRESULT hr;
	//----- DirectX11の初期化処理を呼び出し -----//
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

	// シーンを作成して初期化
	g_pScene = std::make_shared<RootScene>();
	g_pScene->Init();

	// ビュー（ＲＴＶとＤＳＶ）を作成してビューポートに送る
	auto rtv = g_pScene->CreateObj<RenderTarget>("RTV");
	rtv->CreateRTVFromScreen();
	auto dsv = g_pScene->CreateObj<DepthStencil>("DSV");
	hr = dsv->CreateDSV(false);
	DirectX11::SetRenderTargets(1, &rtv, dsv);
}

void GAME::ReleaseGame()
{
	g_pScene->Release();
	g_pScene.reset();

	KEYINPUT::ReleaseKeyInput();

	DirectX11::ReleaseDirectX();
}

void GAME::UpdateGame(float tick)
{
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
