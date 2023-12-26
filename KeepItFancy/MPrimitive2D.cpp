#include "MPrimitive2D.h"

///--------------------------------------------------
//! Circle Class
///--------------------------------------------------
void FlatCircle::BindVertices(sRGBA color)
{
	m_Vertices.clear();
	// vertex data for the circle(ring)
	for (int i = 0; i <= m_iSegments; ++i) {
		float angle = XM_2PI * static_cast<float>(i) / static_cast<float>(m_iSegments);
		VERTEX vtx = {};
		vtx.pos.x = m_fRadius * cosf(angle);
		vtx.pos.y = m_fRadius * sinf(angle);
		vtx.pos.z = 0.0f;

		vtx.color = { color.r, color.g, color.b, color.a };

		vtx.normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		m_Vertices.emplace_back(vtx);
	}

	// vertex data for the center of the circle
	VERTEX centerVtx = {
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		{ color.r, color.g, color.b, color.a },
		XMFLOAT3(0.0f, 0.0f, -1.0f)
	};
	m_Vertices.emplace_back(centerVtx);
}

void FlatCircle::BindIndices()
{
	m_Faces.clear();
	for (int i = 0; i < m_iSegments; ++i) {
		FACE face = {};
		face.idx[0] = i;
		face.idx[1] = i + 1;
		face.idx[2] = static_cast<unsigned short>(m_Vertices.size() - 1);		// index of center vertex

		m_Faces.emplace_back(face);
	}
	assert(m_Faces.size() == m_iSegments);
}

void FlatCircle::Create(float radius, int segments, sRGBA color)
{
	useLight = false;

	m_fRadius = radius;
	m_iSegments = segments;

	BindVertices(color);
	BindIndices();

	CreateDefaultBuffers();
	LoadDefaultShaders();
}

void FlatCircle::Draw(RasterType cullmode)
{
	BindComputeShaders();

	// set buffers
	unsigned int stride = sizeof(VERTEX);
	unsigned offset = 0;
	DirectX11::GetContext()->IASetVertexBuffers(0, 1, m_cpVertexBuf.GetAddressOf(), &stride, &offset);
	DirectX11::GetContext()->IASetIndexBuffer(m_cpIndexBuf.Get(), DXGI_FORMAT_R32_UINT, 0);

	DirectX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//SetTopology(TopologyType::TRIANGLELIST);

	SetWVPMatrix(m_pVS);
	m_pVS->BindShader();

	ProcessTessellation();

	SetLight(m_pPS);
	m_pPS->BindShader();

	// render
	SetCulling(cullmode);
	DirectX11::GetContext()->DrawIndexed(
		m_Faces.size() * 3,						// 描画するインデックス数（面数×３）
		0,										// 最初のインデックスバッファの位置
		0);										// 頂点バッファの最初から使う
}

void Plane::BindVertices(sRGBA color)
{
	m_Vertices.clear();

	for (unsigned int y = 0; y <= m_iDivY; y++) {
		for (unsigned int x = 0; x <= m_iDivX; x++) {
			VERTEX vtx = {};
			vtx.pos.x = -m_fWidth / 2.0f + x * m_fWidth / m_iDivX;
			vtx.pos.y = -m_fHeight / 2.0f + y * m_fHeight / m_iDivY;
			vtx.pos.z = 0.0f;

			vtx.color = { color.r, color.g, color.b, color.a };

			vtx.normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			m_Vertices.emplace_back(vtx);
		}
	}
}

void Plane::Create(float width, float height, int divX, int divY, sRGBA color)
{
	m_fWidth = width;
	m_fHeight = height;

	m_iDivX = divX;
	m_iDivY = divY;

	BindVertices(color);
	BindIndices();

	CreateDefaultBuffers();
	LoadDefaultShaders();
}

