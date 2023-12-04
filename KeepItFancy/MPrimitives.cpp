#include "MPrimitives.h"

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

	// 分割数を保存
	m_iDivX = divX;
	m_iDivY = divY;

	BindVertices(color);
	MESH::BindIndices();

	MESH::Create();
}

///--------------------------------------------------
//! Sphere Class
///--------------------------------------------------
void Sphere::BindVertices(sRGBA color)
{
	m_Vertices.clear();

	float azimuth = 0.0f;			// 方位角
	float elevation = 0.0f;			// 仰角
	XMFLOAT3 locNormal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 方位角と仰角から球メッシュの頂点データを作成
	for (unsigned int y = 0; y <= m_iDivY; y++) {
		elevation = (XM_PI * (float)y) / (float)m_iDivY;			// 仰角をセット
		float r = m_fRadius * sinf(elevation);					// 仰角に応じた半径を計算

		for (unsigned int x = 0; x <= m_iDivX; x++) {
			azimuth = (2 * XM_PI * (float)x) / (float)m_iDivX;	// 方位角をセット

			VERTEX vtx;
			vtx.pos.x = r * sinf(azimuth);
			vtx.pos.y = m_fRadius * cosf(elevation);
			vtx.pos.z = r * cosf(azimuth);

			vtx.color = { color.r, color.g, color.b, color.a };

			NormalizeVectors(vtx.pos, locNormal);
			vtx.normal = locNormal;

			m_Vertices.emplace_back(vtx);
		}
	}
}

void Sphere::Create(float radius, int divisions, sRGBA color)
{
	m_fRadius = radius;

	// 分割数を保存
	m_iDivX = m_iDivY = divisions;

	BindVertices(color);
	MESH::BindIndices();

	MESH::Create();
}

///--------------------------------------------------
//! Cube Class
///--------------------------------------------------
void Cube::BindVertices(sRGBA color)
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

					vtx.color = { color.r, color.g, color.b, color.a };

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

					vtx.color = { color.r, color.g, color.b, color.a };

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

					vtx.color = { color.r, color.g, color.b, color.a };

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

					vtx.color = { color.r, color.g, color.b, color.a };

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

					vtx.color = { color.r, color.g, color.b, color.a };

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

					vtx.color = { color.r, color.g, color.b, color.a };

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

void Cube::TestVertices(sRGBA color)
{
	m_Vertices.clear();

	for (int currentFace = FRONT; currentFace < END; currentFace++) {
		VERTEX vtx = {};
		XMFLOAT3 locNormal = {};

		for (unsigned int y = 0; y <= m_iDivY; y++) {
			for (unsigned int x = 0; x <= m_iDivX; x++) {
				switch (currentFace)
				{
				case FRONT:
					vtx.pos.x = -m_fWidth / 2.0f + x * m_fWidth / m_iDivX;
					vtx.pos.y = -m_fDepth / 2.0f + y * m_fDepth / m_iDivY;
					vtx.pos.z = -m_fDepth / 2.0f;

					locNormal = XMFLOAT3(0.0f, 0.0f, -1.0f);
					break;

				case BACK:
					vtx.pos.x = -m_fWidth / 2.0f + x * m_fWidth / m_iDivX;
					vtx.pos.y = -m_fDepth / 2.0f + y * m_fDepth / m_iDivY;
					vtx.pos.z = m_fDepth / 2.0f;

					locNormal = XMFLOAT3(0.0f, 0.0f, 1.0f);
					break;

				case LEFT:
					vtx.pos.x = -m_fWidth / 2.0f;
					vtx.pos.y = -m_fDepth / 2.0f + y * m_fDepth / m_iDivY;
					vtx.pos.z = -m_fDepth / 2.0f + x * m_fDepth / m_iDivZ;

					locNormal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
					break;

				case RIGHT:
					vtx.pos.x = m_fWidth / 2.0f;
					vtx.pos.y = -m_fDepth / 2.0f + y * m_fDepth / m_iDivY;
					vtx.pos.z = -m_fDepth / 2.0f + x * m_fDepth / m_iDivZ;

					locNormal = XMFLOAT3(1.0f, 0.0f, 0.0f);
					break;

				case TOP:
					vtx.pos.x = -m_fWidth / 2.0f + x * m_fWidth / m_iDivX;
					vtx.pos.y = m_fDepth / 2.0f;
					vtx.pos.z = -m_fDepth / 2.0f + y * m_fDepth / m_iDivZ;

					locNormal = XMFLOAT3(0.0f, 1.0f, 0.0f);
					break;

				case BOTTOM:
					vtx.pos.x = -m_fWidth / 2.0f + x * m_fWidth / m_iDivX;
					vtx.pos.y = -m_fDepth / 2.0f;
					vtx.pos.z = -m_fDepth / 2.0f + y * m_fDepth / m_iDivZ;

					locNormal = XMFLOAT3(0.0f, -1.0f, 0.0f);
					break;
				}
				NormalizeVectors(vtx.pos, locNormal);
				vtx.normal = locNormal;

				vtx.color = { color.r, color.g, color.b, color.a };

				m_Vertices.emplace_back(vtx);
			}
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

					MESH::GenerateIndices(m_iDivX, offset);
				}
			}
		}
		else if (BACK < currentFace && currentFace <= RIGHT)
		{
			for (unsigned int y = 0; y < m_iDivY; y++) {
				for (unsigned int z = 0; z < m_iDivZ; z++) {
					offset = ((m_iDivZ + 1) * (m_iDivY + 1) * currentFace) + ((m_iDivZ + 1) * y) + z;

					MESH::GenerateIndices(m_iDivZ, offset);
				}
			}
		}
		else if (RIGHT < currentFace && currentFace <= BOTTOM)
		{
			for (unsigned int z = 0; z < m_iDivZ; z++) {
				for (unsigned int x = 0; x < m_iDivX; x++) {
					offset = ((m_iDivX + 1) * (m_iDivZ + 1) * currentFace) + ((m_iDivX + 1) * z) + x;

					MESH::GenerateIndices(m_iDivZ, offset);
				}
			}
		}
	}
}

void Cube::Create(float width, float height, float depth, int divisions, sRGBA color)
{
	m_fWidth = width;
	m_fHeight = height;
	m_fDepth = depth;

	// 分割数を保存
	m_iDivX = m_iDivY = m_iDivZ = divisions;

	BindVertices(color);
	BindIndices();

	MESH::Create();
}
