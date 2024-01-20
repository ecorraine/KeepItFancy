#include "SceneOne.h"
#include "MPrimitive2D.h"
#include "MPrimitive3D.h"
#include "Waves.h"

void SceneOne::Init()
{
	TPlane* pPlane = CreateObj<TPlane>("Plane");
	pPlane->Create();

	Waves* pWaves = CreateObj<Waves>("Waves");
	pWaves->Create(3.0f, 3.0f, 50, 50);
	pWaves->SetColor(sRGBA(0.2f, 0.5f, 0.8f, 0.8f));
}

void SceneOne::Release()
{
	DestroyObj("Waves");
}

void SceneOne::Update(float tick)
{
	m_fTime += tick;

	TPlane* pPlane = GetObj<TPlane>("Plane");
	pPlane->Update(tick);

	Waves* pWaves = GetObj<Waves>("Waves");
	ImGui::Begin("Wave Plane");

	/*
	// needs better fix
	ImGui::SeparatorText("Shaders");
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
	*/

	ImGui::SeparatorText("Color");
	static sRGBA newColor = pWaves->GetColor();
	ImGui::ColorEdit4("Base Color", (float*)&newColor);
	pWaves->SetColor(newColor);

	ImGui::SeparatorText("Other Settings");
	static float freq = pWaves->GetFrequency();
	ImGui::SliderFloat("Wave Frequency", &freq, 0.0001f, 5.0f);
	pWaves->SetFrequency(freq);

	static float ampli = pWaves->GetAmplitude();
	ImGui::SliderFloat("Wave Amplitude", &ampli, 0.0001f, 0.0005f, "%.4f");
	pWaves->SetAmplitude(ampli);

	pPlane->SetColor(newColor);
	ImGui::Text(" %.2f %.2f %.2f %.2f", pWaves->GetColor().r, pWaves->GetColor().g, pWaves->GetColor().b, pWaves->GetColor().a);
	ImGui::End();

	pWaves->Update(m_fTime);
}

void SceneOne::Draw()
{
	TPlane* pPlane = GetObj<TPlane>("Plane");
	pPlane->Draw();

	Waves* pWaves = GetObj<Waves>("Waves");
	pWaves->Draw();
}
