#include "MPrimitive2D.h"

///--------------------------------------------------
//! Segment-based Circle Class
///--------------------------------------------------
void SCircle::BindVertices()
{
	m_Vertices.clear();
	// vertex data for the circle(ring)
	for (int i = 0; i <= m_iSegments; ++i) {
		float angle = XM_2PI * static_cast<float>(i) / static_cast<float>(m_iSegments);
		VERTEX vtx = {};
		vtx.pos.x = m_fRadius * cosf(angle);
		vtx.pos.y = m_fRadius * sinf(angle);
		vtx.pos.z = 0.0f;

		vtx.uv.x = static_cast<float>(i) / static_cast<float>(m_iSegments);
		vtx.uv.y = 0.0f;

		vtx.color = { 1.0f, 1.0f, 1.0f, 1.0f };

		vtx.normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		m_Vertices.emplace_back(vtx);
	}
}

/*
void SCircle::BindIndices()
{
	m_Faces.clear();
	for (int i = 0; i < m_iSegments; ++i) {
		FACE face = {};
		face.idx[0] = i;
		face.idx[1] = i + 1;
		//face.idx[2] = static_cast<unsigned short>(m_Vertices.size() - 1);		// index of center vertex

		m_Faces.emplace_back(face);
	}
	assert(m_Faces.size() == m_iSegments);
}
*/

void SCircle::Create(float radius, int segments)
{
	m_useLight = false;

	m_fRadius = radius;
	m_iSegments = segments;
	SetBaseColor(sRGBA(0.0f, 1.0f, 0.0f));

	BindVertices();
	BindIndices();

	CreateDefaultBuffers();
	LoadDefaultShaders();
}


///--------------------------------------------------
//! Triangle-based Plane Class
///--------------------------------------------------
void TPlane::BindVertices()
{
	m_Vertices.clear();

	XMFLOAT3 locNormal;
	if (!useXZAxis)
	{
		locNormal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	}
	else
	{
		locNormal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}

	for (unsigned int y = 0; y <= m_iDivY; y++) {
		for (unsigned int x = 0; x <= m_iDivX; x++) {
			VERTEX vtx = {};
			vtx.pos.x = -m_fWidth / 2.0f + x * m_fWidth / m_iDivX;
			if (!useXZAxis)
			{
				vtx.pos.y = -m_fHeight / 2.0f + y * m_fHeight / m_iDivY;
				vtx.pos.z = 0.0f;
			}
			else
			{
				vtx.pos.y = 0.0f;
				vtx.pos.z = -m_fDepth / 2.0f + y * m_fDepth / m_iDivY;
			}

			vtx.uv.x = static_cast<float>(x) / static_cast<float>(m_iDivX);
			vtx.uv.y = 1 - static_cast<float>(y) / static_cast<float>(m_iDivY);

			vtx.color = { 1.0f, 1.0f, 1.0f, 1.0f };

			if (useXZAxis)
				NormalizeVectors(vtx.pos, locNormal);
			vtx.normal = locNormal;

			m_Vertices.emplace_back(vtx);
		}
	}
}

void TPlane::Create(float width, float height, int divX, int divY)
{
	m_fWidth = width;
	if (!useXZAxis)
	{
		m_fHeight = height;
	}

	m_iDivX = divX;
	m_iDivY = divY;

	BindVertices();
	BindIndices();

	CreateDefaultBuffers();
	LoadDefaultShaders();
}

