#ifndef BUFFERS_H
#define BUFFERS_H

#include "Renderer.h"

//! \enum Buffer Type
/*! \brief Buffer Type to be classified on create.
 *  \brief 作るバッファの種類
 */
enum class BufferType
{
	ConstantBuf,
	IndexBuf,
	VertexBuf,
	VertexUAV,
	StructuredBuf
};

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! BUFFERS
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Buffers.h "Buffers.h"
/*! \brief Template Class for Buffers.
 *  \brief
 */
class BUFFERS
{
public:
	BUFFERS() = delete;
	virtual ~BUFFERS() { }

	static HRESULT CreateBuffer(BufferType eBufferType, ID3D11Buffer** ppBuffer, unsigned int stride, bool isDynamic = false, unsigned int num = 1, void* pData = nullptr)
	{
		HRESULT hr = E_FAIL;

		D3D11_BUFFER_DESC bufferDesc = {};
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));


		switch (eBufferType)
		{
		case BufferType::ConstantBuf:
			bufferDesc.ByteWidth = stride;
			bufferDesc.Usage = isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
			break;
		case BufferType::IndexBuf:
			bufferDesc.ByteWidth = sizeof(unsigned int) * stride;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			break;
		case BufferType::VertexBuf:
			bufferDesc.ByteWidth = stride * num;
			bufferDesc.Usage = isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
			bufferDesc.MiscFlags = 0;
			break;
		case BufferType::VertexUAV:
			bufferDesc.ByteWidth = stride * num;
			bufferDesc.Usage = isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
			bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
			break;
		case BufferType::StructuredBuf:
			bufferDesc.ByteWidth = stride * num;
			bufferDesc.Usage = isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
			bufferDesc.BindFlags = isDynamic ? D3D11_BIND_SHADER_RESOURCE : D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			bufferDesc.CPUAccessFlags = isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
			bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			bufferDesc.StructureByteStride = stride;
			break;
		default:
			BufferType::ConstantBuf;
			break;
		}

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
		{
			hr = DirectX11::GetDevice()->CreateBuffer(&bufferDesc, nullptr, ppBuffer);
		}

		return hr;
	}
};

///--------------------------------------------------
//! Create Constant Buffer
///--------------------------------------------------
//! \fn HRESULT CreateConstantBuffer(unsigned int bytesize, void* pData, ID3D11Buffer** ppBuffer)
/*! \brief Creates a default or dynamic Constant Buffer
 *  \param bytesize		: size of the buffer to use
 *  \param ppBuffer		: the buffer
 *  \param isDynamic	: to create as dynamic or default
 *  \return HRESULT
 */
HRESULT CreateConstantBuffer
(unsigned int bytesize, ID3D11Buffer** ppBuffer, bool isDynamic = true);



///--------------------------------------------------
//! Create Index Buffer
///--------------------------------------------------
//! \fn HRESULT CreateIndexBuffer(unsigned int bytesize, void* pData, ID3D11Buffer** ppBuffer)
/*! \brief Creates an Index Buffer
 *  \param bytesize		: size of the buffer
 *  \param pData		: the data
 *  \param ppBuffer		: the buffer
 *  \return HRESULT
 */
HRESULT CreateIndexBuffer
(unsigned int bytesize, void* pData, ID3D11Buffer** ppBuffer);



///--------------------------------------------------
//! Create Vertex Buffer
///--------------------------------------------------
//! \fn HRESULT CreateVertexBuffer (unsigned int bytesize, void* pData, ID3D11Buffer** ppBuffer, bool isDynamic)
/*! \brief Creates a default or dynamic Vertex Buffer
 *  \param bytesize		: size of the buffer to use [(sizeof(struct) * vertexnum]
 *  \param pData		: the data
 *  \param ppBuffer		: the buffer
 *  \param isDynamic	: to create as dynamic or default
 *  \return HRESULT
 */
HRESULT CreateVertexBuffer
(unsigned int bytesize, void* pData, ID3D11Buffer** ppBuffer, bool isDynamic = true);

//! \fn HRESULT CreateVertexBufferUAV (unsigned int bytesize, void* pData, ID3D11Buffer** ppBuffer)
/*! \brief Creates a UAV Vertex Buffer
 *  \param bytesize		: size of the buffer to use [(sizeof(struct) * vertexnum]
 *  \param pData		: the data
 *  \param ppBuffer		: the buffer
 *  \return HRESULT
 */
HRESULT CreateVertexBufferUAV
(unsigned int bytesize, void* pData, ID3D11Buffer** ppBuffer);



///--------------------------------------------------
//! Create Structured Buffer
///--------------------------------------------------
//! \fn HRESULT CreateStructuredBuffer (unsigned int stride, unsigned int num, void* pData, ID3D11Buffer** ppBuffer, bool isDynamic)
/*! \brief Creates a default or dynamic Structured Buffer
 *  \param stride		: sizeof(struct)
 *  \param num			: vertex num
 *  \param pData		: the data
 *  \param ppBuffer		: the buffer
 *  \param isDynamic	: to create as dynamic or default
 *  \return HRESULT
 */
HRESULT CreateStructuredBuffer
(unsigned int stride, unsigned int num, void* pData, ID3D11Buffer** ppBuffer, bool isDynamic = true);



///--------------------------------------------------
//! Create Staging Buffer
///--------------------------------------------------
//! \fn ID3D11Buffer* CreateStagingBuffer(ID3D11Buffer* pBuffer)
/*! \brief Copy Structured Buffer for staging
 *  \param pBuffer	: the Structured Buffer to copy
 *  \return ID3D11Buffer*
 */
ID3D11Buffer* CreateStagingBuffer(ID3D11Buffer* pBuffer);



///--------------------------------------------------
//! Create Views
///--------------------------------------------------

HRESULT CreateUnorderAccessView(ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppUAV);

HRESULT CreateShaderResourceView(ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRV);



#endif // !BUFFERS_H
