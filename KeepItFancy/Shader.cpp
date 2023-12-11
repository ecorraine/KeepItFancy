#include "Shader.h"
#include "Buffers.h"
#include "myUtilities.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SHADER
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HRESULT SHADER::LoadShader(const char* fileName)
{
	HRESULT hr = E_FAIL;

	FILE* fp = nullptr;
	fopen_s(&fp, fileName, "rb");	// read file in binary: disables special handling of '\n' and '\x1A'
	if (!fp) { return hr; }

	// check file size | ファイルのサイズを調べる
	fseek(fp, 0, SEEK_END);
	int fileSize = 0;
	fileSize = ftell(fp);

	std::string extname = GetFileExtension(fileName);
	char* pData = nullptr;
	if (extname == "cso")
	{
		// | メモリに読み込み
		fseek(fp, 0, SEEK_SET);
		pData = new char[fileSize];
		fread(pData, fileSize, 1, fp);
		fclose(fp);

	}
	else
	{
		hr = CompileShader(fileName);
	}

	// create buffer |定数バッファ作成
	hr = CreateBuffer(pData, fileSize);
	if (pData)
	{
		delete[] pData;
	}

	return hr;
}

HRESULT SHADER::CreateBuffer(void* pData, UINT fileSize, unsigned int stride)
{
	HRESULT hr = E_FAIL;

	// 解析用のリフレクター作成
	ComPtr<ID3D11ShaderReflection> cpReflection = nullptr;
	hr = D3DReflect(pData, fileSize, IID_PPV_ARGS(cpReflection.GetAddressOf()));
	if (FAILED(hr)) { return hr; }

	// 定数バッファ作成
	D3D11_SHADER_DESC shaderDesc = {};
	ZeroMemory(&shaderDesc, sizeof(shaderDesc));

	cpReflection->GetDesc(&shaderDesc);
	m_pBuffers.resize(shaderDesc.ConstantBuffers, nullptr);
	for (unsigned int i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		// シェーダーの定数バッファの情報を取得
		ID3D11ShaderReflectionConstantBuffer* cpBuf = cpReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC shaderBufDesc = {};
		ZeroMemory(&shaderBufDesc, sizeof(shaderBufDesc));

		cpBuf->GetDesc(&shaderBufDesc);
		switch (shaderBufDesc.Type)
		{
		case _D3D_CBUFFER_TYPE::D3D_CT_CBUFFER:
			hr = BUFFERS::CreateBuffer(BufferType::ConstantBuf, &m_pBuffers[i], shaderBufDesc.Size);
			break;
		case _D3D_CBUFFER_TYPE::D3D_CT_RESOURCE_BIND_INFO:
			/*
			// シェーダーインプットの情報を取得
			D3D11_SHADER_INPUT_BIND_DESC sibDesc = {};
			hr = cpReflection->GetResourceBindingDescByName(shaderBufDesc.Name, &sibDesc);
			switch (sibDesc.Type)
			{
			case _D3D_SHADER_INPUT_TYPE::D3D_SIT_STRUCTURED:
				hr = BUFFERS::CreateBuffer(BufferType::StructuredBuf, &m_pBuffers[i], shaderBufDesc.Size);
				break;
			case _D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWSTRUCTURED:
				hr = BUFFERS::CreateBuffer(BufferType::StructuredBuf, &m_pBuffers[i], 48, true);
				break;
			}
			*/
			break;
		}

		/*
		const char* SHADER_INPUT_TYPE[] = {
			"D3D_SIT_CBUFFER",
			"D3D_SIT_TBUFFER",
			"D3D_SIT_TEXTURE",
			"D3D_SIT_SAMPLER",
			"D3D_SIT_UAV_RWTYPED",
			"D3D_SIT_STRUCTURED",
			"D3D_SIT_UAV_RWSTRUCTURED",
			"D3D_SIT_BYTEADDRESS",
			"D3D_SIT_UAV_RWBYTEADDRESS",
			"D3D_SIT_UAV_APPEND_STRUCTURED",
			"D3D_SIT_UAV_CONSUME_STRUCTURED",
			"D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER",
			"D3D_SIT_RTACCELERATIONSTRUCTURE",
			"D3D_SIT_UAV_FEEDBACKTEXTURE"
		};
		//std::cout << "Found '" << sibDesc.Name << "' bound as " << SHADER_INPUT_TYPE[sibDesc.Type] << " in slot ("<< i << ")\n" << std::endl;
		*/

		if (FAILED(hr)) { return hr; }
	}

	return CreateShader(pData, fileSize);
}

