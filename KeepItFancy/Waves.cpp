#include "Waves.h"

void Waves::BindVertices(sRGBA color)
{
	m_Vertices.clear();

	for (unsigned int y = 0; y <= m_iDivY; y++) {
		for (unsigned int x = 0; x <= m_iDivX; x++) {
			VERTEX vtx = {};
			vtx.pos.x = -m_fWidth / 2.0f + x * m_fWidth / m_iDivX;
			vtx.pos.y = 0.0f;
			vtx.pos.z = -m_fDepth / 2.0f + y * m_fDepth / m_iDivY;

			vtx.uv.x = static_cast<float>(x) / static_cast<float>(m_iDivX);
			vtx.uv.y = static_cast<float>(y) / static_cast<float>(m_iDivY);

			vtx.color = { color.r, color.g, color.b, color.a };

			vtx.normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			m_Vertices.emplace_back(vtx);
		}
	}
}

void Waves::Create(float width, float depth, int divX, int divY, sRGBA color)
{
	m_fWidth = width;
	m_fDepth = depth;

	// 分割数を保存
	m_iDivX = divX;
	m_iDivY = divY;

	BindVertices(color);
	BindIndices();

	m_pCS = AddComponent<ComputeShader>();
	m_pCS->LoadShader(SHADER_PATH("CS_NoiseWave.cso"));

	CreateDefaultBuffers();
	LoadDefaultShaders();

	// Create staging buffer for reading back data from GPU;
	CHECK_HR(CreateStagingBuffer(sizeof(VERTEX) * m_Vertices.size(), m_cpStagingBuffer.GetAddressOf()));

	// Set up shader resources and UAV
	CHECK_HR(CreateVertexBufferUAV(sizeof(VERTEX) * m_Vertices.size(), m_Vertices.data(), pOutputBuffer.GetAddressOf()));
	CHECK_HR(CreateUnorderAccessView(pOutputBuffer.Get(), pOutputBufferUAV.GetAddressOf()));

	const rsize_t vertexSize = m_Vertices.size();
	XMFLOAT4 structOffsetSize = { offsetof(VERTEX, pos), offsetof(VERTEX, uv), offsetof(VERTEX, color), offsetof(VERTEX, normal) };
	cbData[0] = structOffsetSize;
	cbData[1] = { static_cast<float>(m_iDivX), static_cast<float>(m_iDivY), static_cast<float>(m_iDivZ), static_cast<float>(vertexSize) };
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
	cbData[2] = { tick, 0.0f, 0.0f, 0.0f };
	m_pCS->SendToBuffer(0, &cbData);
	m_pPS->SendToBuffer(0, &cbData[2]);
}
