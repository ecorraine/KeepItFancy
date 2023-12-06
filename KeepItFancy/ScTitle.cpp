#include "ScTitle.h"
#include "MPrimitives.h"
#include "Waves.h"

void ScTitle::Init()
{
	Waves* pWaves = CreateObj<Waves>("Waves");
	pWaves->Create(3.0f, 3.0f);

}

void ScTitle::Release()
{
}

void ScTitle::Update(float tick)
{
	Waves* pWaves = GetObj<Waves>("Waves");
	pWaves->Update(tick);

	//printf_s("tick: %.3f | ", tick);
}

void ScTitle::Draw()
{
	Waves* pWaves = GetObj<Waves>("Waves");
	pWaves->Draw(RasterType::WIREFRAME_NO_CULL);
}