HRESULT SHADER::CompileShader(const char* fileName)
{
	HRESULT hr = E_FAIL;
	ComPtr<ID3DBlob> cpShaderBlob = nullptr;
	ComPtr<ID3DBlob> cpErrorBlob = nullptr;

	static const char* pTargetList[] =
	{
		"vs_5_0",
		"hs_5_0",
		"ds_5_0",
		"gs_5_0",
		"ps_5_0",
		"cs_5_0"
	};

	UINT compileFlag = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	compileFlag |= D3DCOMPILE_DEBUG;
#endif // _DEBUG

	wchar_t wFileName[512];
	setlocale(LC_ALL, "Japanese_Japan.932");
	mbstowcs_s(nullptr, wFileName, 512, fileName, _TRUNCATE);	// char -> wcharに変換

	hr = D3DCompileFromFile(
		wFileName,							// wFileName LPCWST pFileName
		nullptr,							// D3D_SHADER_MACRO *pDefines
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// ID3DInclude *pInclude
		"main",								// LPCSTR pEntrypoint
		pTargetList[m_eShaderType],			// LPCSTR pTarget
		compileFlag,						// UINT Flags1
		0,									// UINT Flags2
		&cpShaderBlob,						// ID3DBlob** ppCode
		&cpErrorBlob);						// ID3DBlob** ppErrorMsg
	
	/*
	hr = D3DCompile(
		fileName,
		strlen(fileName),
		nullptr,
		nullptr,
		nullptr,
		"main",
		pTargetList[m_eShaderType],
		compileFlag,
		0,
		&cpShaderBlob,
		&cpErrorBlob);
	*/
	
	if (FAILED(hr))
	{
		if (cpErrorBlob)
		{
			// Output the error message
			//OutputDebugStringA(static_cast<const char*>(cpErrorBlob->GetBufferPointer()));
			LogToFile(static_cast<const char*>(cpErrorBlob->GetBufferPointer()));
			cpErrorBlob->Release();
		}
		return hr;
	}

	// シェーダ作成
	hr = CreateShader(cpShaderBlob->GetBufferPointer(), (UINT)cpShaderBlob->GetBufferSize());
	cpShaderBlob->Release();

	return hr;
}

void SHADER::SendToBuffer(UINT bufferSlot, void* pData)
{
	if (bufferSlot < m_pBuffers.size())
		DirectX11::GetContext()->UpdateSubresource(m_pBuffers[bufferSlot], 0, nullptr, pData, 0, 0);
}

