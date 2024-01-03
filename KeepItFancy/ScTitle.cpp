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
	pWaves->Update(m_fTime);
}

void ScTitle::Draw()
{
	DirectX11::SetBlendState(BlendType::ALPHA);

	Waves* pWaves = GetObj<Waves>("Waves");
	pWaves->Draw();

	DirectX11::ClearBlendState();

	DirectX11::SetDepthStencilState(DepthStencilState::DEPTH_STENCIL_WRITE);
}
