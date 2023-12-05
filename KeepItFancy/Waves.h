#pragma once
#include "BMesh.h"

class Waves : public MESH
{
private:
	ComputeShader*						m_pCS;
	ComPtr<ID3D11UnorderedAccessView>	m_cpVertexBufUAV;
	ComPtr<ID3D11Buffer>				m_cpStagingBuffer;

	float								m_fWidth;
	float								m_fDepth;

public:
	Waves() :
		m_pCS(nullptr),
		m_cpVertexBufUAV(nullptr),
		m_cpStagingBuffer(nullptr),
		m_fWidth(0),
		m_fDepth(0)
	{}

protected:
	void BindVertices(sRGBA color);

public:
	void Create(float width, float depth, int divX = 10, int divY = 10, sRGBA color = sRGBA(135, 206, 235));
	void BindComputeShaders();
	void Update(float tick);
};

