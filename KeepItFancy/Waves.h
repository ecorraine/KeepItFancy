#pragma once
#include "MPrimitive2D.h"

class Waves final : public TPlane
{
private:
	ComPtr<ID3D11Buffer>				pOutputBuffer = nullptr;
	ComPtr<ID3D11UnorderedAccessView>	pOutputBufferUAV = nullptr;
	ComPtr<ID3D11Buffer>				m_cpStagingBuffer = nullptr;

	XMFLOAT4 cbData[3] = {};
	float m_fFrequency;
	float m_fAmplitude;

public:
	Waves() :
		m_fFrequency(1.0f),
		m_fAmplitude(0.0003f)
	{}
	~Waves() {}

	void Create(float width, float depth, int divX = 10, int divY = 10) override;
	void BindComputeShaders();
	void Update(float tick);

	void SetFrequency(const float& _value) { m_fFrequency = _value; }
	const float GetFrequency() const { return m_fFrequency; }
	void SetAmplitude(const float& _value) { m_fAmplitude = _value; }
	const float GetAmplitude() const { return m_fAmplitude; }

protected:
	virtual void LoadDefaultShaders()
	{
		m_pCS = AddComponent<ComputeShader>();
		m_pCS->LoadShader(SHADER_PATH("CS_NoiseWaves.cso"));

		m_pVS = AddComponent<VertexShader>();
		m_pVS->LoadShader(SHADER_PATH("VS_WorldPosition.cso"));

		m_pHS = AddComponent<HullShader>();
		m_pHS->LoadShader(SHADER_PATH("HS_Default.cso"));

		m_pDS = AddComponent<DomainShader>();
		m_pDS->LoadShader(SHADER_PATH("DS_Default.cso"));

		m_pPS = AddComponent<PixelShader>();
		m_pPS->LoadShader(SHADER_PATH("PS_CausticsVoronoi.cso"));
	}

	void ProcessTessellation() override
	{
		m_pHS->SendToBuffer(1, (void*)&m_fTessellationFactor);
		m_pHS->BindShader();

		m_pDS->SendToBuffer(1, (void*)&m_fTessellationFactor);
		m_pDS->BindShader();
	}
};

