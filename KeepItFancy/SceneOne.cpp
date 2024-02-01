#include "SceneOne.h"
#include "MPrimitive2D.h"
#include "MPrimitive3D.h"
#include "Terrain.h"
#include "Waves.h"

void SceneOne::Init()
{
	TPlane* pPlane = CreateObj<TPlane>("Plane");
	pPlane->Create();
	pPlane->SetBaseSRV(ASSET_PATH("img/HalLogo.jpg"));

	Terrain* pTerrain = CreateObj<Terrain>("Terrain");
	pTerrain->Create(20.0f, 20.0f, 20, 20);
	pTerrain->SetFrequency(2.5f);
	pTerrain->SetPosition(XMFLOAT3(0.0f, -1.3f, 0.0f));
	pTerrain->SetRotation(XMFLOAT3(0.0f, 180.0f, 0.0f));

	Waves* pWaves = CreateObj<Waves>("Waves");
	pWaves->Create(12.0f, 12.0f, 120, 120);
	pWaves->SetColor(sRGBA(51, 128, 204, 128));
	pWaves->SetUVTiling(XMFLOAT2(6.0f, 6.0f));
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

	Terrain* pTerrain = GetObj<Terrain>("Terrain");
	pTerrain->Update(tick);

	Waves* pWaves = GetObj<Waves>("Waves");

	ImGui::Begin("Customize Panel");
	if (ImGui::CollapsingHeader("Terrain Plane"))
	{
		static bool useWireframe = pTerrain->GetWireframeStatus();
		ImGui::Checkbox("Wireframe", &useWireframe);
		pTerrain->SetWireframe(useWireframe);
		ImGui::Spacing();

		ImGui::SeparatorText("Other Settings");
		static float freq = pTerrain->GetFrequency();
		ImGui::SetNextItemWidth(150.0f);
		ImGui::SliderFloat("Frequency", &freq, 0.0001f, 5.0f);
		pTerrain->SetFrequency(freq);
		ImGui::Spacing();
	}

	if (ImGui::CollapsingHeader("Water Plane"))
	{
		static bool useWireframe = pWaves->GetWireframeStatus();
		ImGui::Checkbox("Wireframe", &useWireframe);
		pWaves->SetWireframe(useWireframe);
		ImGui::Spacing();

		// needs better fix as this creates new buffers per frame
		/*
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
		ImGui::Spacing();

		ImGui::SeparatorText("Caustics Tiling");
		static XMFLOAT2 tiling = pWaves->GetUVTiling();
		ImGui::InputFloat2("UV Tiling", (float*)&tiling);
		pWaves->SetUVTiling(tiling);
		ImGui::Spacing();

		ImGui::SeparatorText("Tessellation");
		static bool useTess = pWaves->GetTessellationStatus();
		ImGui::Checkbox("Tessellation On", &useTess);
		pWaves->SetTessellation(useTess);

		if (useTess)
		{
			static float tessFactor = pWaves->GetTessellationFactor();
			ImGui::SetNextItemWidth(150.0f);
			ImGui::SliderFloat("Tessellation Factor", &tessFactor, 0.001f, 8.0f);
			pWaves->SetTessellationFactor(tessFactor);

			static float noiseScale = pWaves->GetNoiseFactor();
			ImGui::SetNextItemWidth(150.0f);
			ImGui::SliderFloat("Noise Scale", &noiseScale, 1.0f, 8.0f);
			pWaves->SetNoiseFactor(noiseScale);
		}
		ImGui::Spacing();

		ImGui::SeparatorText("Other Settings");
		static float freq = pWaves->GetFrequency();
		ImGui::SetNextItemWidth(150.0f);
		ImGui::SliderFloat("Wave Frequency", &freq, 0.0001f, 5.0f);
		pWaves->SetFrequency(freq);

		static float ampli = pWaves->GetAmplitude();
		ImGui::SetNextItemWidth(150.0f);
		ImGui::SliderFloat("Wave Amplitude", &ampli, 0.0001f, 0.0005f, "%.4f");
		pWaves->SetAmplitude(ampli);

		ImGui::Spacing();
	}

	ImGui::Text(" %.2f %.2f %.2f %.2f", pWaves->GetColor().r, pWaves->GetColor().g, pWaves->GetColor().b, pWaves->GetColor().a);
	ImGui::End();

	pWaves->Update(m_fTime);
}

void SceneOne::Draw()
{

	TPlane* pPlane = GetObj<TPlane>("Plane");
	pPlane->Draw();

	Terrain* pTerrain = GetObj<Terrain>("Terrain");
	pTerrain->Draw();

	DirectX11::SetBlendState(BlendType::ALPHA);
	Waves* pWaves = GetObj<Waves>("Waves");
	pWaves->Draw();
	DirectX11::SetBlendState(BlendType::NONE);
}
