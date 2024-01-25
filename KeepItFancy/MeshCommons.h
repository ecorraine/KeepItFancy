#ifndef MESHCOMMONS_H
#define MESHCOMMONS_H

#include "BMesh.h"

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! LINEBASE Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class LINEBASE MeshCommons.h "MeshCommons.h"
/*! \brief Base Class for Segment-based Meshes
 *  \brief 線形メッシュ親クラス
 */
class LINEBASE : public MESH
{
public:
	LINEBASE() :
		m_iSides(1),
		m_iSegments(1)
	{}
	~LINEBASE()
	{
		m_Faces.erase(m_Faces.begin(), m_Faces.end());
		m_Faces.clear();
	}

protected:
	struct FACE
	{
		unsigned int idx[2];
	};

	std::vector<FACE>	m_Faces;

	unsigned int		m_iSides;			//!< 立体化のため、繰り返すの数
	unsigned int		m_iSegments;		//!< 分割数

	void CreateDefaultBuffers()
	{
		CHECK_HR(CreateVertexBuffer(sizeof(VERTEX) * m_Vertices.size(), m_Vertices.data(), m_cpVertexBuf.GetAddressOf()));
		CHECK_HR(CreateIndexBuffer(m_Faces.size() * 2, m_Faces.data(), m_cpIndexBuf.GetAddressOf()));
	}

	void BindIndices();

public:
	virtual void Draw(RasterType cullmode = RasterType::WIREFRAME_NO_CULL);
};



///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! TRIANGLEBASE Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class TRIANGLEBASE MeshCommons.h "MeshCommons.h"
/*! \brief Base Class for Triangle-based Meshes
 *  \brief 三角形メッシュ親クラス
 */
class TRIANGLEBASE : public MESH
{
public:
	TRIANGLEBASE() :
		m_iDivX(1),
		m_iDivY(1),
		m_iDivZ(1)
	{}
	~TRIANGLEBASE()
	{
		m_Faces.erase(m_Faces.begin(), m_Faces.end());
		m_Faces.clear();
	}

protected:
	struct FACE
	{
		unsigned int idx[3];
	};

	std::vector<FACE>	m_Faces;

	unsigned int		m_iDivX;			//!< X方向の分割数
	unsigned int		m_iDivY;			//!< Y方向の分割数
	unsigned int		m_iDivZ;			//!< Z方向の分割数

	void CreateDefaultBuffers()
	{
		CHECK_HR(CreateVertexBuffer(sizeof(VERTEX) * m_Vertices.size(), m_Vertices.data(), m_cpVertexBuf.GetAddressOf()));
		CHECK_HR(CreateIndexBuffer(m_Faces.size() * 3, m_Faces.data(), m_cpIndexBuf.GetAddressOf()));
	}

	void BindIndices();
	//! \fn void GenerateIndices(int SideToDivide, int offset)
	/*! \brief generate indices according offset
	 *  \brief オフセットによるインデックス生成
	 *  \param SideToDivide : side where division is applied | 分割を適用する方向
	 *  \param offset : offset count per division | 分割毎のオフセット
	 */
	virtual void GenerateIndices(int SideToDivide, int offset);

public:
	virtual void Draw(RasterType cullmode = RasterType::SOLID_NO_CULL);

};

#endif // !MESHCOMMONS_H
