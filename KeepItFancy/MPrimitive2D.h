#ifndef MPRIMITIVE2D_H
#define MPRIMITIVE2D_H

#include "BMesh.h"

///--------------------------------------------------
//! Circle Class
///--------------------------------------------------
//! \class Circle Mesh.h "Mesh.h"
/*! \brief Circle Mesh Class
 *  \brief 円メッシュクラス
 */
class FlatCircle : public MESH
{
protected:


private:
	float		m_fRadius;
	int			m_iSegments;	// 分割数

public:
	FlatCircle() :
		m_fRadius(0.0f),
		m_iSegments(0)
	{}
	~FlatCircle() {}

	void Create(float radius = 1.0f, int segments = 32, sRGBA color = sRGBA());
	void Draw(RasterType cullmode = RasterType::WIREFRAME_NO_CULL);

protected:
	void CreateDefaultBuffers()
	{
		CHECK_HR(CreateVertexBuffer(sizeof(VERTEX) * m_Vertices.size(), m_Vertices.data(), m_cpVertexBuf.GetAddressOf()));
		CHECK_HR(CreateIndexBuffer(m_Faces.size() * 3, m_Faces.data(), m_cpIndexBuf.GetAddressOf()));
	}

	virtual void LoadDefaultShaders()
	{
		m_pVS = AddComponent<VertexShader>();
		m_pVS->LoadShader(SHADER_PATH("VS_WorldPosition.cso"));

		m_pPS = AddComponent<PixelShader>();
		m_pPS->LoadShader(SHADER_PATH("PS_FlatColor.cso"));
	}

	void BindVertices(sRGBA color);
	void BindIndices() override;
};

///--------------------------------------------------
//! Plane Class
///--------------------------------------------------
//! \class Plane Mesh.h "Mesh.h"
/*! \brief Plane Mesh Class
 *  \brief 平面メッシュクラス
 */
class Plane : public MESH
{
protected:


private:
	float		m_fWidth;
	float		m_fHeight;

public:
	Plane() :
		m_fWidth(0.0f),
		m_fHeight(0.0f)
	{}
	~Plane() {}

	void Create(float width = 1.0f, float height = 1.0f, int divX = 1, int divY = 1, sRGBA color = sRGBA());

protected:
	void CreateDefaultBuffers()
	{
		CHECK_HR(CreateVertexBuffer(sizeof(VERTEX) * m_Vertices.size(), m_Vertices.data(), m_cpVertexBuf.GetAddressOf()));
		CHECK_HR(CreateIndexBuffer(m_Faces.size() * 3, m_Faces.data(), m_cpIndexBuf.GetAddressOf()));
	}

	virtual void LoadDefaultShaders()
	{
		m_pVS = AddComponent<VertexShader>();
		m_pVS->LoadShader(SHADER_PATH("VS_WorldPosition.cso"));

		m_pPS = AddComponent<PixelShader>();
		m_pPS->LoadShader(SHADER_PATH("PS_HalfLambert.cso"));
	}

	void BindVertices(sRGBA color);
};

#endif // !MPRIMITIVE2D_H