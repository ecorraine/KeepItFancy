#ifndef BMESH_H
#define BMESH_H

#include "GameObject.h"
#include "Buffers.h"
#include "srgba.hpp"
#include "myUtilities.hpp"

enum class TopologyType
{
	TRIANGLELIST,
	TRIANGLESTRIP,
	MAX_TOPOLOGY_TYPE
};

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! MESH Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class MESH Mesh.h "Mesh.h"
/*! \brief Base Class for Meshes
 *  \brief メッシュ親クラス
 */
class MESH : public GameObject
{
protected:
public:
	MESH() :
		m_pVS(nullptr),
		m_pPS(nullptr),
		m_iDivX(0),
		m_iDivY(0),
		m_iDivZ(0)
	{}
	~MESH() {}

	struct VERTEX
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
		XMFLOAT3 normal;
	};

protected:
	struct FACE
	{
		unsigned int idx[3];
	};

	VertexShader*	m_pVS;
	PixelShader*	m_pPS;

	ComPtr<ID3D11Buffer>				m_cpVertexBuf;
	ComPtr<ID3D11Buffer>				m_cpIndexBuf;
	ComPtr<ID3D11ShaderResourceView>	m_cpSRV;			//!< テクスチャ

	std::vector<VERTEX>					m_Vertices;
	std::vector<FACE>					m_Faces;

	unsigned int						m_iDivX;			//!< X方向の分割数
	unsigned int						m_iDivY;			//!< Y方向の分割数
	unsigned int						m_iDivZ;			//!< Z方向の分割数

	void NormalizeVectors(XMFLOAT3 vector, XMFLOAT3& normal)
	{
		DXVec3Normalize(normal, vector);
	}

	virtual void BindVertices(sRGBA) = 0;
	//! \fn void GenerateIndices(int SideToDivide, int offset)
	/*! \brief generate indices according offset
	 *  \brief オフセットによるインデックス生成
	 *  \param SideToDivide : side where division is applied | 分割を適用する方向
	 *  \param offset : offset count per division | 分割毎のオフセット
	 */
	void GenerateIndices(int SideToDivide, int offset);
	virtual void BindIndices();

	void CreateDefaultBuffers()
	{
		CHECK_HR(CreateVertexBuffer(sizeof(VERTEX) * m_Vertices.size(), m_Vertices.data(), &m_cpVertexBuf, true));
		CHECK_HR(CreateIndexBuffer(m_Faces.size() * 3, m_Faces.data(), &m_cpIndexBuf));
	}

	virtual void LoadDefaultShaders()
	{
		m_pVS = AddComponent<VertexShader>();
		m_pVS->LoadShader(SHADER_PATH("VS_WorldPosition.cso"));

		m_pPS = AddComponent<PixelShader>();
		m_pPS->LoadShader(SHADER_PATH("PS_HalfLambert.cso"));
	}

	virtual void Create()
	{
		CreateDefaultBuffers();
		LoadDefaultShaders();
	}

	void BindShaders()
	{
		BindComputeShaders();

		SetWVPMatrix(m_pVS);
		m_pVS->BindShader();

		ProcessTessellation();

		SetLight(m_pPS);
		m_pPS->BindShader();
	}
	virtual void BindComputeShaders() {}
	virtual void ProcessTessellation() {}

public:
	static void SetCulling(RasterType cullmode)
	{
		switch (cullmode)
		{
		case RasterType::SOLID_NO_CULL:
			DirectX11::SetRasterState(RasterType::SOLID_NO_CULL);
			break;
		case RasterType::CULL_FRONT:
			DirectX11::SetRasterState(RasterType::CULL_FRONT);
			break;
		case RasterType::CULL_BACK:
			DirectX11::SetRasterState(RasterType::CULL_BACK);
			break;
		case RasterType::WIREFRAME_NO_CULL:
			DirectX11::SetRasterState(RasterType::WIREFRAME_NO_CULL);
			break;
		case RasterType::WIREFRAME_CULL_BACK:
			DirectX11::SetRasterState(RasterType::WIREFRAME_CULL_BACK);
			break;
		default:
			RasterType::CULL_BACK;
			break;
		}
	}

	virtual void Draw(RasterType cullmode = RasterType::CULL_BACK);
};

#endif // !BMESH_H
