#ifndef DIRECTX11_H
#define DIRECTX11_H

#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "DirectX_Files/DirectXTK/Lib/x64/Debug/DirectXTK.lib")
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
	WIREFRAME_CULL_BACK,	//!< 背面向きの線のみを描画
	MAX_RASTERTYPE
};

///--------------------------------------------------
//! \enum Blend Type
/*! \brief List of Blending States
 *  \brief 使用するブレンドステートの列挙型
 */
enum class BlendType
{
	NORMAL,				//!< アルファ合成
	ADDITION,			//!< 加算合成
	ADDITION_APLHA,		//!< 加算(透過あり)合成
	SUBTRACTION,		//!< 減算合成
	SCREEN,				//!< スクリーン合成
	NONE,
	MAX_BLENDTYPE
};

///--------------------------------------------------
//! \enum FilterType
/*! \brief List of Sampling Types
 *  \brief 使用するサンプリングの列挙型
 */
enum class FilterType
{
	POINT,				//!< ポイント サンプリング
	LINEAR,				//!< 線形補間
	ANISOTROPIC,		//!< 異方性補間
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

	static ID3D11Device* g_d11Device;
	static IDXGISwapChain* g_dxSwapChain;
	static ID3D11DeviceContext* g_d11DeviceContext;

public:
	static ID3D11Device* GetDevice() { return g_d11Device; }
	static IDXGISwapChain* GetSwapChain() { return g_dxSwapChain; }
	static ID3D11DeviceContext* GetContext() { return g_d11DeviceContext; }

	static ID3D11RasterizerState* g_d11RasterState[(int)RasterType::MAX_RASTERTYPE];
	static ID3D11BlendState* g_d11BlendState[(int)BlendType::MAX_BLENDTYPE];
	static ID3D11SamplerState* g_d11SamplerState[(int)FilterType::MAX_SAMPLING_TYPE];

	static uint32_t GetScreenWidth() { return g_App->GetScreenWidth(); }
	static uint32_t GetScreenHeight() { return g_App->GetScreenHeight(); }

	static HRESULT InitializeDirectX(APPLICATION* pApp, bool isFullscreen);

	static void ReleaseDirectX()
	{
		for (int i = 0; i < (int)FilterType::MAX_SAMPLING_TYPE; i++)
			SAFE_RELEASE(g_d11SamplerState[i]);
		for (int i = 0; i < (int)BlendType::MAX_BLENDTYPE; i++)
			SAFE_RELEASE(g_d11BlendState[i]);
		for (int i = 0; i < (int)RasterType::MAX_RASTERTYPE; i++)
			SAFE_RELEASE(g_d11RasterState[i]);

		SAFE_RELEASE(g_d11DeviceContext);
		g_dxSwapChain->SetFullscreenState(false, NULL);
		SAFE_RELEASE(g_dxSwapChain);
		SAFE_RELEASE(g_d11Device);
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

	//! \fn static void SetBlendState(BlendType eBlendType)
	/*! \brief Indicate the Blending State to use
	 *  \brief 使用するブレンドステートを設定
	 *  \param eBlendType : enum class BlendType
	 */
	static void SetBlendState(BlendType eBlendType)
	{
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		GetContext()->OMSetBlendState(g_d11BlendState[(int)eBlendType], blendFactor, 0xffffffff);
	}

	//! \fn static void SetSamplerState(FilterType eFilterType)
	/*! \brief Indicate the Sampling State to use
	 *  \brief 使用するサンプリングを設定
	 *  \param eFilterType : enum class FilterType
	 */
	static void SetSamplerState(FilterType eFilterType)
	{
		switch (eFilterType)
		{
		case FilterType::POINT:
			GetContext()->PSSetSamplers(0, 1, &g_d11SamplerState[(int)FilterType::POINT]);
			break;
		case FilterType::LINEAR:
			GetContext()->PSSetSamplers(0, 1, &g_d11SamplerState[(int)FilterType::LINEAR]);
			break;
		case FilterType::ANISOTROPIC:
			GetContext()->PSSetSamplers(0, 1, &g_d11SamplerState[(int)FilterType::ANISOTROPIC]);
			break;
		default:
			FilterType::POINT;
			break;
		}
	}

	static void Swap() { GetSwapChain()->Present(0, 0); }
};

#endif // !DIRECTX11_H
