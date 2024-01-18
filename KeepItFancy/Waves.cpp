#include "Waves.h"

void Waves::Create(float width, float depth, int divX, int divY)
{
	useXZAxis = true;

	m_fWidth = width;
	m_fDepth = depth;

	// 分割数を保存
	m_iDivX = divX;
	m_iDivY = divY;

	BindVertices();
	BindIndices();

	CreateDefaultBuffers();
	LoadDefaultShaders();

	// Create staging buffer for reading back data from GPU;
	CHECK_HR(CreateStagingBuffer(sizeof(VERTEX) * m_Vertices.size(), m_cpStagingBuffer.GetAddressOf()));

	// Set up shader resources and UAV
	CHECK_HR(CreateVertexBufferUAV(sizeof(VERTEX) * m_Vertices.size(), m_Vertices.data(), pOutputBuffer.GetAddressOf()));
	CHECK_HR(CreateUnorderAccessView(pOutputBuffer.Get(), pOutputBufferUAV.GetAddressOf()));

	cbData[1] = {
		offsetof(VERTEX, pos),
		offsetof(VERTEX, uv),
		offsetof(VERTEX, color),
		offsetof(VERTEX, normal)
	};

	const rsize_t vertexSize = m_Vertices.size();
	cbData[2] = {
		static_cast<float>(m_iDivX),
		static_cast<float>(m_iDivY),
		static_cast<float>(m_iDivZ),
		static_cast<float>(vertexSize)
	};
}

void Waves::BindComputeShaders()
{
	m_pCS->BindShader();
	DirectX11::GetContext()->CSSetUnorderedAccessViews(0, 1, pOutputBufferUAV.GetAddressOf(), 0);

	DirectX11::GetContext()->Dispatch(m_Vertices.size(), 1, 1);

	DirectX11::GetContext()->CopyResource(m_cpStagingBuffer.Get(), pOutputBuffer.Get());

	std::vector<VERTEX> UpdatedVertices;
	UpdatedVertices.resize(m_Vertices.size());
	D3D11_MAPPED_SUBRESOURCE resource;
	DirectX11::GetContext()->Map(m_cpStagingBuffer.Get(), 0, D3D11_MAP_READ, 0, &resource);
	if (resource.pData)
	{
		memcpy(UpdatedVertices.data(), resource.pData, sizeof(VERTEX) * m_Vertices.size());
	}
	DirectX11::GetContext()->Unmap(m_cpStagingBuffer.Get(), 0);
	UpdatedVertices.clear();
	
	DirectX11::GetContext()->CopyResource(m_cpVertexBuf.Get(), m_cpStagingBuffer.Get());
}

void Waves::Update(float tick)
{
	cbData[0] = { tick, 0.0f, 0.0f, 0.0f };

	XMFLOAT4 data[] = {
		{ m_color.r, m_color.g, m_color.b, m_color.a },
		cbData[0]
	};

	m_pCS->SendToBuffer(0, &cbData);
	m_pPS->SendToBuffer(1, &data);
}
