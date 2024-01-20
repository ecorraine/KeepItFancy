#include "FreeCamera.h"
#include "BCamera.h"
#include "KeyInput.h"

void FreeCamera::Create(float radius, int segments)
{
	useLight = false;

	m_fRadius = radius;
	m_iSegments = segments;
	m_iSides = 3;

	BindVertices();
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
	const float moveSpeed = 10.0f * tick;
	const float turnSpeed = 50.0f * tick;

	if (KEYINPUT::KeyPress(VK_HOME))
	{
		SetPosition(XMFLOAT3(0.0f, 1.0f, -1.0f));
		SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	}

#ifdef PERSPECTIVE
	DirectX::XMFLOAT3 frontVec = GetForwardVector();

	if (KEYINPUT::KeyPress(VK_PRIOR))
		CAMERA::GetCamera()->m_fZoom -= moveSpeed;
	if (KEYINPUT::KeyPress(VK_NEXT))
		CAMERA::GetCamera()->m_fZoom += moveSpeed;

	// Pitch | Tilt
	if (KEYINPUT::KeyPress('I'))
	{
		m_Rotation.x -= turnSpeed;
	}
	if (KEYINPUT::KeyPress('K'))
	{
		m_Rotation.x += turnSpeed;
	}
	// Yaw | Panning
	if (KEYINPUT::KeyPress(VK_LEFT))
	{
		m_Rotation.y -= turnSpeed;
	}
	if (KEYINPUT::KeyPress(VK_RIGHT))
	{
		m_Rotation.y += turnSpeed;
	}

	// 移動処理
	if (KEYINPUT::KeyPress(VK_UP))
	{
		m_Position.x += frontVec.x * moveSpeed;
		m_Position.y += frontVec.y * moveSpeed;
		m_Position.z += frontVec.z * moveSpeed;
	}
	if (KEYINPUT::KeyPress(VK_DOWN))
	{
		m_Position.x -= frontVec.x * moveSpeed;
		m_Position.y -= frontVec.y * moveSpeed;
		m_Position.z -= frontVec.z * moveSpeed;
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
	camPos.y = (m_Position.y) + (backVec.y * CAMERA::GetCamera()->m_fZoom);
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

	MESH::Update(tick);
}
