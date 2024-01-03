#ifndef MPRIMITIVE2D_H
#define MPRIMITIVE2D_H

#include "MeshCommons.h"

class SSegment : public LINEBASE
{

};

///--------------------------------------------------
//! SCircle Class
///--------------------------------------------------
//! \class SCircle MPrimitive2D.h "MPrimitive2D.h"
/*! \brief Segment-type Circle Mesh Class
 *  \brief 弓形円メッシュクラス
 */
class SCircle : public LINEBASE
{
protected:
	float		m_fRadius;

public:
	SCircle() :
		m_fRadius(1.0f)
	{}
	~SCircle() {}

	void Create(float radius = 1.0f, int segments = 32, sRGBA _color = sRGBA(0.0f, 1.0f, 0.0f));

protected:
	virtual void LoadDefaultShaders()
	{
		m_pVS = AddComponent<VertexShader>();
		m_pVS->LoadShader(SHADER_PATH("VS_WorldPosition.cso"));

		m_pPS = AddComponent<PixelShader>();
		m_pPS->LoadShader(SHADER_PATH("PS_FlatColor.cso"));
	}

	void BindVertices(sRGBA);
};



///--------------------------------------------------
//! Plane Class
///--------------------------------------------------
//! \class Plane MPrimitive2D.h "MPrimitive2D.h"
/*! \brief Plane Mesh Class
 *  \brief 平面メッシュクラス
 */
class Plane : public TRIANGLEBASE
{
protected:
	float		m_fWidth;
	float		m_fHeight;

public:
	Plane() :
		m_fWidth(1.0f),
		m_fHeight(1.0f)
	{}
	~Plane() {}

	void Create(float width = 1.0f, float height = 1.0f, int divX = 1, int divY = 1, sRGBA _color = sRGBA());

protected:
	virtual void LoadDefaultShaders()
	{
		m_pVS = AddComponent<VertexShader>();
		m_pVS->LoadShader(SHADER_PATH("VS_WorldPosition.cso"));

		m_pPS = AddComponent<PixelShader>();
		m_pPS->LoadShader(SHADER_PATH("PS_HalfLambert.cso"));
	}

	void BindVertices(sRGBA);
};

#endif // !MPRIMITIVE2D_H