#ifndef BLIGHT_H
#define BLIGHT_H

#include <DirectXMath.h>
#include "srgba.hpp"

class LIGHT
{
protected:
	DirectX::XMFLOAT3 m_Position = { 1.0f, 1.0f, -1.0f };
	DirectX::XMFLOAT3 m_Direction = { -1.0f, -1.0f, 1.0f };
	DirectX::XMFLOAT4 m_Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4 m_Ambient = { 0.3f, 0.3f, 0.3f, 1.0f };

public:
	static LIGHT* GetLight()
	{
		static LIGHT instance;

		return &instance;
	}

	const DirectX::XMFLOAT4 GetLightDirection() const
	{
		return DirectX::XMFLOAT4(m_Direction.x, m_Direction.y, m_Direction.z, 0.0f);
	}
	const DirectX::XMFLOAT4 GetLightDiffuse() const
	{
		return m_Diffuse;
	}
	const DirectX::XMFLOAT4 GetLightAmbient() const
	{
		return m_Ambient;
	}

	void SetLightDiffuse(const sRGBA color)
	{
		m_Diffuse = { color.r, color.g, color.b, color.a };
	}

	void SetLightAmbient(const sRGBA color)
	{
		m_Ambient = { color.r, color.g, color.b, color.a };
	}
};


#endif // !BLIGHT_H
