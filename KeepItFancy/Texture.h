#ifndef TEXTURE_H
#define TEXTURE_H

#include "DirectX11.h"
#include "Component.h"

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "DirectX_Files/DirectXTex/Lib/x64/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "DirectX_Files/DirectXTex/Lib/x64/Release/DirectXTex.lib")
#endif
#endif

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! TEXTURE Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class TEXTURE Texture.h "Texture.h"
/*! \brief Base Class for Texture.
 *  \brief テクスチャ
 */
class TEXTURE : public COMPONENT
{
protected:
	ComPtr<ID3D11ShaderResourceView>	m_cpSRV = nullptr;
	ComPtr<ID3D11Texture2D>				m_cpTextureBuffer = nullptr;
	uint32_t							m_width;
	uint32_t							m_height;

public:
	TEXTURE() :
		m_width(0),
		m_height(0)
	{}
	virtual ~TEXTURE()
	{
		//SAFE_RELEASE(m_cpTextureBuffer);
		//SAFE_RELEASE(m_cpSRV);
	}

	ID3D11ShaderResourceView* GetSRV() const { return m_cpSRV.Get(); };
	HRESULT CreateTexture(const char* pFileName);
	HRESULT CreateTexture(const void* _pData = nullptr);

	uint32_t GetTexWidth() { return m_width; }
	uint32_t GetTexHeight() { return m_height; }

protected:
	D3D11_TEXTURE2D_DESC CreateTextureDesc(DXGI_FORMAT _format);
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& texDesc, const void* pData);
};


///--------------------------------------------------
//! Render Target View
///--------------------------------------------------
//! \class Render Target View Texture.h "Texture.h"
/*! \brief Base Class for Render Target View.
 *  \brief レンダーターゲットビュー
 */
class RenderTarget : public TEXTURE
{
private:
	ComPtr<ID3D11RenderTargetView>		m_cpRTV = nullptr;

public:
	RenderTarget() {}
	~RenderTarget() {}

	ID3D11RenderTargetView* GetRTV() const { return m_cpRTV.Get(); }
	HRESULT CreateRTV();
	HRESULT CreateRTVFromScreen();

	void Clear(const float* color);

protected:
	HRESULT CreateResource(D3D11_TEXTURE2D_DESC& texDesc, const void* pData = nullptr);
};


///--------------------------------------------------
//! Depth Stencil View
///--------------------------------------------------
//! \class Depth Stencil View Texture.h "Texture.h"
/*! \brief Base Class for Depth Stencil View.
 *  \brief デプスステンシルビュー
 */
class DepthStencil : public TEXTURE
{
private:
	ComPtr<ID3D11DepthStencilView>		m_cpDSV = nullptr;

public:
	DepthStencil() {}
	~DepthStencil() {}

	ID3D11DepthStencilView* GetDSV() const { return m_cpDSV.Get(); }
	HRESULT CreateDSV(bool useStencil);

	void Clear();

protected:
	HRESULT CreateResource(D3D11_TEXTURE2D_DESC& texDesc, const void* pData = nullptr);
};

#endif // !TEXTURE_H