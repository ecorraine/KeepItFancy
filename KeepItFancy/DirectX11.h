#ifndef DIRECTX11_H
#define DIRECTX11_H

#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "DirectX_Files/DirectXTK/Lib/x64/Debug/DirectXTK.lib")
#pragma comment(lib,"dxguid.lib")
#include <dxgidebug.h>
#include <dxgi1_3.h>
#else
#pragma comment(lib, "DirectX_Files/DirectXTK/Lib/x64/Release/DirectXTK.lib")
#endif
#endif

#include "Application.h"

class RenderTarget;
class DepthStencil;

///--------------------------------------------------
//! \enum Raster Type
/*! \brief List of Fill Modes and Cull Modes
 *  \brief 使用する塗りつぶしまたは三角形描画モードの列挙型
 */
enum class RasterType
{
	SOLID_NO_CULL,			//!< 全ての三角形を描画
	CULL_FRONT,				//!< 正面向きの三角形を描画しない
	CULL_BACK,				//!< 背面向きの三角形を描画しない
	WIREFRAME_NO_CULL,		//!< 全ての線のみを描画
	WIREFRAME_CULL_BACK,	//!< 背面向きの線のみを描画しない
	MAX_RASTER_TYPE
};

///--------------------------------------------------
/// \enum DepthStencilState
/*! \brief List of DepthStencil States
 *  \brief 使用する深度ステンシルステートの列挙型
 */
enum class DepthStencilState
{
	DEPTH_STENCIL_DISABLED,	//!< 深度ステンシルステート
	DEPTH_STENCIL_READ,		//!< 深度ステンシル読み取りステート
	DEPTH_STENCIL_WRITE,	//!< 深度ステンシル書き込みステート
	MAX_DEPTHSTENCIL_STATE
};

///--------------------------------------------------
//! \enum Blend Type
/*! \brief List of Blending States
 *  \brief 使用するブレンドステートの列挙型
 */
enum class BlendType
{
	ALPHA,					//!< アルファ合成
	ADDITION,				//!< 加算合成
	ADDITION_APLHA,			//!< 加算(透過あり)合成
	SUBTRACTION,			//!< 減算合成
	SCREEN,					//!< スクリーン合成
	NONE,
	MAX_BLEND_TYPE
};

///--------------------------------------------------
//! \enum SamplerType
/*! \brief List of Sampling Types
 *  \brief 使用するサンプリングの列挙型
 */
enum class SamplerType
{
	POINT,					//!< ポイント サンプリング
	LINEAR,					//!< 線形補間
	ANISOTROPIC,			//!< 異方性補間
	MAX_SAMPLING_TYPE
};

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! DirectX11 Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class DirectX11 DirectX11.h "DirectX11.h"
/*! \brief Base Class for DirectX11.
 *  \brief
 */
class DirectX11
{
private:
	static APPLICATION* g_App;

	static ComPtr<ID3D11Device> g_d11Device;
	static ComPtr<ID3D11Debug> g_d11Debug;
	static ComPtr<IDXGISwapChain> g_dxSwapChain;
	static ComPtr<ID3D11DeviceContext> g_d11DeviceContext;

public:
	~DirectX11() {}

	static ID3D11Device* GetDevice() { return g_d11Device.Get(); }
	static ID3D11Debug* GetDebugDevice() { return g_d11Debug.Get(); }
	static IDXGISwapChain* GetSwapChain() { return g_dxSwapChain.Get(); }
	static ID3D11DeviceContext* GetContext() { return g_d11DeviceContext.Get(); }

	static ID3D11RasterizerState* g_d11RasterState[(int)RasterType::MAX_RASTER_TYPE];
	static ID3D11DepthStencilState* g_d11DepthStencilState[(int)DepthStencilState::MAX_DEPTHSTENCIL_STATE];
	static ID3D11BlendState* g_d11BlendState[(int)BlendType::MAX_BLEND_TYPE];
	static ID3D11SamplerState* g_d11SamplerState[(int)SamplerType::MAX_SAMPLING_TYPE];

	//static std::vector<ComPtr<ID3D11SamplerState>> test;

	static uint32_t GetScreenWidth() { return g_App->GetScreenWidth(); }
	static uint32_t GetScreenHeight() { return g_App->GetScreenHeight(); }

	static HRESULT InitializeDirectX(APPLICATION* pApp, bool isFullscreen);

	static void ReleaseDirectX()
	{
		for (int i = 0; i < (int)SamplerType::MAX_SAMPLING_TYPE; i++)
			SAFE_RELEASE(g_d11SamplerState[i]);
		for (int i = 0; i < (int)BlendType::MAX_BLEND_TYPE; i++)
			SAFE_RELEASE(g_d11BlendState[i]);
		for (int i = 0; i < (int)DepthStencilState::MAX_DEPTHSTENCIL_STATE; i++)
			SAFE_RELEASE(g_d11DepthStencilState[i]);
		for (int i = 0; i < (int)RasterType::MAX_RASTER_TYPE; i++)
			SAFE_RELEASE(g_d11RasterState[i]);

		//if (g_d11DeviceContext)
		//	g_d11DeviceContext->Release();

		if (g_dxSwapChain)
		{
			g_dxSwapChain->SetFullscreenState(false, NULL);
			g_dxSwapChain->Release();
		}

		if (g_d11Debug)
		{
			HRESULT hr = DirectX11::GetDebugDevice()->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			g_d11Debug->Release();
		}

		if (g_d11Device)
			g_d11Device->Release();
	}

