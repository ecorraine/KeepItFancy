#ifndef SKYBOX_H
#define SKYBOX_H

#include "MPrimitive3D.h"

class SkyBox final : public Sphere
{
public:
	SkyBox() {}
	~SkyBox() {}

	void Create(float radius = 50.0f, int divisions = 500, sRGBA _color = sRGBA())
	{
		//useLight = false;

		m_fRadius = radius;
		m_iDivX = m_iDivY = divisions;

		SetSRV(ASSET_PATH("img/sky.jpg"));

		BindVertices();
		BindIndices();

		CreateDefaultBuffers();
		LoadDefaultShaders();
	}

	void Draw()
	{
		TRIANGLEBASE::Draw(RasterType::SOLID_NO_CULL);
	}

protected:
	void LoadDefaultShaders() override
	{
		m_pVS = AddComponent<VertexShader>();
		m_pVS->LoadShader(SHADER_PATH("VS_WorldPosition.cso"));

		m_pPS = AddComponent<PixelShader>();
		m_pPS->LoadShader(SHADER_PATH("PS_FlatColor.cso"));
	}
};


#endif // !SKYBOX_H
