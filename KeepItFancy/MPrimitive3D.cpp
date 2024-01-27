#include "MPrimitive3D.h"

///--------------------------------------------------
//! Segment-based Sphere Class
///--------------------------------------------------
void SSphere::BindVertices()
{
	m_Vertices.clear();

	// vertex data for the longitude | x ring
	for (int i = 0; i <= m_iSegments; ++i) {
		float angle = XM_2PI * static_cast<float>(i) / static_cast<float>(m_iSegments);
		VERTEX vtx = {};
		vtx.pos.x = 0.0f;
		vtx.pos.y = m_fRadius * sinf(angle);
		vtx.pos.z = m_fRadius * cosf(angle);

		vtx.uv.x = static_cast<float>(i) / static_cast<float>(m_iSegments);
		vtx.uv.y = 0.0f;

		vtx.color = { 1.0f, 0.0f, 0.0f, 1.0f };		// red

		vtx.normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		m_Vertices.emplace_back(vtx);
	}

	// vertex data for the latitude | y ring
	for (int i = 0; i <= m_iSegments; ++i) {
		float angle = XM_2PI * static_cast<float>(i) / static_cast<float>(m_iSegments);
		VERTEX vtx = {};
		vtx.pos.x = m_fRadius * sinf(angle);
		vtx.pos.y = 0.0f;
		vtx.pos.z = m_fRadius * cosf(angle);

		vtx.uv.x = static_cast<float>(i) / static_cast<float>(m_iSegments);
		vtx.uv.y = 0.0f;

		vtx.color = { 0.0f, 1.0f, 0.0f, 1.0f };		// green

		vtx.normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		m_Vertices.emplace_back(vtx);
	}

	// vertex data for the meridian | z ring
	for (int i = 0; i <= m_iSegments; ++i) {
		float angle = XM_2PI * static_cast<float>(i) / static_cast<float>(m_iSegments);
		VERTEX vtx = {};
		vtx.pos.x = m_fRadius * sinf(angle);
		vtx.pos.y = m_fRadius * cosf(angle);
		vtx.pos.z = 0.0f;

		vtx.uv.x = static_cast<float>(i) / static_cast<float>(m_iSegments);
		vtx.uv.y = 0.0f;

		vtx.color = { 0.0f, 0.0f, 1.0f, 1.0f };		// blue

		vtx.normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		m_Vertices.emplace_back(vtx);
	}
}

void SSphere::Create(float radius, int segments)
{
	m_useLight = false;

	m_fRadius = radius;
	m_iSegments = segments;
	m_iSides = 3;

	BindVertices();
	BindIndices();

	CreateDefaultBuffers();
	LoadDefaultShaders();
}



///--------------------------------------------------
//! Triangle-based Sphere Class
///--------------------------------------------------
void TSphere::BindVertices()
{
	m_Vertices.clear();

	float azimuth = 0.0f;			// 方位角
	float elevation = 0.0f;			// 仰角
	XMFLOAT3 locNormal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 方位角と仰角から球メッシュの頂点データを作成
	for (unsigned int y = 0; y <= m_iDivY; y++) {
		elevation = (XM_PI * (float)y) / (float)m_iDivY;		// 仰角をセット
		float r = m_fRadius * sinf(elevation);					// 仰角に応じた半径を計算

		for (unsigned int x = 0; x <= m_iDivX; x++) {
			azimuth = (2 * XM_PI * (float)x) / (float)m_iDivX;	// 方位角をセット

			VERTEX vtx;
			vtx.pos.x = r * sinf(azimuth);
			vtx.pos.y = m_fRadius * cosf(elevation);
			vtx.pos.z = r * cosf(azimuth);

			vtx.uv.x = static_cast<float>(x) / static_cast<float>(m_iDivX);
			vtx.uv.y = static_cast<float>(y) / static_cast<float>(m_iDivY);

			vtx.color = { 1.0f , 1.0f, 1.0f, 1.0f };

			NormalizeVectors(vtx.pos, locNormal);
			vtx.normal = locNormal;

			m_Vertices.emplace_back(vtx);
		}
	}
}

