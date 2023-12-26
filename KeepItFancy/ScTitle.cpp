#include "ScTitle.h"
#include "Waves.h"

void ScTitle::Init()
{
	Waves* pWaves = CreateObj<Waves>("Waves");
	pWaves->Create(3.0f, 3.0f, 50, 50);

	//FlatCircle* pCircle = CreateObj<FlatCircle>("Circle");
	//pCircle->Create();

}

void ScTitle::Release()
{
	DestroyObj("Waves");
}

void ScTitle::Update(float tick)
{
	m_fTime += tick;

	Waves* pWaves = GetObj<Waves>("Waves");
	pWaves->Update(m_fTime);
}

void ScTitle::Draw()
{
	Waves* pWaves = GetObj<Waves>("Waves");
	pWaves->Draw(RasterType::CULL_BACK);

	//FlatCircle* pCircle = GetObj<FlatCircle>("Circle");
	//pCircle->Draw();
}
