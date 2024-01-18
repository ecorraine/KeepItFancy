#include "ScTitle.h"
#include "MPrimitive2D.h"
#include "MPrimitive3D.h"
#include "Waves.h"

void ScTitle::Init()
{

	Waves* pWaves = CreateObj<Waves>("Waves");
	pWaves->Create(3.0f, 3.0f, 50, 50);
}

void ScTitle::Release()
{
	DestroyObj("Waves");
}

void ScTitle::Update(float tick)
{
	m_fTime += tick;

	Waves* pWaves = GetObj<Waves>("Waves");
	ImGui::Begin("Wave Plane");

	const char* items[] = { "SimpleCaustics", "SimpleVoronoi" };
	static int selection = 1;
	ImGui::Combo("Pixel Shader", &selection, items, IM_ARRAYSIZE(items));

	switch (selection)
	{
	case 0:
		pWaves->ChangeShader(SHADER::PixelS, SHADER_PATH("PS_CausticsSimple.cso"));
		break;
	case 1:
		pWaves->ChangeShader(SHADER::PixelS, SHADER_PATH("PS_CausticsVoronoi.cso"));
		break;
	}

	static sRGBA newColor = pWaves->GetColor();
	ImGui::ColorEdit4("Color", (float*)&newColor);
	pWaves->ChangeColor(newColor);
	ImGui::Text(" %.2f %.2f %.2f %.2f", pWaves->GetColor().r, pWaves->GetColor().g, pWaves->GetColor().b, pWaves->GetColor().a);
	ImGui::End();

	pWaves->Update(m_fTime);
}

void ScTitle::Draw()
{

	Waves* pWaves = GetObj<Waves>("Waves");
	pWaves->Draw();
}
