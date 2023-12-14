#ifndef MPRIMITIVES_H
#define MPRIMITIVES_H

#include "BMesh.h"

///--------------------------------------------------
//! Plane Class
///--------------------------------------------------
//! \class Plane Mesh.h "Mesh.h"
/*! \brief Plane Mesh Class
 *  \brief 平面メッシュクラス
 */
class Plane : public MESH
{
private:
	float		m_fWidth;
	float		m_fHeight;

public:
	Plane() :
		m_fWidth(0.0f),
		m_fHeight(0.0f)
	{}
	~Plane() {}

protected:
	void BindVertices(sRGBA color);

public:
	void Create(float width = 1.0f, float height = 1.0f, int divX = 1, int divY = 1, sRGBA color = sRGBA());
};

///--------------------------------------------------
//! Circle Class
///--------------------------------------------------
//! \class Circle Mesh.h "Mesh.h"
/*! \brief Circle Mesh Class
 *  \brief 円メッシュクラス
 */
class FlatCircle : public MESH
{
private:
	float		m_fRadius;
	int			m_iSegments;	// 分割数

public:
	FlatCircle() :
		m_fRadius(0.0f),
		m_iSegments(0)
	{}
	~FlatCircle() {}

protected:
	void BindVertices(sRGBA color);
	void BindIndices() override;

public:
	void Create(float radius = 1.0f, int segments = 32, sRGBA color = sRGBA());
	void Draw(RasterType cullmode = RasterType::WIREFRAME_NO_CULL);
};

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

protected:
	 void BindVertices(sRGBA color);

public:
	void Create(float radius, int divisions = 8, sRGBA color = sRGBA());
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

protected:
	void BindIndices();
	void BindVertices(sRGBA color);

public:
	void Create(float width = 1.0f, float height = 1.0f, float depth = 1.0f, int divisions = 1, sRGBA color = sRGBA());
};

#endif // !MPRIMITIVES_H