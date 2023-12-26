#ifndef MPRIMITIVE3D_H
#define MPRIMITIVE3D_H

#include "MPrimitive2D.h"

///--------------------------------------------------
//! Sphere Class
///--------------------------------------------------
//! \class Sphere Mesh.h "Mesh.h"
/*! \brief Sphere Mesh Class
 *  \brief 球体メッシュクラス
 */
class Sphere : public MESH
{
private:
	float		m_fRadius;

public:
	Sphere() :
		m_fRadius(0.0f)
	{}
	~Sphere() {}

	void Create(float radius, int divisions = 8, sRGBA color = sRGBA());

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

///--------------------------------------------------
//! Cube Class
///--------------------------------------------------
//! \class Cube Mesh.h "Mesh.h"
/*! \brief Cube Mesh Class
 *  \brief 平面メッシュクラス
 */
class Cube : public MESH
{
private:
	enum CubeFace {
		FRONT,
		BACK,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
		END
	};

	float		m_fWidth;
	float		m_fHeight;
	float		m_fDepth;

public:
	Cube() :
		m_fWidth(0.0f),
		m_fHeight(0.0f),
		m_fDepth(0.0f)
	{}
	~Cube() {}

	void Create(float width = 1.0f, float height = 1.0f, float depth = 1.0f, int divisions = 1, sRGBA color = sRGBA());

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

	void BindIndices() override;
	void BindVertices(sRGBA color);
};

#endif // MPRIMITIVE3D_H