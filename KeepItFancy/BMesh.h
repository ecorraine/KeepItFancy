#ifndef BMESH_H
#define BMESH_H

#include "GameObject.h"
#include "Buffers.h"
#include "srgba.hpp"
#include "myUtilities.hpp"

///--------------------------------------------------
//! \enum Topology Type
/*! \brief List of Topology types to use for rendering
 *  \brief 描画時に使うトポロジー
 */
enum class TopologyType
{
	LINELIST,						//!< 線形
	TRIANGLELIST,					//!< 三角形
	THREE_CONTROL_POINT_PATCHLIST,	//!< パッチリスト
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
	HullShader* m_pHS = nullptr;
	DomainShader* m_pDS = nullptr;
	PixelShader* m_pPS = nullptr;
	ComputeShader* m_pCS = nullptr;

	ComPtr<ID3D11Buffer>				m_cpVertexBuf = nullptr;
	ComPtr<ID3D11Buffer>				m_cpIndexBuf = nullptr;
	ComPtr<ID3D11ShaderResourceView>	m_cpSRV = nullptr;			//!< テクスチャ

	std::vector<VERTEX>					m_Vertices;

private:
	sRGBA	m_baseColor = sRGBA();
	sRGBA	m_emissiveColor = sRGBA();
	float	m_fEmissivePower = 1.0f;
	float	m_fShininess = 0.0f;
	float	m_fSpecularPower = 0.0f;
	float	m_fRoughness = 0.0f;
	float	m_fMetallic = 0.0f;
	float	m_fAmbientOcclusion = 0.0f;
	float	m_fUVTiling[2] = { 1, 1 };

protected:
	float	m_fTessellationFactor = 2.0f;
	bool	m_useTessellation = false;
	bool	m_useWireframe = false;
	bool	m_isUsingTexture = false;

public:
	~MESH()
	{
		if (m_pPS)
		{
			delete m_pPS;
			m_pPS = nullptr;
		}
		if (m_pDS)
		{
			delete m_pDS;
			m_pDS = nullptr;
		}
		if (m_pHS)
		{
			delete m_pHS;
			m_pHS = nullptr;
		}
		if (m_pVS)
		{
			delete m_pVS;
			m_pVS = nullptr;
		}
		if (m_pCS)
		{
			delete m_pCS;
			m_pCS = nullptr;
		}

		m_Vertices.erase(m_Vertices.begin(), m_Vertices.end());
		m_Vertices.clear();
	}

protected:
	void NormalizeVectors(XMFLOAT3 vector, XMFLOAT3& normal)
	{
		DXVec3Normalize(normal, vector);
	}

	virtual void BindIndices() = 0;
	virtual void BindVertices() = 0;

	virtual void CreateDefaultBuffers() = 0;
	virtual void LoadDefaultShaders() = 0;

	virtual void ProcessTessellation() {}
	virtual void BindComputeShaders() {}

	static void SetTopology(TopologyType topology)
	{
		switch (topology)
		{
		case TopologyType::LINELIST:
			DirectX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		case TopologyType::TRIANGLELIST:
			DirectX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		case TopologyType::THREE_CONTROL_POINT_PATCHLIST:
			DirectX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
			break;
		}
	}

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
	void SetColor(const sRGBA& _color) { m_baseColor = _color; }
	const sRGBA GetColor() const { return m_baseColor; }

	const XMFLOAT2 GetUVTiling() const { return XMFLOAT2(m_fUVTiling[0], m_fUVTiling[1]); }
	void SetUVTiling(const XMFLOAT2& value)
	{
		m_fUVTiling[0] = value.x;
		m_fUVTiling[1] = value.y;
	}

	const bool GetWireframeStatus() const { return m_useWireframe; }
	void SetWireframe(bool value) { m_useWireframe = value; }

	const bool GetTessellationStatus() const { return m_useTessellation; }
	void SetTessellation(bool value) { m_useTessellation = value; }

	const float GetTessellationFactor() const { return m_fTessellationFactor; }
	void SetTessellationFactor(const float& value) { m_fTessellationFactor = value; }

	void SetSRV(const char* file)
	{
		m_isUsingTexture = true;

		TEXTURE* texture = new TEXTURE();
		texture->CreateTexture(file);

		m_cpSRV = texture->GetSRV();
	}

	void Update(float tick) override
	{
		XMFLOAT4 data[] = {
			{ m_baseColor.r, m_baseColor.g, m_baseColor.b, m_baseColor.a },
			{ tick, static_cast<float>(m_isUsingTexture), m_fUVTiling[0], m_fUVTiling[1] }
		};

		m_pPS->SendToBuffer(0, &data);
	};

	void ChangeShader(SHADER::ShaderType shadertype, const char* _file)
	{
		switch (shadertype)
		{
		case SHADER::VertexS:
			m_pVS->LoadShader(_file);
			break;
		case SHADER::HullS:
			break;
		case SHADER::DomainS:
			break;
		case SHADER::GeometryS:
			break;
		case SHADER::PixelS:
			m_pPS->LoadShader(_file);
			break;
		case SHADER::ComputeS:
			break;
		}
	}
};

#endif // !BMESH_H
