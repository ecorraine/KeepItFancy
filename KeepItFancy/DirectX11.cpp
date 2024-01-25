#include "DirectX11.h"

#pragma comment(lib,"dxgi.lib")

#include "Texture.h"

APPLICATION* DirectX11::g_App;

ComPtr<ID3D11Device> DirectX11::g_d11Device;
ComPtr<ID3D11Debug> DirectX11::g_d11Debug;
ComPtr<IDXGISwapChain> DirectX11::g_dxSwapChain;
ComPtr<ID3D11DeviceContext> DirectX11::g_d11DeviceContext;

ID3D11RasterizerState* DirectX11::g_d11RasterState[(int)RasterType::MAX_RASTER_TYPE];
ID3D11DepthStencilState* DirectX11::g_d11DepthStencilState[(int)DepthStencilState::MAX_DEPTHSTENCIL_STATE];
ID3D11BlendState* DirectX11::g_d11BlendState[(int)BlendType::MAX_BLEND_TYPE];
ID3D11SamplerState* DirectX11::g_d11SamplerState[(int)SamplerType::MAX_SAMPLING_TYPE];

HRESULT DirectX11::InitializeDirectX(APPLICATION* pApp, bool isFullscreen)
{
	g_App = pApp;

	HRESULT hr = E_FAIL;
	D3D_FEATURE_LEVEL level;

	ComPtr<IDXGIFactory> factory = nullptr;
	ComPtr<IDXGIAdapter> adapter = nullptr;
	ComPtr<IDXGIOutput> adapterOutput = nullptr;	// monitor
	unsigned int numerator = 60;					// 分子
	unsigned int denominator = 1;					// 分母
	//unsigned int numModes;
	//DXGI_MODE_DESC* displayModeList;

	// ドライバの種類
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,	// GPU Rendering
		D3D_DRIVER_TYPE_WARP,		// Windows Advanced Rasterizer
		D3D_DRIVER_TYPE_REFERENCE,	// CPU Rendering
		D3D_DRIVER_TYPE_SOFTWARE,
	};
	unsigned int numDriverTypes = ARRAYSIZE(driverTypes);

	unsigned int deviceFlag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	deviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	// 機能レベル
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// create a DirectX graphics interface factory
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf());
	if (FAILED(hr))
		throw hr;

	// use the factory to create an adpter for the primary graphics interface(video card)
	hr = factory->EnumAdapters(0, adapter.GetAddressOf());
	if (FAILED(hr))
		throw hr;

	// enumerrate primary adapter output(monitor)
	hr = adapter->EnumOutputs(0, adapterOutput.GetAddressOf());
	if (FAILED(hr))
		throw hr;

	/*
	// get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output(monitor)
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(hr))
		throw hr;

	// create alist to hold all possible display modes for this monitor/video card combination
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
		throw E_FAIL;

	// now fill the display mode list structures
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(hr))
		throw hr;

	// now go through all the display modes and find the one that matches the screen width and height
	// when a match is found store the numerator and denominator of the refresh rate for that monitor
	for (unsigned int i = 0; i < numModes; i++) {
		if (displayModeList[i].Width == g_App->GetScreenWidth()) {
			if (displayModeList[i].Height == g_App->GetScreenHeight()) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	delete[] displayModeList;
	displayModeList = NULL;

	if (adapterOutput)
		adapterOutput->Release();

	if (adapter)
		adapter->Release();

	if (factory)
		factory->Release();
	*/

	//--------------------------------------------------
	// Swap Chain
	// スワップチェインとは、ウインドウへの表示ダブルバッファを管理する
	// クラス　マルチサンプリング、リフレッシュレートが設定できる
	//--------------------------------------------------
	DXGI_SWAP_CHAIN_DESC scDesc = {};
	ZeroMemory(&scDesc, sizeof(scDesc));
	scDesc.BufferDesc.Width = g_App->GetScreenWidth();
	scDesc.BufferDesc.Height = g_App->GetScreenHeight();
	scDesc.BufferDesc.RefreshRate.Numerator = numerator;				// リフレッシュレート（分母）
	scDesc.BufferDesc.RefreshRate.Denominator = denominator;			// リフレッシュレート（分子）
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// バックバッファフォーマット(R,G,B,A 範囲０.０から１.０)
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.SampleDesc.Count = 1;										// マルチサンプルの数
	scDesc.SampleDesc.Quality = 0;										// マルチサンプルのクオリティ
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;				// バックバッファの使用方法
	scDesc.BufferCount = 1;												// バックバッファの数
	scDesc.OutputWindow = g_App->GetWindow();							// 関連付けるウインドウ
	scDesc.Windowed = isFullscreen ? FALSE : TRUE;						// ウインドウモードを設定
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;				// モードの自動切り替え

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		//m_DriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain
		(
			NULL,							// ディスプレイデバイスのアダプタ（ＮＵＬＬの場合最初に見つかったアダプタ）
			driverTypes[driverTypeIndex],	// デバイスドライバのタイプ
			NULL,							// ソフトウェアラスタライザを使用する場合に指定する
			deviceFlag,
			featureLevels,					// 機能レベル
			numFeatureLevels,
			D3D11_SDK_VERSION,				// 
			&scDesc,						// スワップチェインの設定
			g_dxSwapChain.GetAddressOf(),
			g_d11Device.GetAddressOf(),
			&level,							// サポートされている機能レベル
			g_d11DeviceContext.GetAddressOf()
		);
		if (SUCCEEDED(hr))
		{
			g_d11Device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(g_d11Debug.GetAddressOf()));
			break;
		}
	}
	if (FAILED(hr))
	{
		MessageBoxA(g_App->GetWindow(), "Failed to create Device & Swap Chain!\nデバイスとスワップチェーン作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
		throw hr;
	}

	//--------------------------------------------------
	// Texture Sampler
	//--------------------------------------------------
	D3D11_SAMPLER_DESC samplerDesc = {};
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	D3D11_FILTER filter[] = {
		D3D11_FILTER_MIN_MAG_MIP_POINT,		// point sampling
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,	// linear interpolation
		D3D11_FILTER_ANISOTROPIC			// anisotropic interpolation
	};
	// create individual sampler state per filter type
	for (int i = 0; i < (int)SamplerType::ANISOTROPIC; i++)
	{
		samplerDesc.Filter = filter[i];
		hr = g_d11Device->CreateSamplerState(&samplerDesc, &g_d11SamplerState[i]);

	}

	// create for anisotropic filter
	samplerDesc.Filter = filter[(int)SamplerType::ANISOTROPIC];
	//samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g_d11Device->CreateSamplerState(&samplerDesc, &g_d11SamplerState[(int)SamplerType::ANISOTROPIC]);

	if (FAILED(hr))
	{
		MessageBoxA(g_App->GetWindow(), "Failed to create Texture Sampler State!\nサンプラーステート作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
		throw hr;
	}

	// initialize default sampler state
	SetSamplerState(SamplerType::LINEAR);

	//--------------------------------------------------
	// Blend State
	//--------------------------------------------------
	D3D11_BLEND_DESC blendDesc = {};
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// create individual blend state per blend type
	for (int i = 0; i < (int)BlendType::MAX_BLEND_TYPE; i++)
	{
		switch (i)
		{
		case (int)BlendType::ALPHA:			// alpha blending
		{
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			hr = g_d11Device->CreateBlendState(&blendDesc, &g_d11BlendState[(int)BlendType::ALPHA]);
			if (FAILED(hr))
			{
				MessageBoxA(g_App->GetWindow(), "Failed to create NORMAL Blend State!\nブレンドステート作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
				throw hr;
			}
		}
		break;
		case (int)BlendType::ADDITION:			// addition blending
		{
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			hr = g_d11Device->CreateBlendState(&blendDesc, &g_d11BlendState[(int)BlendType::ADDITION]);
			if (FAILED(hr))
			{
				MessageBoxA(g_App->GetWindow(), "Failed to create ADDITION Blend State!\nブレンドステート作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
				throw hr;
			}
		}
		break;
		case (int)BlendType::ADDITION_APLHA:	// addition with alpha blending
		{
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			hr = g_d11Device->CreateBlendState(&blendDesc, &g_d11BlendState[(int)BlendType::ADDITION_APLHA]);
			if (FAILED(hr))
			{
				MessageBoxA(g_App->GetWindow(), "Failed to create ADDITION_APLHA Blend State!\nブレンドステート作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
				throw hr;
			}
		}
		break;
		case (int)BlendType::SUBTRACTION:		// subtraction blending
		{
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
			hr = g_d11Device->CreateBlendState(&blendDesc, &g_d11BlendState[(int)BlendType::SUBTRACTION]);
			if (FAILED(hr))
			{
				MessageBoxA(g_App->GetWindow(), "Failed to create SUBTRACTION Blend State!\nブレンドステート作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
				throw hr;
			}
		}
		break;
		case (int)BlendType::SCREEN:			// screen blending
		{
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_DEST_COLOR;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			hr = g_d11Device->CreateBlendState(&blendDesc, &g_d11BlendState[(int)BlendType::SCREEN]);
			if (FAILED(hr))
			{
				MessageBoxA(g_App->GetWindow(), "Failed to create SCREEN Blend State!\nブレンドステート作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
				throw hr;
			}
		}
		break;
		case (int)BlendType::NONE:
		default:								// 保険処理
		{
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
			hr = g_d11Device->CreateBlendState(&blendDesc, &g_d11BlendState[(int)BlendType::NONE]);
			if (FAILED(hr))
			{
				MessageBoxA(g_App->GetWindow(), "Failed to create Blend State!\nブレンドステート作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
				throw hr;
			}
		}
		break;
		}
	}
	// initialize default bland state
	SetBlendState(BlendType::ALPHA);

	//--------------------------------------------------
	// Rasterizer
	// initialize the raster description which will determine how and what polygons will be drawn
	//--------------------------------------------------
	D3D11_RASTERIZER_DESC rasterDesc = {};
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.AntialiasedLineEnable = false;

	// culling types
	D3D11_CULL_MODE culling[] = {
	D3D11_CULL_NONE,					// no culling: draw all triangles
	D3D11_CULL_FRONT,					// do not draw front-facing triangles
	D3D11_CULL_BACK	 					// do not draw back-facing triangles
	};
	// create individual raster state per culling type
	for (int i = 0; i <= (int)RasterType::CULL_BACK; i++)
	{
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.CullMode = culling[i];
		hr = GetDevice()->CreateRasterizerState(&rasterDesc, &g_d11RasterState[i]);
	}
	// create wireframe type fill
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;

	rasterDesc.CullMode = D3D11_CULL_NONE;
	hr = GetDevice()->CreateRasterizerState(&rasterDesc, &g_d11RasterState[3]);

	rasterDesc.CullMode = D3D11_CULL_BACK;
	hr = GetDevice()->CreateRasterizerState(&rasterDesc, &g_d11RasterState[4]);
	if (FAILED(hr))
	{
		MessageBoxA(g_App->GetWindow(), "Failed to create Raster State!\nラスタライザー作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
		throw hr;
	}

	// initialize the default rasterizer state
	SetRasterState(RasterType::SOLID_NO_CULL);

	//--------------------------------------------------
	// Depth Stencil State
	//--------------------------------------------------
	// https://tositeru.github.io/ImasaraDX11/part/ZBuffer-and-depth-stencil
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	// front facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
	// back facing
	dsDesc.BackFace = dsDesc.FrontFace;

	// create individual depth stencil state
	bool dsPattern[] = { false, true, true };
	D3D11_DEPTH_WRITE_MASK maskPattern[] =
	{
		D3D11_DEPTH_WRITE_MASK_ZERO,
		D3D11_DEPTH_WRITE_MASK_ZERO,
		D3D11_DEPTH_WRITE_MASK_ALL
	};
	for (int i = 0; i < (int)DepthStencilState::MAX_DEPTHSTENCIL_STATE; ++i)
	{
		dsDesc.DepthEnable = dsDesc.StencilEnable = dsPattern[i];
		dsDesc.DepthWriteMask = maskPattern[i];
		hr = g_d11Device->CreateDepthStencilState(&dsDesc, &g_d11DepthStencilState[i]);
		if (FAILED(hr)) { return hr; }
	}
	// initialize default depth stencil state
	SetDepthStencilState(DepthStencilState::DEPTH_STENCIL_WRITE);

	return hr;
}

void DirectX11::SetRenderTargets(UINT num, RenderTarget** ppRTV, DepthStencil* pDSV)
{
	static ComPtr<ID3D11RenderTargetView> rtvs[4] = { nullptr };
	if (num > 4)
	{
		num = 4;
	}
	for (UINT i = 0; i < num; ++i)
	{
		rtvs[i] = ppRTV[i]->GetRTV();
	}
	g_d11DeviceContext->OMSetRenderTargets(num, rtvs->GetAddressOf(), pDSV ? pDSV->GetDSV() : nullptr);

	// ビューポートの設定
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (float)ppRTV[0]->GetTexWidth();
	viewport.Height = (float)ppRTV[0]->GetTexHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	g_d11DeviceContext->RSSetViewports(1, &viewport);
}