void TSphere::Create(float radius, int divisions)
{
	m_fRadius = radius;

	// 分割数を保存
	m_iDivX = m_iDivY = divisions;

	BindVertices();
	BindIndices();

	CreateDefaultBuffers();
	LoadDefaultShaders();
}



///--------------------------------------------------
//! Cube Class
///--------------------------------------------------
void Cube::BindVertices()
{
	m_Vertices.clear();

	for (int currentFace = FRONT; currentFace < END; currentFace++) {
		VERTEX vtx = {};
		XMFLOAT3 locNormal = {};

		switch (currentFace)
		{
		case FRONT:
			for (unsigned int y = 0; y <= m_iDivY; y++) {
				for (unsigned int x = 0; x <= m_iDivX; x++) {

					vtx.pos.x = -m_fWidth / 2.0f + x * m_fWidth / m_iDivX;
					vtx.pos.y = -m_fHeight / 2.0f + y * m_fHeight / m_iDivY;
					vtx.pos.z = -m_fDepth / 2.0f;

					vtx.uv.x = static_cast<float>(x) / static_cast<float>(m_iDivX);
					vtx.uv.y = static_cast<float>(y) / static_cast<float>(m_iDivY);

					vtx.color = { 1.0f, 1.0f, 1.0f, 1.0f };

					locNormal = XMFLOAT3(0.0f, 0.0f, -1.0f);
					NormalizeVectors(vtx.pos, locNormal);
					vtx.normal = locNormal;

					m_Vertices.emplace_back(vtx);
				}
			}
			break;

		case BACK:
			for (unsigned int y = 0; y <= m_iDivY; y++) {
				for (unsigned int x = 0; x <= m_iDivX; x++) {

					vtx.pos.x = -m_fWidth / 2.0f + x * m_fWidth / m_iDivX;
					vtx.pos.y = -m_fHeight / 2.0f + y * m_fHeight / m_iDivY;
					vtx.pos.z = m_fDepth / 2.0f;

					vtx.uv.x = static_cast<float>(x) / static_cast<float>(m_iDivX);
					vtx.uv.y = static_cast<float>(y) / static_cast<float>(m_iDivY);

					vtx.color = { 1.0f, 1.0f, 1.0f, 1.0f };

					locNormal = XMFLOAT3(0.0f, 0.0f, 1.0f);
					NormalizeVectors(vtx.pos, locNormal);
					vtx.normal = locNormal;

					m_Vertices.emplace_back(vtx);
				}
			}
			break;

		case LEFT:
			for (unsigned int y = 0; y <= m_iDivY; y++) {
				for (unsigned int z = 0; z <= m_iDivZ; z++) {

					vtx.pos.x = -m_fWidth / 2.0f;
					vtx.pos.y = -m_fHeight / 2.0f + y * m_fHeight / m_iDivY;
					vtx.pos.z = -m_fDepth / 2.0f + z * m_fDepth / m_iDivZ;

					vtx.uv.x = static_cast<float>(z) / static_cast<float>(m_iDivZ);
					vtx.uv.y = static_cast<float>(y) / static_cast<float>(m_iDivY);

					vtx.color = { 1.0f, 1.0f, 1.0f, 1.0f };

					locNormal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
					NormalizeVectors(vtx.pos, locNormal);
					vtx.normal = locNormal;

					m_Vertices.emplace_back(vtx);
				}
			}
			break;

		case RIGHT:
			for (unsigned int y = 0; y <= m_iDivY; y++) {
				for (unsigned int z = 0; z <= m_iDivZ; z++) {

					vtx.pos.x = m_fWidth / 2.0f;
					vtx.pos.y = -m_fHeight / 2.0f + y * m_fHeight / m_iDivY;
					vtx.pos.z = -m_fDepth / 2.0f + z * m_fDepth / m_iDivZ;

					vtx.uv.x = static_cast<float>(z) / static_cast<float>(m_iDivZ);
					vtx.uv.y = static_cast<float>(y) / static_cast<float>(m_iDivY);

					vtx.color = { 1.0f, 1.0f, 1.0f, 1.0f };

					locNormal = XMFLOAT3(1.0f, 0.0f, 0.0f);
					NormalizeVectors(vtx.pos, locNormal);
					vtx.normal = locNormal;

					m_Vertices.emplace_back(vtx);
				}
			}
			break;

		case TOP:
			for (unsigned int z = 0; z <= m_iDivZ; z++) {
				for (unsigned int x = 0; x <= m_iDivX; x++) {

					vtx.pos.x = -m_fWidth / 2.0f + x * m_fWidth / m_iDivX;
					vtx.pos.y = m_fHeight / 2.0f;
					vtx.pos.z = -m_fDepth / 2.0f + z * m_fDepth / m_iDivZ;

					vtx.uv.x = static_cast<float>(x) / static_cast<float>(m_iDivX);
					vtx.uv.y = static_cast<float>(z) / static_cast<float>(m_iDivZ);

					vtx.color = { 1.0f, 1.0f, 1.0f, 1.0f };

					locNormal = XMFLOAT3(0.0f, 1.0f, 0.0f);
					NormalizeVectors(vtx.pos, locNormal);
					vtx.normal = locNormal;

					m_Vertices.emplace_back(vtx);
				}
			}
			break;

		case BOTTOM:
			for (unsigned int z = 0; z <= m_iDivZ; z++) {
				for (unsigned int x = 0; x <= m_iDivX; x++) {

					vtx.pos.x = -m_fWidth / 2.0f + x * m_fWidth / m_iDivX;
					vtx.pos.y = -m_fHeight / 2.0f;
					vtx.pos.z = -m_fDepth / 2.0f + z * m_fDepth / m_iDivZ;

					vtx.uv.x = static_cast<float>(x) / static_cast<float>(m_iDivX);
					vtx.uv.y = static_cast<float>(z) / static_cast<float>(m_iDivZ);

					vtx.color = { 1.0f, 1.0f, 1.0f, 1.0f };

					locNormal = XMFLOAT3(0.0f, -1.0f, 0.0f);
					NormalizeVectors(vtx.pos, locNormal);
					vtx.normal = locNormal;

					m_Vertices.emplace_back(vtx);
				}
			}
			break;
		}
	}
}

