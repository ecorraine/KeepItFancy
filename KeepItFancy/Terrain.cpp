#include "Terrain.h"

void Terrain::Create(float width, float depth, int divX, int divY)
{
	useXZAxis = true;

	m_fWidth = width;
	m_fDepth = depth;

	// 分割数を保存
	m_iDivX = divX;
	m_iDivY = divY;

	SetBaseSRV(ASSET_PATH("img/ValueNoise_Color.png"));

	BindVertices();
	BindIndices();

	CreateDefaultBuffers();
	LoadDefaultShaders();

	// Create staging buffer for reading back data from GPU;
	CHECK_HR(CreateStagingBuffer(sizeof(VERTEX) * m_Vertices.size(), m_cpStagingBuffer.GetAddressOf()));

	// Set up shader resources and UAV
	CHECK_HR(CreateVertexBufferUAV(sizeof(VERTEX) * m_Vertices.size(), m_Vertices.data(), m_cpOutputBufferUAV.GetAddressOf()));
	CHECK_HR(CreateUnorderAccessView(m_cpOutputBufferUAV.Get(), m_cpUAV.GetAddressOf()));

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

	SetBaseColor(sRGBA(218, 96, 20));
	m_colorOverlay = sRGBA(0.0f, 1.0f, 0.0f);
}

void Terrain::BindComputeShaders()
{
	MESH::BindComputeShaders();
	m_pCS->SetSRV(0, m_cpBaseSRV.Get());

	DirectX11::GetContext()->Dispatch(m_Vertices.size(), 1, 1);

	DirectX11::GetContext()->CopyResource(m_cpStagingBuffer.Get(), m_cpOutputBufferUAV.Get());

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

void Terrain::Update(float tick)
{
	cbData[0] = { tick, m_fFrequency, m_fAmplitude, 0.0f };

	m_pCS->SendToBuffer(0, &cbData);

	MESH::Update(tick);
}

void Terrain::SendDatatoBuffer()
{
	m_pPS->SendToBuffer(2, (void*)&m_colorOverlay);
}

void Terrain::ProcessTessellation(void* tessData)
{
	MESH::ProcessTessellation(tessData);

	m_pHS->SetSRV(0, m_cpHeightMapSRV.Get());
	m_pDS->SetSRV(0, m_cpHeightMapSRV.Get());
}
