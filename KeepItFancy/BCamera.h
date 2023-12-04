#ifndef BCAMERA_H
#define BCAMERA_H

#include <DirectXMath.h>
using namespace DirectX;

#include "defines.h"

class Cube;

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! CAMERA Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class CAMERA BCamera.h "BCamera.h"
/*! \brief The Base Camera Class
 *  \brief カメラの親クラス
 */
class CAMERA
{
public:
	//! \fn static CAMERA* GetCamera()
	/*! \brief Camera Instance
	 *  \brief カメラインスタンス
	 *  \return static CAMERA
	 */
	static CAMERA* GetCamera()
	{
		static CAMERA instance;
		instance.InitCamera();

		return &instance;
	}

	//! \fn XMFLOAT4X4& GetCameraViewMatrix()
	/*! \brief returns the Camera View Matrix
	 *  \brief カメラビュー行列を返す
	 *  \return XMFLOAT4X4
	 */
	XMFLOAT4X4 GetCameraViewMatrix() { return m_xmfViewMatrix; }

	//! \fn XMFLOAT4X4& GetProjectioneMatrix()
	/*! \brief returns the Projection Matrix
	 *  \brief 投影行列を返す
	 *  \return XMFLOAT4X4
	 */
	XMFLOAT4X4 GetProjectioneMatrix()
	{
#ifdef PERSPECTIVE
		m_xmfProjectionMtx = m_xmfPerspectiveProj;
#endif // PERSPECTIVE
#ifdef ORTHOGRAPHIC
		m_xmfProjectionMtx = m_xmfOrthographicProj;
#endif // ORTHOGRAPHIC

		return m_xmfProjectionMtx;
	}

	const DirectX::XMFLOAT3 GetPosition() const { return m_xmfCameraPos; }

private:
	DirectX::XMFLOAT4X4		m_xmfViewMatrix;		//!< カメラビュー行列
	DirectX::XMFLOAT4X4		m_xmfPerspectiveProj;
	DirectX::XMFLOAT4X4		m_xmfOrthographicProj;
	DirectX::XMFLOAT4X4		m_xmfProjectionMtx;		//!< 投影行列

protected:
	DirectX::XMFLOAT3		m_xmfCameraPos;
	DirectX::XMFLOAT3		m_xmfLookAt;
	DirectX::XMFLOAT3		m_xmfUpVector;

	Cube* m_pHost;
	void InitCamera();

	float					m_fZoom = 0.0f;						//!< カメラの拡大縮小
	float					m_fTilt = 0.0f;						//!< カメラのティルト
	float					m_fPan = 0.0f;						//!< カメラのパンニング 
	float					m_fAngle = 0.0f;					//!< カメラの角度

private:
	float					m_fFoV = DirectX::XM_PIDIV4;		//!< 視野角
	float					m_fAspectRatio = 0.0f;
	float					m_fNearClip = 0.1f;					//!< ニアクリップ
	float					m_fFarClip = 1000.0f;				//!< ファークリップ

public:
	//! \fn void SetCamPos(const XMFLOAT3& xmfCameraPosition)
	/*! \brief
	 *  \brief カメラの位置を設置 ｜ LookAtと同じ座標はダメ
	 *  \param xmfCameraPosition : DirectX::XMFLOAT3
	 */
	void SetCamPos(const XMFLOAT3& xmfCameraPosition) { m_xmfCameraPos = xmfCameraPosition; }

	//! \fn void SetFocus(const XMFLOAT3& xmfLookAt)
	/*! \brief
	 *  \brief カメラの注視点を設置 ｜ CameraPosと同じ座標はダメ
	 *  \param xmfLookAt : DirectX::XMFLOAT3
	 */
	void SetFocus(const XMFLOAT3& xmfLookAt) { m_xmfLookAt = xmfLookAt; }

	//! \fn void SetUpVector(const XMFLOAT3& xmfUpVector)
	/*! \brief
	 *  \brief カメラの上方向を設置
	 *  \param xmfUpVector : 固定値 DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
	 */
	void SetUpVector(const XMFLOAT3& xmfUpVector) { m_xmfUpVector = xmfUpVector; }

	virtual void UpdateCamera();
	virtual void DrawCamera();

protected:
	//! \fn void CreateProjectionMatrix()
	/*! \brief set up the Projection Matrix
	 *  \brief 投影行列を作成
	 */
	void CreateProjectionMatrix();

	//! \fn CreateViewMatrix()
	/*! \brief set up the Camera Matrix
	 *  \brief カメラビュー行列を作成
	 */
	void CreateViewMatrix();

};

#endif // !BCAMERA_H
