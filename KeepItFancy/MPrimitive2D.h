#ifndef MPRIMITIVE2D_H
#define MPRIMITIVE2D_H

#include "MeshCommons.h"

class SSegment : public LINEBASE
{

};



///--------------------------------------------------
//! Segment-based Circle Class
///--------------------------------------------------
//! \class SCircle MPrimitive2D.h "MPrimitive2D.h"
/*! \brief Segment-based Circle Mesh Class
 *  \brief 弓形円メッシュクラス
 */
class SCircle : public LINEBASE
{
protected:
	float		m_fRadius;

public:
	SCircle() :
		m_fRadius(0.0f)
	{}
	~SCircle() {}

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
//! Triangle-based Plane Class
///--------------------------------------------------
//! \class TPlane MPrimitive2D.h "MPrimitive2D.h"
/*! \brief Triangle-based Plane Mesh Class
 *  \brief 三角形平面メッシュクラス
 */
class TPlane : public TRIANGLEBASE
{
protected:
	float		m_fWidth;
	float		m_fHeight;
	float		m_fDepth;
	bool		useXZAxis = false;

public:
	TPlane() :
		m_fWidth(0.0f),
		m_fHeight(0.0f),
		m_fDepth(0.0f)
	{}
	~TPlane() {}

	virtual void Create(float width = 1.0f, float height = 1.0f, int divX = 1, int divY = 1);

protected:
	virtual void LoadDefaultShaders()
	{
		m_pVS = AddComponent<VertexShader>();
		m_pVS->LoadShader(SHADER_PATH("VS_WorldPosition.cso"));

		m_pPS = AddComponent<PixelShader>();
		m_pPS->LoadShader(SHADER_PATH("PS_HalfLambert.cso"));
	}

	void BindVertices();
};

#endif // !MPRIMITIVE2D_H