#include "RootScene.h"
#include "FreeCamera.h"
#include "MPrimitives.h"
#include "ScTitle.h"

void RootScene::ChangeScene()
{
	switch ((int)m_eSceneIndex)
	{
	default:
	case (int)SceneList::TITLE:	AddSubScene<ScTitle>(); break;
	//case (int)SceneList::DEBUG:	AddSubScene<ScDebug>(); break;
	//case (int)SceneList::STAGE:	AddSubScene<ScStage>(); break;
	}
}

void RootScene::Init()
{
	Sphere* pSky = CreateObj<Sphere>("Sky");
	pSky->Create(50.0f, 500);

	ChangeScene();
}

void RootScene::Release()
{
}

void RootScene::Update(float tick)
{
	m_fTime += tick;

	int idx = (int)m_eSceneIndex;
	/*
	if (KEYINPUT::KeyPress(VK_LEFT))
		--idx;
	if (KEYINPUT::KeyPress(VK_RIGHT))
		++idx;
	*/
	if (idx < 0) idx = (int)SceneList::SCENE_MAX - 1;
	if (idx >= (int)SceneList::SCENE_MAX) idx = 0;

	if (idx != (int)m_eSceneIndex)
	{
		m_eSceneIndex = (SceneList)idx;
		RemoveSubScene();
		ChangeScene();
	}

	FreeCamera::GetCamera()->UpdateCamera();
}

void RootScene::Draw()
{
	FreeCamera::GetCamera()->DrawCamera();

	Sphere* pSky = GetObj<Sphere>("Sky");
	pSky->Draw(/*RasterType::CULL_FRONT*/);
}
