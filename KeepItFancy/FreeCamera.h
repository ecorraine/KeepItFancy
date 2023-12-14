#ifndef FREECAMERA_H
#define FREECAMERA_H

#include "GameObject.h"
#include "BCamera.h"
#include "KeyInput.h"

class FreeCamera
{
private:
	GameObject*			m_pHost;
	static CAMERA*		m_Camera;
	float				m_fTilt = 0.0f;
	float				m_fPan = 0.0f;
	float				m_fAngle = 0.0f;					//!< カメラの角度

public:
	FreeCamera();
	~FreeCamera() {}

	static CAMERA* GetCamera()
	{
		return m_Camera;
	}

	void Draw();
	void Update(float tick);
};

#endif // !FREECAMERA_H