	//! \fn static void SetRenderTargets(UINT num, RenderTarget** ppRTV, DepthStencil* pDSV)
	/*! \brief レンダーターゲット設定 */
	static void SetRenderTargets(UINT num, RenderTarget** ppRTV, DepthStencil* pDSV);

	//! \fn static void SetRasterState(RasterType eRasterType)
	/*! \brief Indicate the Fill Mode and Cull Mode to use
	 *  \brief 使用する塗りつぶしまたは三角形描画モードを設定
	 *  \param eRasterType : enum class RasterType
	 */
	static void SetRasterState(RasterType eRasterType)
	{
		switch (eRasterType)
		{
		case RasterType::SOLID_NO_CULL:
			GetContext()->RSSetState(g_d11RasterState[(int)RasterType::SOLID_NO_CULL]);
			break;
		case RasterType::CULL_FRONT:
			GetContext()->RSSetState(g_d11RasterState[(int)RasterType::CULL_FRONT]);
			break;
		case RasterType::CULL_BACK:
			GetContext()->RSSetState(g_d11RasterState[(int)RasterType::CULL_BACK]);
			break;
		case RasterType::WIREFRAME_NO_CULL:
			GetContext()->RSSetState(g_d11RasterState[(int)RasterType::WIREFRAME_NO_CULL]);
			break;
		case RasterType::WIREFRAME_CULL_BACK:
			GetContext()->RSSetState(g_d11RasterState[(int)RasterType::WIREFRAME_CULL_BACK]);
		default:
			RasterType::SOLID_NO_CULL;
			break;
		}
	}

	//! \fn static void SetDepthStencilState(DepthStencilState eDepthStencilState)
	/*! \brief Indicate the DepthStencil State to use
	 *  \brief 使用する深度ステンシルステートを設定
	 *  \param eDepthStencilState : enum class DepthStencilState
	 */
	static void SetDepthStencilState(DepthStencilState eDepthStencilState)
	{
		switch (eDepthStencilState)
		{
		case DepthStencilState::DEPTH_STENCIL_DISABLED:
			GetContext()->OMSetDepthStencilState(g_d11DepthStencilState[(int)DepthStencilState::DEPTH_STENCIL_DISABLED], 0);
			break;
		case DepthStencilState::DEPTH_STENCIL_READ:
			GetContext()->OMSetDepthStencilState(g_d11DepthStencilState[(int)DepthStencilState::DEPTH_STENCIL_READ], 0);
			break;
		case DepthStencilState::DEPTH_STENCIL_WRITE:
			GetContext()->OMSetDepthStencilState(g_d11DepthStencilState[(int)DepthStencilState::DEPTH_STENCIL_WRITE], 0);
			break;
		default:
			DepthStencilState::DEPTH_STENCIL_DISABLED;
			break;
		}
	}

	//! \fn static void SetBlendState(BlendType eBlendType)
	/*! \brief Indicate the Blending State to use
	 *  \brief 使用するブレンドステートを設定
	 *  \param eBlendType : enum class BlendType
	 */
	static void SetBlendState(BlendType eBlendType)
	{
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		UINT sampleMask = 0xffffffff;
		GetContext()->OMSetBlendState(g_d11BlendState[(int)eBlendType], blendFactor, sampleMask);
	}

	static void ClearBlendState()
	{
		float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		UINT sampleMask = 0xffffffff;
		GetContext()->OMSetBlendState(nullptr, blendFactor, sampleMask);
	}

	//! \fn static void SetSamplerState(SamplerType eFilterType)
	/*! \brief Indicate the Sampling State to use
	 *  \brief 使用するサンプリングを設定
	 *  \param eFilterType : enum class SamplerType
	 */
	static void SetSamplerState(SamplerType eFilterType)
	{
		switch (eFilterType)
		{
		case SamplerType::POINT:
			GetContext()->PSSetSamplers(0, 1, &g_d11SamplerState[(int)SamplerType::POINT]);
			break;
		case SamplerType::LINEAR:
			GetContext()->PSSetSamplers(0, 1, &g_d11SamplerState[(int)SamplerType::LINEAR]);
			break;
		case SamplerType::ANISOTROPIC:
			GetContext()->PSSetSamplers(0, 1, &g_d11SamplerState[(int)SamplerType::ANISOTROPIC]);
			break;
		default:
			SamplerType::POINT;
			break;
		}
	}

	static void Swap() { GetSwapChain()->Present(0, 0); }	// (0, 0) = VSync Off
};

#endif // !DIRECTX11_H
