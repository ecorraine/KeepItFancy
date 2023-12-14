#ifndef BMESH_H
#define BMESH_H

#include "GameObject.h"
#include "Buffers.h"
#include "srgba.hpp"
#include "myUtilities.hpp"

///--------------------------------------------------
//! \enum Topology Type
/*! \brief List of Topology Type on how vertices are connected
 *  \brief 頂点がどのように接続されているかのトポロジータイプのr列挙型
 */
enum class TopologyType
{
	TRIANGLELIST,
	LINELIST,
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
		m_iDivX(0),
		m_iDivY(0),
		m_iDivZ(0)
	{}
	~MESH()
	{
		delete m_pPS;
		delete m_pVS;

		m_Vertices.erase(m_Vertices.begin(), m_Vertices.end());
		m_Vertices.clear();
		m_Faces.erase(m_Faces.begin(), m_Faces.end());
		m_Faces.clear();
	}

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

	VertexShader*	m_pVS = nullptr;
	PixelShader*	m_pPS = nullptr;

	ComPtr<ID3D11Buffer>				m_cpVertexBuf = nullptr;
	ComPtr<ID3D11Buffer>				m_cpIndexBuf = nullptr;
	ComPtr<ID3D11ShaderResourceView>	m_cpSRV = nullptr;			//!< テクスチャ

	std::vector<VERTEX>					m_Vertices;
	std::vector<FACE>					m_Faces;

	unsigned int						m_iDivX;			//!< X方向の分割数
	unsigned int						m_iDivY;			//!< Y方向の分割数
	unsigned int						m_iDivZ;			//!< Z方向の分割数

	void NormalizeVectors(XMFLOAT3 vector, XMFLOAT3& normal)
	{
		DXVec3Normalize(normal, vector);
	}

	void SetTopology(TopologyType type)
	{
		switch (type)
		{
		case TopologyType::TRIANGLELIST:
			DirectX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		case TopologyType::LINELIST:
			DirectX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		default:
			TopologyType::TRIANGLELIST;
			break;
		}
	}

	virtual void BindVertices(sRGBA) = 0;
	virtual void BindIndices();
	//! \fn void GenerateIndices(int SideToDivide, int offset)
	/*! \brief generate indices according offset
	 *  \brief オフセットによるインデックス生成
	 *  \param SideToDivide : side where division is applied | 分割を適用する方向
	 *  \param offset : offset count per division | 分割毎のオフセット
	 */
	virtual void GenerateIndices(int SideToDivide, int offset);

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

	virtual void ProcessTessellation() {}
	virtual void BindComputeShaders() {}

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
