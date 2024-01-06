#include "RootScene.h"
#include "FreeCamera.h"
#include "SkyBox.h"
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
	CAMERA::g_Camera.InitCamera();
	FreeCamera* pCamera = CreateObj<FreeCamera>("Camera");
	pCamera->Create();
	pCamera->SetPosition(XMFLOAT3(0.0f, 1.0f, -1.0f));
	pCamera->SetRotation(XMFLOAT3(-30.0f, 0.0f, 0.0f));

	SkyBox* pSky = CreateObj<SkyBox>("Sky");
	pSky->Create();

	ChangeScene();
}

void RootScene::Release()
{
}

void RootScene::Update(float tick)
{
	FreeCamera* pCamera = GetObj<FreeCamera>("Camera");
	pCamera->Update(tick);

	ImGui::Begin("Camera");
	ImGui::Text("Position: %.2f, %.2f, %.2f", pCamera->GetPosition().x, pCamera->GetPosition().y, pCamera->GetPosition().z);
	ImGui::Text("Rotation: %.2f, %.2f, %.2f", pCamera->GetRotation().x, pCamera->GetRotation().y, pCamera->GetRotation().z);
	ImGui::End();

	/*
	int idx = (int)m_eSceneIndex;
	if (KEYINPUT::KeyPress(VK_LEFT))
		--idx;
	if (KEYINPUT::KeyPress(VK_RIGHT))
		++idx;
	if (idx < 0) idx = (int)SceneList::SCENE_MAX - 1;
	if (idx >= (int)SceneList::SCENE_MAX) idx = 0;

	if (idx != (int)m_eSceneIndex)
	{
		m_eSceneIndex = (SceneList)idx;
		RemoveSubScene();
		ChangeScene();
	}
	*/
}

void RootScene::Draw()
{
	FreeCamera* pCamera = GetObj<FreeCamera>("Camera");
	pCamera->Draw();

	SkyBox* pSky = GetObj<SkyBox>("Sky");
	pSky->Draw();
}
