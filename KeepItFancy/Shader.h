#ifndef SHADER_H
#define SHADER_H

#include "Texture.h"

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! SHADER
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class SHADER Shader.h "Shader.h"
/*! \brief Base Class for Shaders.
 *  \brief シェーダー親クラス
 */
class SHADER : public COMPONENT
{
protected:
	//! \enum Shader Type
	/*! \brief Shader Type to be classified on create. */
	enum ShaderType
	{
		VertexS,
		HullS,
		DomainS,
		GeometryS,
		PixelS,
		ComputeS
	};
	ShaderType m_eShaderType;

	virtual HRESULT CreateShader(void*, UINT) = 0;
	virtual void BindShader() = 0;

public:
	SHADER(ShaderType eShaderType) :
		m_eShaderType(eShaderType)
	{}
	virtual ~SHADER() {}

	std::vector<ID3D11Buffer*> m_pBuffers;

	HRESULT LoadShader(const char* fileName);
	HRESULT CompileShader(const char* fileName);

	void SendToBuffer(UINT bufferSlot, void* pData);
	void MapToBuffer(UINT bufferSlot, void* pData);
	void SetSRV(UINT bufferSlot, ID3D11ShaderResourceView* pSRV);

private:
	HRESULT CreateBuffer(void* pData, UINT fileSize, unsigned int stride = 0);
};


///--------------------------------------------------
//! Vertex Shader
///--------------------------------------------------
//! \class Vertex Shader Shader.h "Shader.h"
/*! \brief The Vertex Shader Class
 *  \brief 頂点シェーダークラス
 */
class VertexShader : public SHADER
{
private:
	ComPtr<ID3D11InputLayout>	m_d11InputLayout;
	ComPtr<ID3D11VertexShader>	m_d11VertexShader;

public:
	VertexShader() :
		SHADER(ShaderType::VertexS),
		m_d11InputLayout(nullptr),
		m_d11VertexShader(nullptr)
	{}
	void BindShader();

protected:
	HRESULT CreateShader(void* pData, UINT fileSize);
};

///--------------------------------------------------
//! Hull Shader
///--------------------------------------------------
//! \class Hull Shader Shader.h "Shader.h"
/*! \brief The Hull Shader Class | pre-Tessellation stage
 *  \brief ハルシェーダークラス ｜ テセレーション前
 */
class HullShader : public SHADER
{
private:
	ComPtr<ID3D11HullShader> m_d11HullShader;
public:
	HullShader() :
		SHADER(ShaderType::HullS),
		m_d11HullShader(nullptr)
	{}
	void BindShader();

protected:
	HRESULT CreateShader(void* pData, UINT fileSize);
};

///--------------------------------------------------
//! Domain Shader
///--------------------------------------------------
//! \class Domain Shader Shader.h "Shader.h"
/*! \brief The Domain Shader Class | post-Tessellation stage
 *  \brief ドメインシェーダークラス ｜ テセレーション後
 */
class DomainShader : public SHADER
{
private:
	ComPtr<ID3D11DomainShader> m_d11DomainShader;

public:
	DomainShader() :
		SHADER(ShaderType::DomainS),
		m_d11DomainShader(nullptr)
	{}
	void BindShader();

protected:
	HRESULT CreateShader(void* pData, UINT fileSize);
};

///--------------------------------------------------
//! Pixel Shader
///--------------------------------------------------
//! \class Pixel Shader Shader.h "Shader.h"
/*! \brief The Pixel Shader Class
 *  \brief ピクセルシェーダークラス
 */
class PixelShader : public SHADER
{
private:
	ComPtr<ID3D11PixelShader>	m_d11PixelShader;

public:
	PixelShader() :
		SHADER(ShaderType::PixelS),
		m_d11PixelShader(nullptr)
	{}
	void BindShader();

protected:
	HRESULT CreateShader(void* pData, UINT fileSize);
};

///--------------------------------------------------
//! Compute Shader
///--------------------------------------------------
//! \class Compute Shader
/*! \brief The Compute Shader Class
 *  \brief コンピュートシェーダークラス
 */
class ComputeShader : public SHADER
{
private:
	ComPtr<ID3D11ComputeShader>			m_d11ComputeShader;

public:
	ComputeShader() :
		SHADER(ShaderType::ComputeS),
		m_d11ComputeShader(nullptr)
	{}
	void BindUAV(UINT bufferSlot, ID3D11UnorderedAccessView** pUAV);
	void BindShader();

protected:
	HRESULT CreateShader(void* pData, UINT fileSize);
};

#endif // !SHADER_H
