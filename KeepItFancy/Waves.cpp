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
	MESH::BindIndices();

	m_pCS = AddComponent<ComputeShader>();
	m_pCS->LoadShader(SHADER_PATH("CS_GerstnerWaves.cso"));

	CHECK_HR(CreateVertexBuffer(sizeof(VERTEX) * m_Vertices.size(), m_Vertices.data(), m_cpVertexBuf.GetAddressOf()));
	CHECK_HR(CreateIndexBuffer(m_Faces.size() * 3, m_Faces.data(), m_cpIndexBuf.GetAddressOf()));

	m_pVS = AddComponent<VertexShader>();
	m_pVS->LoadShader(SHADER_PATH("VS_WorldPosition.cso"));

	m_pPS = AddComponent<PixelShader>();
	m_pPS->LoadShader(SHADER_PATH("PS_HalfLambert.cso"));

	// Create staging buffer for reading back data from GPU;
	CHECK_HR(CreateStagingBuffer(sizeof(VERTEX) * m_Vertices.size(), m_cpStagingBuffer.GetAddressOf()));

	// Set up shader resources and UAV
	//CHECK_HR(CreateStructuredBuffer(sizeof(VERTEX), m_Vertices.size(), m_Vertices.data(), &pInputBuffer));
	//CHECK_HR(CreateShaderResourceView(pInputBuffer.Get(), &pInputBufferSRV));

	CHECK_HR(CreateVertexBufferUAV(sizeof(VERTEX) * m_Vertices.size(), m_Vertices.data(), pOutputBuffer.GetAddressOf()));
	CHECK_HR(CreateUnorderAccessView(pOutputBuffer.Get(), pOutputBufferUAV.GetAddressOf()));

	XMFLOAT4 structOffsetSize = { offsetof(VERTEX, pos), offsetof(VERTEX, color), offsetof(VERTEX, normal), sizeof(VERTEX) };
	cbData[0] = structOffsetSize;
	cbData[1] = { static_cast<float>(m_iDivX), static_cast<float>(m_iDivY), static_cast<float>(m_iDivZ), static_cast<float>(m_Vertices.size()) };
}

void Waves::BindComputeShaders()
{
	m_pCS->BindShader();
	//DirectX11::GetContext()->CSSetShaderResources(0, 1, pInputBufferSRV.GetAddressOf());
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
}
