#ifndef MPRIMITIVE3D_H
#define MPRIMITIVE3D_H

#include "MeshCommons.h"

///--------------------------------------------------
//! Segment-based Sphere Class
///--------------------------------------------------
//! \class SSphere MPrimitive3D.h "MPrimitive3D.h"
/*! \brief Segment-based Sphere Mesh Class
 *  \brief 線形球体メッシュクラス
 */
class SSphere : public LINEBASE
{
protected:
	float		m_fRadius;

public:
	SSphere() :
		m_fRadius(0.0f)
	{}
	~SSphere() {}

	void Create(float radius = 1.0f, int segments = 32);

protected:
	virtual void LoadDefaultShaders()
	{
		m_pVS = AddComponent<VertexShader>();
		m_pVS->LoadShader(SHADER_PATH("VS_WorldPosition.cso"));

		m_pPS = AddComponent<PixelShader>();
		m_pPS->LoadShader(SHADER_PATH("PS_FlatColor.cso"));
	}

	void BindVertices();
};



///--------------------------------------------------
//! TSphere Class
///--------------------------------------------------
//! \class TSphere MPrimitive3D.h "MPrimitive3D.h"
/*! \brief Triangle-based Sphere Mesh Class
 *  \brief 三角形球体メッシュクラス
 */
class TSphere : public TRIANGLEBASE
{
protected:
	float		m_fRadius;

public:
	TSphere() :
		m_fRadius(0.0f)
	{}
	~TSphere() {}

	void Create(float radius = 1.0f, int divisions = 8);

protected:
	virtual void LoadDefaultShaders()
	{
		m_pVS = AddComponent<VertexShader>();
		m_pVS->LoadShader(SHADER_PATH("VS_WorldPosition.cso"));

		m_pPS = AddComponent<PixelShader>();
		m_pPS->LoadShader(SHADER_PATH("PS_BlinnPhong.cso"));
	}

	void BindVertices();
};



///--------------------------------------------------
//! Cube Class
///--------------------------------------------------
//! \class Cube MPrimitive3D.h "MPrimitive3D.h"
/*! \brief Cube Mesh Class
 *  \brief 平面メッシュクラス
 */
class Cube : public TRIANGLEBASE
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

protected:
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

	void Create(float width = 1.0f, float height = 1.0f, float depth = 1.0f, int divisions = 1);

protected:
	virtual void LoadDefaultShaders()
	{
		m_pVS = AddComponent<VertexShader>();
		m_pVS->LoadShader(SHADER_PATH("VS_WorldPosition.cso"));

		m_pPS = AddComponent<PixelShader>();
		m_pPS->LoadShader(SHADER_PATH("PS_HalfLambert.cso"));
	}

	void BindIndices() override;
	void BindVertices();
};

#endif // MPRIMITIVE3D_H