void Cube::BindIndices()
{
	m_Faces.clear();

	for (int currentFace = FRONT; currentFace < END; currentFace++) {
		unsigned int offset = 0;

		if (currentFace <= BACK)
		{
			for (unsigned int y = 0; y < m_iDivY; y++) {
				for (unsigned int x = 0; x < m_iDivX; x++) {
					offset = ((m_iDivX + 1) * (m_iDivY + 1) * currentFace) + ((m_iDivX + 1) * y) + x;

					GenerateIndices(m_iDivX, offset);
				}
			}
		}
		else if (BACK < currentFace && currentFace <= RIGHT)
		{
			for (unsigned int y = 0; y < m_iDivY; y++) {
				for (unsigned int z = 0; z < m_iDivZ; z++) {
					offset = ((m_iDivZ + 1) * (m_iDivY + 1) * currentFace) + ((m_iDivZ + 1) * y) + z;

					GenerateIndices(m_iDivZ, offset);
				}
			}
		}
		else if (RIGHT < currentFace && currentFace <= BOTTOM)
		{
			for (unsigned int z = 0; z < m_iDivZ; z++) {
				for (unsigned int x = 0; x < m_iDivX; x++) {
					offset = ((m_iDivX + 1) * (m_iDivZ + 1) * currentFace) + ((m_iDivX + 1) * z) + x;

					GenerateIndices(m_iDivZ, offset);
				}
			}
		}
	}
}

void Cube::Create(float width, float height, float depth, int divisions)
{
	m_fWidth = width;
	m_fHeight = height;
	m_fDepth = depth;

	// 分割数を保存
	m_iDivX = m_iDivY = m_iDivZ = divisions;

	BindVertices();
	BindIndices();

	CreateDefaultBuffers();
	LoadDefaultShaders();
}
