#pragma once
#include "MeshCommons.h"

class Waves : public TRIANGLEBASE
{
private:
	ComputeShader*						m_pCS = nullptr;

	ComPtr<ID3D11Buffer>				pOutputBuffer = nullptr;
	ComPtr<ID3D11UnorderedAccessView>	pOutputBufferUAV = nullptr;
	ComPtr<ID3D11Buffer>				m_cpStagingBuffer = nullptr;

	XMFLOAT4 cbData[3] = {};

	float								m_fWidth;
	float								m_fDepth;

public:
	Waves() :
		m_fWidth(0),
		m_fDepth(0)
	{}
	~Waves()
	{
		delete m_pCS;
	}

	void Create(float width, float depth, int divX = 10, int divY = 10, sRGBA _color = sRGBA(173, 216, 230));
	void BindComputeShaders();
	void Update(float tick);

protected:
	virtual void LoadDefaultShaders()
	{
		m_pCS = AddComponent<ComputeShader>();
		m_pCS->LoadShader(SHADER_PATH("CS_NoiseWaves.cso"));

		m_pVS = AddComponent<VertexShader>();
		m_pVS->LoadShader(SHADER_PATH("VS_WorldPosition.cso"));

		m_pPS = AddComponent<PixelShader>();
		m_pPS->LoadShader(SHADER_PATH("PS_CausticsVoronoi.cso"));
	}

	void BindVertices(sRGBA);
};

