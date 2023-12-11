#include "BCamera.h"
#include "MPrimitives.h"

CAMERA CAMERA::g_Camera;

void CAMERA::InitCamera()
{
	// プロジェクションを作成
	m_fAspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
	CreateProjectionMatrix();

	// カメラビューを作成
	m_fZoom = -5.0f;
	m_fTilt = 0.0f;
	m_fPan = 0.0f;
	m_fAngle = 0.0f;

	SetCamPos(XMFLOAT3(0.0f, 3.0f, m_fZoom));
	SetFocus(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetUpVector(XMFLOAT3(0.0f, 1.0f, 0.0f));
	CreateViewMatrix();

	//Cube* host = new Cube();
	//host->Create(0.1f, 0.1f, 0.1f);
	//m_pHost = host;
}

void CAMERA::UpdateCamera()
{
	CreateViewMatrix();
}

void CAMERA::DrawCamera()
{
#ifdef _DEBUG
	//m_pHost->Draw(RasterType::WIREFRAME_CULL_BACK);
#endif // _DEBUG
}

void CAMERA::CreateProjectionMatrix()
{
	ALIGN16 XMMATRIX projection;

	// パースペクティブ投影を作成
	projection = XMMatrixPerspectiveFovLH(m_fFoV, m_fAspectRatio, m_fNearClip, m_fFarClip);
	XMStoreFloat4x4(&m_xmfPerspectiveProj, XMMatrixTranspose(projection));

	// 正投影を作成
	projection = XMMatrixOrthographicLH(RATIO_WIDTH, RATIO_HEIGHT, m_fNearClip, m_fFarClip);
	XMStoreFloat4x4(&m_xmfOrthographicProj, XMMatrixTranspose(projection));

}

void CAMERA::CreateViewMatrix()
{
	// カメラのデータをそれぞれ設定
	ALIGN16 XMVECTOR locPos = XMLoadFloat3(&m_xmfCameraPos);
	ALIGN16 XMVECTOR locFocus = XMLoadFloat3(&m_xmfLookAt);
	ALIGN16 XMVECTOR locUp = XMLoadFloat3(&m_xmfUpVector);

	ALIGN16 XMMATRIX camera;
	camera = XMMatrixLookAtLH(locPos, locFocus, locUp);					// ビュー行列作成
	XMStoreFloat4x4(&m_xmfViewMatrix, XMMatrixTranspose(camera));		// 変数に保存
}