void SHADER::MapToBuffer(UINT bufferSlot, void* pData)
{
	if (bufferSlot < m_pBuffers.size())
	{
		D3D11_MAPPED_SUBRESOURCE data = {};
		HRESULT hr = DirectX11::GetContext()->Map(m_pBuffers[bufferSlot], 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
		if (SUCCEEDED(hr))
		{
			const rsize_t dataSize = sizeof(pData);
			memcpy_s(data.pData, data.RowPitch, &pData, dataSize);
			DirectX11::GetContext()->Unmap(m_pBuffers[bufferSlot], 0);
		}
	}
}

void SHADER::SetSRV(UINT bufferSlot, ID3D11ShaderResourceView* pSRV)
{
	if (!pSRV) { return; }
	ComPtr<ID3D11ShaderResourceView> cpSRV = pSRV;
	switch (m_eShaderType)
	{
	case VertexS:
		DirectX11::GetContext()->VSSetShaderResources(bufferSlot, 1, cpSRV.GetAddressOf());
		break;
	case HullS:
		DirectX11::GetContext()->HSSetShaderResources(bufferSlot, 1, cpSRV.GetAddressOf());
		break;
	case DomainS:
		DirectX11::GetContext()->DSSetShaderResources(bufferSlot, 1, cpSRV.GetAddressOf());
		break;
	case PixelS:
		DirectX11::GetContext()->PSSetShaderResources(bufferSlot, 1, cpSRV.GetAddressOf());
		break;
	case ComputeS:
		DirectX11::GetContext()->CSSetShaderResources(bufferSlot, 1, cpSRV.GetAddressOf());
		break;
	}
	cpSRV->Release();
}

//--------------------------------------------------
// Vertex Shader
//--------------------------------------------------
HRESULT VertexShader::CreateShader(void* pData, UINT fileSize)
{
	HRESULT hr = E_FAIL;

	hr = DirectX11::GetDevice()->CreateVertexShader(pData, fileSize, nullptr, m_d11VertexShader.GetAddressOf());
	if (FAILED(hr)) { return hr; }

	// シェーダ作成時にシェーダリフレクションを通してインプットレイアウトを取得
	// セマンティクスの配置などから識別子を作成
	// 識別子が登録済→再利用、なければ新規作成
	// https://blog.techlab-xe.net/dxc-shader-reflection/
	ComPtr<ID3D11ShaderReflection> cpReflection = nullptr;
	hr = D3DReflect(pData, fileSize, IID_PPV_ARGS(cpReflection.GetAddressOf()));
	if (FAILED(hr)) { return hr; }

	D3D11_SHADER_DESC shaderDesc = {};
	ZeroMemory(&shaderDesc, sizeof(shaderDesc));
	cpReflection->GetDesc(&shaderDesc);

	D3D11_INPUT_ELEMENT_DESC* pInputLayout = {};
	pInputLayout = new D3D11_INPUT_ELEMENT_DESC[shaderDesc.InputParameters];

	DXGI_FORMAT formats[][4] =
	{
		{
			DXGI_FORMAT_R32_UINT,
			DXGI_FORMAT_R32G32_UINT,
			DXGI_FORMAT_R32G32B32_UINT,
			DXGI_FORMAT_R32G32B32A32_UINT,
		}, {
			DXGI_FORMAT_R32_SINT,
			DXGI_FORMAT_R32G32_SINT,
			DXGI_FORMAT_R32G32B32_SINT,
			DXGI_FORMAT_R32G32B32A32_SINT,
		}, {
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_R32G32B32_FLOAT,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
		},
	};

	D3D11_SIGNATURE_PARAMETER_DESC sigDesc = {};
	ZeroMemory(&sigDesc, sizeof(sigDesc));
	for (unsigned int i = 0; i < shaderDesc.InputParameters; ++i)
	{
		cpReflection->GetInputParameterDesc(i, &sigDesc);
		pInputLayout[i].SemanticName = sigDesc.SemanticName;
		pInputLayout[i].SemanticIndex = sigDesc.SemanticIndex;

		// http://marupeke296.com/TIPS_No17_Bit.html
		BYTE elementCount = sigDesc.Mask;
		elementCount = (elementCount & 0x05) + ((elementCount >> 1) & 0x05);
		elementCount = (elementCount & 0x03) + ((elementCount >> 2) & 0x03);

		switch (sigDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			pInputLayout[i].Format = formats[0][elementCount - 1];
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			pInputLayout[i].Format = formats[1][elementCount - 1];
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			pInputLayout[i].Format = formats[2][elementCount - 1];
			break;
		}
		pInputLayout[i].InputSlot = 0;
		pInputLayout[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
		pInputLayout[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayout[i].InstanceDataStepRate = 0;
	}

	hr = DirectX11::GetDevice()->CreateInputLayout(pInputLayout, shaderDesc.InputParameters, pData, fileSize, m_d11InputLayout.GetAddressOf());

	delete[] pInputLayout;
	//cpReflection->Release();

	return hr;
}

void VertexShader::BindShader()
{
	DirectX11::GetContext()->VSSetShader(m_d11VertexShader.Get(), NULL, 0);
	DirectX11::GetContext()->IASetInputLayout(m_d11InputLayout.Get());
	for (int i = 0; i < m_pBuffers.size(); i++)
		DirectX11::GetContext()->VSSetConstantBuffers(i, 1, &m_pBuffers[i]);
}

//--------------------------------------------------
// Hull Shader
//--------------------------------------------------
HRESULT HullShader::CreateShader(void* pData, UINT fileSize)
{
	return DirectX11::GetDevice()->CreateHullShader(pData, fileSize, nullptr, m_d11HullShader.GetAddressOf());
}

void HullShader::BindShader()
{
	DirectX11::GetContext()->HSSetShader(m_d11HullShader.Get(), nullptr, 0);
	for (int i = 0; i < m_pBuffers.size(); i++)
		DirectX11::GetContext()->HSSetConstantBuffers(i, 1, &m_pBuffers[i]);
}

//--------------------------------------------------
// Domain Shader
//--------------------------------------------------
HRESULT DomainShader::CreateShader(void* pData, UINT fileSize)
{
	return DirectX11::GetDevice()->CreateDomainShader(pData, fileSize, nullptr, m_d11DomainShader.GetAddressOf());
}

void DomainShader::BindShader()
{
	DirectX11::GetContext()->DSSetShader(m_d11DomainShader.Get(), nullptr, 0);
	for (int i = 0; i < m_pBuffers.size(); i++)
		DirectX11::GetContext()->DSSetConstantBuffers(i, 1, &m_pBuffers[i]);
}

//--------------------------------------------------
// Pixel Shader
//--------------------------------------------------
HRESULT PixelShader::CreateShader(void* pData, UINT fileSize)
{
	return DirectX11::GetDevice()->CreatePixelShader(pData, fileSize, nullptr, m_d11PixelShader.GetAddressOf());
}

void PixelShader::BindShader()
{
	DirectX11::GetContext()->PSSetShader(m_d11PixelShader.Get(), nullptr, 0);
	for (int i = 0; i < m_pBuffers.size(); i++)
		DirectX11::GetContext()->PSSetConstantBuffers(i, 1, &m_pBuffers[i]);
}

//--------------------------------------------------
// Compute Shader
//--------------------------------------------------
HRESULT ComputeShader::CreateShader(void* pData, UINT fileSize)
{
	return DirectX11::GetDevice()->CreateComputeShader(pData, fileSize, nullptr, m_d11ComputeShader.GetAddressOf());
}

void ComputeShader::BindUAV(UINT bufferSlot, ID3D11UnorderedAccessView** pUAV)
{
	DirectX11::GetContext()->CSSetUnorderedAccessViews(bufferSlot, 1, pUAV, 0);
}

void ComputeShader::BindShader()
{
	DirectX11::GetContext()->CSSetShader(m_d11ComputeShader.Get(), nullptr, 0);
	//DirectX11::GetContext()->Dispatch(4, 1, 1);
	for (int i = 0; i < m_pBuffers.size(); i++)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		m_pBuffers[i]->GetDesc(&bufferDesc);
		if (bufferDesc.MiscFlags == 0)
			DirectX11::GetContext()->CSSetConstantBuffers(i, 1, &m_pBuffers[i]);
	}
}

