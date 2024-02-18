#include "SceneOne.h"
#include "MPrimitive2D.h"
#include "MPrimitive3D.h"
#include "Terrain.h"
#include "Waves.h"
#include "AnObject.h"

void SceneOne::Init()
{
#ifdef _DEBUG
	TPlane* pPlane = CreateObj<TPlane>("Plane");
	pPlane->Create();
	pPlane->SetBaseSRV(ASSET_PATH("img/HalLogo.jpg"));

	AnObject* pObject = CreateObj<AnObject>("Object");
	pObject->Create();
#endif // _DEBUG

	Terrain* pTerrain = CreateObj<Terrain>("Terrain");
	pTerrain->Create(30.0f, 30.0f, 60, 60);
	pTerrain->SetFrequency(2.5f);
	pTerrain->SetPosition(XMFLOAT3(0.0f, -1.3f, 0.0f));
	pTerrain->SetRotation(XMFLOAT3(0.0f, 180.0f, 0.0f));

	Waves* pWaves = CreateObj<Waves>("Waves");
	pWaves->SetPosition(XMFLOAT3(0.0f, 0.0f, 5.0f));
	pWaves->Create(20.0f, 20.0f, 200, 200);
	pWaves->SetBaseColor(sRGBA(51, 128, 204, 128));
	pWaves->SetUVTiling(XMFLOAT2(20.0f, 20.0f));
	pWaves->SetTessellation(true);
}

void SceneOne::Release()
{
	DestroyObj("Waves");
}

void SceneOne::Update(float tick)
{
	m_fTime += tick;

	Terrain* pTerrain = GetObj<Terrain>("Terrain");
	pTerrain->Update(tick);

	Waves* pWaves = GetObj<Waves>("Waves");
	pWaves->Update(m_fTime);

#ifdef _DEBUG
	TPlane* pPlane = GetObj<TPlane>("Plane");
	pPlane->Update(tick);

	AnObject* pObject = GetObj<AnObject>("Object");
	pObject->Update(tick);

	ImGui::Begin("Customize Panel");
	if (ImGui::CollapsingHeader("Terrain Plane"))
	{
		static bool useWireframe = pTerrain->GetWireframeStatus();
		ImGui::Checkbox("Wireframe", &useWireframe);
		pTerrain->SetWireframe(useWireframe);
		ImGui::Spacing();

		ImGui::SeparatorText("Color");
		static sRGBA landColor = pTerrain->GetBaseColor();
		ImGui::ColorEdit4("Land Color", (float*)&landColor);
		pTerrain->SetBaseColor(landColor);
		static sRGBA overlayColor = pTerrain->GetTerrainColor();
		ImGui::ColorEdit4("Terrain Color", (float*)&overlayColor);
		pTerrain->SetTerrainColor(overlayColor);
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
		/*ImGui::SeparatorText("Shaders");
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
		}*/

		ImGui::SeparatorText("Color");
		static sRGBA newColor = pWaves->GetBaseColor();
		ImGui::ColorEdit4("Base Color", (float*)&newColor);
		pWaves->SetBaseColor(newColor);
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

	ImGui::Text(" %.2f %.2f %.2f %.2f", pWaves->GetBaseColor().r, pWaves->GetBaseColor().g, pWaves->GetBaseColor().b, pWaves->GetBaseColor().a);
	ImGui::End();

	//ImGui::Begin("test");
	//ImGui::End();

#endif // _DEBUG

}

void SceneOne::Draw()
{
#ifdef _DEBUG
	//TPlane* pPlane = GetObj<TPlane>("Plane");
	//pPlane->Draw();

	//AnObject* pObject = GetObj<AnObject>("Object");
	//pObject->Draw();
#endif // _DEBUG

	Terrain* pTerrain = GetObj<Terrain>("Terrain");
	pTerrain->Draw();

	DirectX11::SetBlendState(BlendType::ALPHA);
	Waves* pWaves = GetObj<Waves>("Waves");
	pWaves->Draw();
	DirectX11::SetBlendState(BlendType::NONE);
}
