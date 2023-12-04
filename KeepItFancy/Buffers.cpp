#include "Buffers.h"

///--------------------------------------------------
//! Create Constant Buffer
///--------------------------------------------------
HRESULT CreateConstantBuffer(unsigned int bytesize, ID3D11Buffer** ppBuffer, bool isDynamic)
{
	HRESULT hr = E_FAIL;

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc = {};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = bytesize;
	bufferDesc.Usage = isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;								// 定数バッファ
	bufferDesc.CPUAccessFlags = isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;

	hr = DirectX11::GetDevice()->CreateBuffer(&bufferDesc, nullptr, ppBuffer);
	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "Failed to create Constant Buffer!\n定数バッファ作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
		throw hr;
	}
	return hr;
}

///--------------------------------------------------
//! Create Index Buffer
///--------------------------------------------------
HRESULT CreateIndexBuffer(unsigned int bytesize, void* pData, ID3D11Buffer** ppBuffer)
{
	HRESULT hr = E_FAIL;

	// インデックスバッファ生成
	D3D11_BUFFER_DESC bufferDesc = {};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(unsigned int) * bytesize;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;				// インデックスバッファ
	bufferDesc.CPUAccessFlags = 0;									// 0 = CPU access not necessary
	bufferDesc.MiscFlags = 0;

	// バッファの初期値指定
	D3D11_SUBRESOURCE_DATA initialData = {};
	ZeroMemory(&initialData, sizeof(initialData));
	initialData.pSysMem = pData;
	//initialData.SysMemPitch = 0;								// 頂点バッファでは使用しない
	//initialData.SysMemSlicePitch = 0;

	hr = DirectX11::GetDevice()->CreateBuffer(&bufferDesc, &initialData, ppBuffer);
	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "Failed to create Index Buffer!\nインデックスバッファ作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
		throw hr;
	}
	return hr;
}

///--------------------------------------------------
//! Create Vertex Buffer
///--------------------------------------------------
HRESULT CreateVertexBuffer(unsigned int bytesize, void* pData, ID3D11Buffer** ppBuffer, bool isDynamic)
{
	HRESULT hr = E_FAIL;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bufferDesc = {};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = bytesize;
	bufferDesc.Usage = isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;									// 頂点バッファ
	bufferDesc.CPUAccessFlags = isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	bufferDesc.MiscFlags = 0;

	// バッファの初期値指定
	D3D11_SUBRESOURCE_DATA initialData = {};
	ZeroMemory(&initialData, sizeof(initialData));
	initialData.pSysMem = pData;
	initialData.SysMemPitch = 0;														// 頂点バッファでは使用しない
	initialData.SysMemSlicePitch = 0;

	hr = DirectX11::GetDevice()->CreateBuffer(&bufferDesc, &initialData, ppBuffer);
	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "Failed to create Vertex Buffer!\n頂点バッファ作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
		throw hr;
	}
	return hr;
}

HRESULT CreateVertexBufferUAV(unsigned int bytesize, void* pData, ID3D11Buffer** ppBuffer)
{
	HRESULT hr = E_FAIL;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bufferDesc = {};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = bytesize;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;		// 頂点バッファ（ＵＡＶ）
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	// バッファの初期値指定
	D3D11_SUBRESOURCE_DATA initialData = {};
	ZeroMemory(&initialData, sizeof(initialData));
	initialData.pSysMem = pData;
	initialData.SysMemPitch = 0;														// 頂点バッファでは使用しない
	initialData.SysMemSlicePitch = 0;

	hr = DirectX11::GetDevice()->CreateBuffer(&bufferDesc, &initialData, ppBuffer);
	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "Failed to create Vertex Buffer(UAV)!\n頂点バッファ(UAV)作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
		throw hr;
	}
	return hr;
}

///--------------------------------------------------
//! Create Structured Buffer
///--------------------------------------------------
HRESULT CreateStructuredBuffer(unsigned int stride, unsigned int num, void* pData, ID3D11Buffer** ppBuffer, bool isDynamic)
{
	HRESULT hr = E_FAIL;

	// 構造バッファ生成
	D3D11_BUFFER_DESC bufferDesc = {};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = stride * num;
	bufferDesc.Usage = isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = isDynamic ? D3D11_BIND_SHADER_RESOURCE : D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.CPUAccessFlags = isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = stride;

	// バッファの初期値指定
	if (pData != nullptr)
	{
		D3D11_SUBRESOURCE_DATA initialData = {};
		ZeroMemory(&initialData, sizeof(initialData));
		initialData.pSysMem = pData;
		initialData.SysMemPitch = 0;
		initialData.SysMemSlicePitch = 0;

		hr = DirectX11::GetDevice()->CreateBuffer(&bufferDesc, &initialData, ppBuffer);
	}
	else
		hr = DirectX11::GetDevice()->CreateBuffer(&bufferDesc, nullptr, ppBuffer);

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "Failed to create Structured Buffer!\nＵＡ構造バッファ作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
		throw hr;
	}
	return hr;
}

ID3D11Buffer* CreateStagingBuffer(ID3D11Buffer* pBuffer)
{
	HRESULT hr = E_FAIL;
	ID3D11Buffer* CloneBuffer = nullptr;

	// Structuredバッファ生成
	D3D11_BUFFER_DESC bufferDesc = {};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	pBuffer->GetDesc(&bufferDesc);

	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	bufferDesc.Usage = D3D11_USAGE_STAGING;
	bufferDesc.BindFlags = 0;
	bufferDesc.MiscFlags = 0;

	hr = DirectX11::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &CloneBuffer);
	if (SUCCEEDED(hr))
	{
		DirectX11::GetContext()->CopyResource(CloneBuffer, pBuffer);
	}

	return CloneBuffer;
}



HRESULT CreateUnorderAccessView(ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppUAV)
{
	HRESULT hr = E_FAIL;

	D3D11_BUFFER_DESC bufferDesc = {};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	pBuffer->GetDesc(&bufferDesc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC  uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;

	if (bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		uavDesc.Buffer.NumElements = bufferDesc.ByteWidth / 4;
	}
	else if (bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer.NumElements = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
	}
	else
	{
		return hr;
	}

	hr = DirectX11::GetDevice()->CreateUnorderedAccessView(pBuffer, &uavDesc, ppUAV);
	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "Failed to create Unordered Access View!\nＵＡＶ作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
		throw hr;
	}

	return hr;
}

HRESULT CreateShaderResourceView(ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRV)
{
	HRESULT hr = E_FAIL;

	D3D11_BUFFER_DESC bufferDesc = {};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	pBuffer->GetDesc(&bufferDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC  srvDesc = {};
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;

	if (bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS) {
		srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		srvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
		srvDesc.BufferEx.NumElements = bufferDesc.ByteWidth / 4;
	}
	else if (bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED) {
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.BufferEx.NumElements = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
	}
	else
	{
		return hr;
	}

	hr = DirectX11::GetDevice()->CreateShaderResourceView(pBuffer, &srvDesc, ppSRV);
	if (FAILED(hr)) {
		MessageBoxA(nullptr, "Failed to create Shader Resource View!\nＳＲＶ作成失敗！", "ERROR", MB_OK | MB_ICONERROR);
		throw hr;
	}

	return hr;
}

