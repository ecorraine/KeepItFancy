#include "Texture.h"
#include "DirectX_Files/DirectXTex/Inc/DirectXTex.h"

HRESULT TEXTURE::CreateTexture(const char* pFileName)
{
	HRESULT hr = E_FAIL;

	// 文字変換
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;
	MultiByteToWideChar(0, 0, pFileName, -1, wPath, MAX_PATH);

	// ファイル別読み込み
	DirectX::TexMetadata	mdata;
	DirectX::ScratchImage	image;
	if (strstr(pFileName, ".tga"))
		hr = DirectX::LoadFromTGAFile(wPath, &mdata, image);
	else
		hr = DirectX::LoadFromWICFile(wPath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &mdata, image);
	if (FAILED(hr)) {
		static char buf[256];
		sprintf_s(buf, sizeof(buf), "Failed to load Texture File!\nテクスチャ読み込み失敗！\n %ls", wPath);
		MessageBoxA(nullptr, buf, "ERROR", MB_OK | MB_ICONWARNING);
		return hr;
	}

	// シェーダリソース生成
	hr = CreateShaderResourceView(DirectX11::GetDevice(), image.GetImages(), image.GetImageCount(), mdata, m_cpSRV.GetAddressOf());
	if (SUCCEEDED(hr))
	{
		m_width = (UINT)mdata.width;
		m_height = (UINT)mdata.height;
	}
	return hr;
}

HRESULT TEXTURE::CreateTexture(const void* _pData)
{
	D3D11_TEXTURE2D_DESC texDesc = CreateTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT);

	return CreateResource(texDesc, _pData);
}

D3D11_TEXTURE2D_DESC TEXTURE::CreateTextureDesc(DXGI_FORMAT _format)
{
	//---------- Create a Texture Resource ----------//
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width		= DirectX11::GetScreenWidth();
	texDesc.Height		= DirectX11::GetScreenHeight();
	texDesc.MipLevels	= 1;
	texDesc.ArraySize	= 1;
	texDesc.Format		= _format;
	// default sampler mode, with no anti - aliasing, has a count of 1 and a quality level of 0
	texDesc.SampleDesc	= { 1, 0 };
	texDesc.Usage		= D3D11_USAGE_DEFAULT;
	texDesc.BindFlags	= D3D11_BIND_SHADER_RESOURCE;

	return texDesc;
}

HRESULT TEXTURE::CreateResource(D3D11_TEXTURE2D_DESC& texDesc, const void* pData)
{
	HRESULT hr = E_FAIL;

	// テクスチャ作成
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = pData;
	data.SysMemPitch = texDesc.Width * 4;
	hr = DirectX11::GetDevice()->CreateTexture2D(&texDesc, pData ? &data : nullptr, m_cpTextureBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to create Texture Buffer!\nテクスチャバッファ作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
		return hr;
	}

	//---------- Create a Shader Resource View ----------//
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	switch (texDesc.Format)
	{
	default:
		srvDesc.Format = texDesc.Format;
		break;
	case DXGI_FORMAT_R32_TYPELESS:
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		break;
	}
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// 生成
	hr = DirectX11::GetDevice()->CreateShaderResourceView(m_cpTextureBuffer.Get(), &srvDesc, m_cpSRV.GetAddressOf());
	if (SUCCEEDED(hr))
	{
		m_width = texDesc.Width;
		m_height = texDesc.Height;
	}
	return hr;
}


///--------------------------------------------------
/// Render Target View
///--------------------------------------------------
HRESULT RenderTarget::CreateRTV()
{
	D3D11_TEXTURE2D_DESC texDesc = CreateTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT);
	texDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;

	return CreateResource(texDesc);
}

HRESULT RenderTarget::CreateRTVFromScreen()
{
	HRESULT hr = E_FAIL;

	// バックバッファのポインタを取得
	ComPtr<ID3D11Texture2D> cpBackBuffer = nullptr;
	hr = DirectX11::GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)m_cpTextureBuffer.GetAddressOf());
	if (FAILED(hr)) { return hr; }

	//---------- Create Render Target View Using Back Buffer----------//
	// バックバッファへのポインタを指定してレンダーターゲットビューを作成
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.Texture2D.MipSlice = 0;
	hr = DirectX11::GetDevice()->CreateRenderTargetView(m_cpTextureBuffer.Get(), &rtvDesc, m_cpRTV.GetAddressOf());
	if (SUCCEEDED(hr))
	{
		D3D11_TEXTURE2D_DESC desc = {};
		m_cpTextureBuffer->GetDesc(&desc);
		m_width = desc.Width;
		m_height = desc.Height;
	}
	return hr;
}

void RenderTarget::Clear(const float* color)
{
	DirectX11::GetContext()->ClearRenderTargetView(m_cpRTV.Get(), color);
}

HRESULT RenderTarget::CreateResource(D3D11_TEXTURE2D_DESC& texDesc, const void* pData)
{
	HRESULT hr = TEXTURE::CreateResource(texDesc, nullptr);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Failed to create Texture Resource!\nテクスチャリソース作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
		return hr;
	}

	//---------- Create Render Target View ----------//
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	return DirectX11::GetDevice()->CreateRenderTargetView(m_cpTextureBuffer.Get(), &rtvDesc, m_cpRTV.GetAddressOf());
}


///--------------------------------------------------
/// Depth Stencil View
///--------------------------------------------------
HRESULT DepthStencil::CreateDSV(bool useStencil)
{
	D3D11_TEXTURE2D_DESC texDesc = CreateTextureDesc(useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS);
	texDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;

	return CreateResource(texDesc);
}

void DepthStencil::Clear()
{
	DirectX11::GetContext()->ClearDepthStencilView(
		m_cpDSV.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,										// Ｚバッファを1.0でクリアする(0.0f～1.0f)
		0);											// ステンシルバッファを0でクリアする
}

HRESULT DepthStencil::CreateResource(D3D11_TEXTURE2D_DESC& texDesc, const void* pData)
{
	bool useStencil = (texDesc.Format == DXGI_FORMAT_R24G8_TYPELESS);
	texDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;

	HRESULT hr = TEXTURE::CreateResource(texDesc, nullptr);
	if (FAILED(hr)) { return hr; }

	//---------- Create Depth Stencil View ----------//
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	return DirectX11::GetDevice()->CreateDepthStencilView(m_cpTextureBuffer.Get(), &dsvDesc, m_cpDSV.GetAddressOf());
}
