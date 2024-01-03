#include "FreeCamera.h"
#include "BCamera.h"
#include "KeyInput.h"

void FreeCamera::Create(float radius, int segments, sRGBA _color)
{
	useLight = false;

	m_fRadius = radius;
	m_iSegments = segments;
	m_iSides = 3;

	BindVertices(_color);
	BindIndices();

	CreateDefaultBuffers();
	LoadDefaultShaders();
}

void FreeCamera::Draw()
{
#ifdef _DEBUG
	LINEBASE::Draw();
#endif // _DEBUG
}

void FreeCamera::Update(float tick)
{
	if (KEYINPUT::KeyPress(VK_HOME))
	{
		SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}

#ifdef PERSPECTIVE
	DirectX::XMFLOAT3 frontVec = GetForwardVector();

	if (KEYINPUT::KeyPress(VK_PRIOR))
		CAMERA::GetCamera()->m_fZoom -= 2.0f * tick;
	if (KEYINPUT::KeyPress(VK_NEXT))
		CAMERA::GetCamera()->m_fZoom += 2.0f * tick;

	// Pitch | Tilt
	if (KEYINPUT::KeyPress('I'))
	{
		m_Rotation.x -= 1.0f;
	}
	if (KEYINPUT::KeyPress('K'))
	{
		m_Rotation.x += 1.0f;
	}
	// Yaw | Panning
	if (KEYINPUT::KeyPress(VK_LEFT))
	{
		m_Rotation.y -= 1.0f;
	}
	if (KEYINPUT::KeyPress(VK_RIGHT))
	{
		m_Rotation.y += 1.0f;
	}

	// 移動処理
	if (KEYINPUT::KeyPress(VK_UP))
	{
		m_Position.x += frontVec.x * 1.0f * tick;
		m_Position.y += frontVec.y * 1.0f * tick;
		m_Position.z += frontVec.z * 1.0f * tick;
	}
	if (KEYINPUT::KeyPress(VK_DOWN))
	{
		m_Position.x -= frontVec.x * 1.0f * tick;
		m_Position.y -= frontVec.y * 1.0f * tick;
		m_Position.z -= frontVec.z * 1.0f * tick;
	}
#endif // PERSPECTIVE

#ifdef ORTHOGRAPHIC
#endif // ORTHOGRAPHIC

	// 追尾処理
	// create a back vector which the camera will use to follow
	DirectX::XMFLOAT3 backVec;
	backVec.x = -frontVec.x;
	backVec.y = -frontVec.y;
	backVec.z = -frontVec.z;
	
	// set camera position at the back
	DirectX::XMFLOAT3 camPos = CAMERA::GetCamera()->GetPosition();
	camPos.x = (m_Position.x) + (backVec.x * CAMERA::GetCamera()->m_fZoom);
	camPos.y = (m_Position.y) + (backVec.y * CAMERA::GetCamera()->m_fZoom) + 3.0f;
	camPos.z = (m_Position.z) + (backVec.z * CAMERA::GetCamera()->m_fZoom);

	static DirectX::XMFLOAT3 lastCamPos;
	/*
	// 緩やか処理
	float blendFactor = 0.990f;				// 平均化の重み
	camPos.x = (lastCamPos.x * blendFactor) + camPos.x * (1.0f - blendFactor);
	camPos.y = (lastCamPos.y * blendFactor) + camPos.y * (1.0f - blendFactor);
	camPos.z = (lastCamPos.z * blendFactor) + camPos.z * (1.0f - blendFactor);
	*/
	CAMERA::GetCamera()->SetCamPos(camPos);
	lastCamPos = camPos;

	// set focus point to front
	DirectX::XMFLOAT3 newCamFocus;
	newCamFocus.x = (m_Position.x) + frontVec.x;
	newCamFocus.y = (m_Position.y) + frontVec.y;
	newCamFocus.z = (m_Position.z) + frontVec.z;
	CAMERA::GetCamera()->SetFocus(newCamFocus);

	// apply new camera view matrix
	CAMERA::GetCamera()->UpdateCamera();
}
