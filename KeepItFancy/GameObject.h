#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Shader.h"
#include "BCamera.h"
#include "BLight.h"

#include <DirectXMath.h>
#include <DirectX_Files/DirectXTK/Inc/SimpleMath.h>
using namespace DirectX;

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "DirectX_Files/DirectXTK/Lib/x64/Debug/DirectXTK.lib")
#else
#pragma comment(lib, "DirectX_Files/DirectXTK/Lib/x64/Release/DirectXTK.lib")
#endif
#endif


///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! GameObject
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class GameObject GameObject.h "GameObject.h"
/*! \brief GameObject Class
 *  \brief ゲーム内で使うオブジェクトのクラス
 */
class GameObject
{
protected:
	std::list<COMPONENT*>	m_Component;
	std::list<GameObject*>	m_ChildGameObject;

public:
	GameObject() {}
	virtual ~GameObject() {}

	DirectX::XMFLOAT3		m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3		m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3		m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	///--------------------------------------------------
	template <class T> T* AddComponent()
	{
		T* ptr = new T();
		m_Component.push_back(ptr);
		return ptr;
	}

	///--------------------------------------------------
	template <class T> T* GetComponent()
	{
		for (COMPONENT* component : m_Component)
		{
			if (typeid(*component) == typeid(T))
			{
				return (T*)component;
			}
		}
		return nullptr;
	}

	///--------------------------------------------------
	template <class T> T* AddChild()
	{
		T* child = new T();
		m_ChildGameObject.push_back(child);

		return child;
	}
	///--------------------------------------------------

	//! \fn XMMATRIX GetRotation()
	/*! \brief returns the Rotation Matrix
	 *  \brief
	 *  \return XMMATRIX
	 */
	DirectX::XMMATRIX GetRotationMatrix()
	{
		ALIGN16 XMMATRIX locRot{};
		if (useCanvas)
		{
		}
		else
		{
			ALIGN16 XMMATRIX locRotX = XMMatrixRotationX(XMConvertToRadians(m_Rotation.x));
			ALIGN16 XMMATRIX locRotY = XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
			ALIGN16 XMMATRIX locRotZ = XMMatrixRotationZ(XMConvertToRadians(m_Rotation.z));

			locRot = (locRotZ * locRotX * locRotY);
		}
		return locRot;
	}

	//! \fn XMMATRIX GetWorldMatrix()
	/*! \brief returns the World Matrix
	 *  \brief
	 *  \return XMMATRIX
	 */
	DirectX::XMMATRIX GetWorldMatrix()
	{
		ALIGN16 XMMATRIX locTrans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
		ALIGN16 XMMATRIX locScale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

		return (locScale * GetRotationMatrix() * locTrans);
	}

	//! \fn Vector3 GetForwardVector()
	/*! \brief returns the Forward Vector of the GameObject
	 *  \brief 前方向ベクトル取得
	 *  \return Vector3
	 */
	DirectX::SimpleMath::Vector3 GetForwardVector()
	{
		float radianY = DirectX::XMConvertToRadians(m_Rotation.y);

		SimpleMath::Vector3 mForwardVector;
		mForwardVector.x = sinf(radianY);
		mForwardVector.y = 0.0f;
		mForwardVector.z = cosf(radianY);

		return mForwardVector;
	}

	//! \fn Vector3 GetForward()
	/*! \brief returns the Forward Vector of the GameObject
	 *  \brief 前方向ベクトル取得
	 *  \return Vector3
	 */
	DirectX::SimpleMath::Vector3 GetForward()
	{
		SimpleMath::Matrix rot;
		rot = SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);

		SimpleMath::Vector3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	const DirectX::XMFLOAT3 GetPosition() const { return m_Position; }

	//! \fn void SetWVPMatrix(SHADER* shader)
	/*! \brief bind World Matrix to Vertex Shader
	 *  \brief
	 */
	void SetWVPMatrix(SHADER* shader)
	{
		XMFLOAT4X4 wvp[3];
		XMStoreFloat4x4(&wvp[0], XMMatrixTranspose(GetWorldMatrix()));

		if (useCanvas)
		{
			// projection = view
			XMStoreFloat4x4(&wvp[1], XMMatrixIdentity());
			wvp[2] = wvp[1];
		}
		else
		{
			wvp[1] = CAMERA::GetCamera()->GetCameraViewMatrix();
			wvp[2] = CAMERA::GetCamera()->GetProjectioneMatrix();
		}

		shader->SendToBuffer(0, wvp);
	}

	void SetLight(SHADER* shader)
	{
		if (useLight)
		{
			XMFLOAT4 light[] = {
				{ CAMERA::GetCamera()->GetPosition().x, CAMERA::GetCamera()->GetPosition().y, CAMERA::GetCamera()->GetPosition().z, 0.0f },
				LIGHT::GetLight()->GetLightDirection(),
				LIGHT::GetLight()->GetLightDiffuse(),
				LIGHT::GetLight()->GetLightAmbient()
			};
			shader->SendToBuffer(0, light);
		}
	}

	virtual void Draw() {}
	virtual void Update(float tick)
	{
	}

	bool useCanvas = false;
	bool useLight = true;
	bool useTesselation = false;
};

#endif // !COMPONENT_H
