#include "BCamera.h"
#include "FreeCamera.h"
#include "MPrimitives.h"

CAMERA* FreeCamera::m_Camera = nullptr;

FreeCamera::FreeCamera()
{
	m_Camera = CAMERA::GetCameraInstance();

	Cube* host = new Cube();
	host->Create(0.1f, 0.1f, 0.1f);
	m_pHost = host;

	if (host)
	{
		delete host;
		host = nullptr;
	}
}

void FreeCamera::Draw()
{
#ifdef _DEBUG
	dynamic_cast<Cube*>(m_pHost)->Draw(RasterType::WIREFRAME_CULL_BACK);
#endif // _DEBUG
}

void FreeCamera::Update(float tick)
{
	if (KEYINPUT::KeyPress(VK_HOME))
		m_fTilt = m_fPan = 0.0f;

#ifdef PERSPECTIVE
	if (KEYINPUT::KeyPress(VK_PRIOR))
		m_Camera->m_fZoom -= 1.0f;
	if (KEYINPUT::KeyPress(VK_NEXT))
		m_Camera->m_fZoom += 1.0f;

	if (KEYINPUT::KeyPress(VK_UP))
		m_pHost->m_Position.z += 0.5f;
	if (KEYINPUT::KeyPress(VK_DOWN))
		m_pHost->m_Position.z -= 0.5f;

	if (KEYINPUT::KeyPress(VK_LEFT))
		m_pHost->m_Rotation.y -= 1.0f;
	if (KEYINPUT::KeyPress(VK_RIGHT))
		m_pHost->m_Rotation.y += 1.0f;
#endif // PERSPECTIVE
#ifdef ORTHOGRAPHIC
	if (KEYINPUT::KeyPress(VK_UP))
		m_fTilt += 0.05f;
	if (KEYINPUT::KeyPress(VK_DOWN))
		m_fTilt -= 0.05f;

	if (KEYINPUT::KeyPress(VK_LEFT))
		m_fPan -= 0.05f;
	if (KEYINPUT::KeyPress(VK_RIGHT))
		m_fPan += 0.05f;
#endif // ORTHOGRAPHIC

	///---------- 追尾処理 ----------
	DirectX::XMFLOAT3 frontVec = m_pHost->GetForwardVector();

	DirectX::XMFLOAT3 backVec;
	// 前向きベクトルを反転させて後ろ向きベクトルを作る
	backVec.x = -frontVec.x;
	backVec.y = -frontVec.y;
	backVec.z = -frontVec.z;
	
	DirectX::XMFLOAT3 camPos;
	// 後ろ向きベクトルにカメラを置く
#ifdef PERSPECTIVE
	camPos.x = (m_pHost->m_Position.x + m_fPan) + (backVec.x * m_Camera->m_fZoom);
	camPos.y = (m_pHost->m_Position.y + m_fTilt) + (backVec.y * m_Camera->m_fZoom) + 0.5f;
	camPos.z = (m_pHost->m_Position.z) + (backVec.z * m_Camera->m_fZoom);
#endif // PERSPECTIVE
#ifdef ORTHHOGRAPHIC
	camPos.x = (m_pHost->m_Position.x + m_fPan) + backVec.x;
	camPos.y = (m_pHost->m_Position.y + m_fTilt) + backVec.y + 0.5f;
	camPos.z = (m_pHost->m_Position.z) + backVec.z;
#endif // ORTHHOGRAPHIC

	///--------- 緩やか処理 ---------
	static DirectX::XMFLOAT3 lastCamPos;
	float blendFactor = 0.990f;				// 平均化の重み
	camPos.x = (lastCamPos.x * blendFactor) + camPos.x * (1.0f - blendFactor);
	camPos.y = (lastCamPos.y * blendFactor) + camPos.y * (1.0f - blendFactor);
	camPos.z = (lastCamPos.z * blendFactor) + camPos.z * (1.0f - blendFactor);
	m_Camera->SetCamPos(camPos);

	lastCamPos = camPos;					// 1フレーム前のカメラ位置を保存

	///--------- 新しい注視点を設置
	DirectX::XMFLOAT3 newCamFocus;
	// 注視点（LookAt）をGameObjectの少し前に設定
	newCamFocus.x = (m_pHost->m_Position.x + m_fPan) + frontVec.x;
	newCamFocus.y = (m_pHost->m_Position.y + m_fTilt) + frontVec.y;
	newCamFocus.z = (m_pHost->m_Position.z) + frontVec.z;
	m_Camera->SetFocus(newCamFocus);

	m_Camera->UpdateCamera();
}
