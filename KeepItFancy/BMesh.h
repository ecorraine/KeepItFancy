#ifndef BMESH_H
#define BMESH_H

#include "GameObject.h"
#include "Buffers.h"
#include "srgba.hpp"
#include "myUtilities.hpp"

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! MESH Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class MESH Mesh.h "Mesh.h"
/*! \brief Base Class for Meshes
 *  \brief メッシュ親クラス
 */
class MESH : public GameObject
{
public:
	struct VERTEX
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
		XMFLOAT4 color;
		XMFLOAT3 normal;
	};

protected:
	VertexShader* m_pVS = nullptr;
	PixelShader* m_pPS = nullptr;

	ComPtr<ID3D11Buffer>				m_cpVertexBuf = nullptr;
	ComPtr<ID3D11Buffer>				m_cpIndexBuf = nullptr;
	ComPtr<ID3D11ShaderResourceView>	m_cpSRV = nullptr;			//!< テクスチャ

	std::vector<VERTEX>					m_Vertices;

public:
	~MESH()
	{
		delete m_pPS;
		delete m_pVS;

		m_Vertices.erase(m_Vertices.begin(), m_Vertices.end());
		m_Vertices.clear();
	}

protected:
	void NormalizeVectors(XMFLOAT3 vector, XMFLOAT3& normal)
	{
		DXVec3Normalize(normal, vector);
	}

	virtual void BindIndices() = 0;
	virtual void BindVertices(sRGBA) = 0;

	virtual void CreateDefaultBuffers() = 0;
	virtual void LoadDefaultShaders() = 0;

	virtual void ProcessTessellation() {}
	virtual void BindComputeShaders() {}

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

public:
	void SetSRV(const char* file)
	{
		TEXTURE* texture = new TEXTURE();
		texture->CreateTexture(file);
		m_cpSRV = texture->GetSRV();
	}
};

#endif // !BMESH_H
