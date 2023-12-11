#include "ScTitle.h"
#include "MPrimitives.h"
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
	pWaves->Update(m_fTime);
}

void ScTitle::Draw()
{
	Waves* pWaves = GetObj<Waves>("Waves");
	pWaves->Draw(RasterType::WIREFRAME_NO_CULL);
}
