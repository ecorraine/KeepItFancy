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

	CHECK_HR(CreateVertexBufferUAV(sizeof(VERTEX) * m_Vertices.size(), m_Vertices.data(), &m_cpStagingBuffer));
	CHECK_HR(CreateUnorderAccessView(m_cpStagingBuffer.Get(), &m_cpVertexBufUAV));
	m_pCS->BindUAV(0, m_cpVertexBufUAV.Get());

	CHECK_HR(CreateIndexBuffer(m_Faces.size() * 3, m_Faces.data(), &m_cpIndexBuf));

	m_pCS = AddComponent<ComputeShader>();
	m_pCS->LoadShader(SHADER_PATH("CS_GerstnerWaves.cso"));

	m_pVS = AddComponent<VertexShader>();
	m_pVS->LoadShader(SHADER_PATH("VS_WorldPosition.cso"));

	m_pPS = AddComponent<PixelShader>();
	m_pPS->LoadShader(SHADER_PATH("PS_HalfLambert.cso"));
}

void Waves::BindComputeShaders()
{
	m_pCS->BindShader();
	DirectX11::GetContext()->Dispatch(m_iDivX, 1, m_iDivY);

	m_cpVertexBuf = CreateStagingBuffer(m_cpStagingBuffer.Get());

	D3D11_MAPPED_SUBRESOURCE resource;
	std::vector<VERTEX> m_NewVertices;
	m_NewVertices.clear();
	DirectX11::GetContext()->Map(m_cpVertexBuf.Get(), 0, D3D11_MAP_READ, 0, &resource);
	if (resource.pData)
	{
		VERTEX* resultVtx = {};
		resultVtx = (VERTEX*)resource.pData;
		for (int i = 0; i < m_Vertices.size(); ++i)
		{
			m_NewVertices.emplace_back(resultVtx[i]);
		}
	}

	DirectX11::GetContext()->Unmap(m_cpVertexBuf.Get(), 0);
}

void Waves::Update(float tick)
{
	XMFLOAT4 cbdata = { (float)m_iDivX, (float)m_iDivY, tick, 0.0f };
	m_pCS->SendToBuffer(0, &cbdata);
}

