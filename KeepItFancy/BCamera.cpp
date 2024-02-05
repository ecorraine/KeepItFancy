#include "BCamera.h"

CAMERA CAMERA::g_Camera;

void CAMERA::InitCamera()
{
	// プロジェクションを作成
	m_fAspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
	CreateProjectionMatrix();

	// カメラビューを作成
	m_fZoom = 3.0f;
	SetCamPos(XMFLOAT3(0.0f, 0.0f, -m_fZoom));
	SetFocus(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetUpVector(XMFLOAT3(0.0f, 1.0f, 0.0f));
	CreateViewMatrix();
}

XMFLOAT4X4 CAMERA::GetProjectioneMatrix()
{
#ifdef PERSPECTIVE
	m_ProjectionMtx = m_PerspectiveProj;
#endif // PERSPECTIVE
#ifdef ORTHOGRAPHIC
	m_ProjectionMtx = m_OrthographicProj;
#endif // ORTHOGRAPHIC

	return m_ProjectionMtx;
}

void CAMERA::UpdateCamera()
{
	CreateViewMatrix();
}

void CAMERA::CreateProjectionMatrix()
{
	ALIGN16 XMMATRIX projection;

	// パースペクティブ投影を作成
	projection = XMMatrixPerspectiveFovLH(m_fFoV, m_fAspectRatio, m_fNearClip, m_fFarClip);
	XMStoreFloat4x4(&m_PerspectiveProj, XMMatrixTranspose(projection));

	// 正投影を作成
	projection = XMMatrixOrthographicLH(RATIO_WIDTH, RATIO_HEIGHT, m_fNearClip, m_fFarClip);
	XMStoreFloat4x4(&m_OrthographicProj, XMMatrixTranspose(projection));

}

void CAMERA::CreateViewMatrix()
{
	// カメラのデータをそれぞれ設定
	ALIGN16 XMVECTOR locPos = XMLoadFloat3(&m_CameraPos);
	ALIGN16 XMVECTOR locFocus = XMLoadFloat3(&m_LookAt);
	ALIGN16 XMVECTOR locUp = XMLoadFloat3(&m_UpVector);

	ALIGN16 XMMATRIX camera;
	camera = XMMatrixLookAtLH(locPos, locFocus, locUp);			// ビュー行列作成
	XMStoreFloat4x4(&m_ViewMtx, XMMatrixTranspose(camera));		// 変数に保存
}
