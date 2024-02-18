#include "RootScene.h"
#include "Profiler.h"
#include "FreeCamera.h"
#include "SkyBox.h"
#include "SceneOne.h"

void RootScene::ChangeScene()
{
	switch ((int)m_eSceneIndex)
	{
	default:
	//case (int)SceneList::DEBUG:	AddSubScene<SceneDebug>(); break;
	case (int)SceneList::STAGE:	AddSubScene<SceneOne>(); break;
	}
}

void RootScene::Init()
{
	SkyBox* pSky = CreateObj<SkyBox>("Sky");
	pSky->Create();

	CAMERA::g_Camera.InitCamera();
	FreeCamera* pCamera = CreateObj<FreeCamera>("Camera");
	pCamera->Create();
	pCamera->SetPosition(XMFLOAT3(0.0f, 1.0f, -1.0f));
	pCamera->SetRotation(XMFLOAT3(-20.0f, 0.0f, 0.0f));

	ChangeScene();
}

void RootScene::Release()
{
}

void RootScene::Update(float tick)
{
	SkyBox* pSky = GetObj<SkyBox>("Sky");
	pSky->Update(tick);

	FreeCamera* pCamera = GetObj<FreeCamera>("Camera");
	pCamera->Update(tick);

#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::SeparatorText(u8"操作");
	ImGui::Text(u8"方向キー：カメラ移動");
	ImGui::Text(u8"I、K：カメラ角度調整");
	ImGui::SeparatorText("Info");
	ImGui::Text("Position: %.2f, %.2f, %.2f", pCamera->GetPosition().x, pCamera->GetPosition().y, pCamera->GetPosition().z);
	ImGui::Text("Rotation: %.2f, %.2f, %.2f", pCamera->GetRotation().x, pCamera->GetRotation().y, pCamera->GetRotation().z);
	ImGui::End();
#endif // _DEBUG

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
	SkyBox* pSky = GetObj<SkyBox>("Sky");
	pSky->Draw();

	FreeCamera* pCamera = GetObj<FreeCamera>("Camera");
	pCamera->Draw();
}
