#ifndef MYUTILITIES_H
#define MYUTILITIES_H

#include <string>
#include <DirectXMath.h>

#include "defines.h"

inline std::string GetFileExtension(const char* filename)
{
	std::string extname;
	std::string fullpathstr(filename);
	size_t ext_i = fullpathstr.find_last_of(".");

	extname = fullpathstr.substr(ext_i + 1, fullpathstr.size() - ext_i);
	return extname;
}

//! \fn void DXVec3Normalize(DirectX::XMFLOAT3& xmfInVector, DirectX::XMFLOAT3& xmfOutVector)
/*! \brief custom utility to normalize a 3D vector
 *  \brief
 *  \param xmfOutVector : DirectX::XMFLOAT3
 *  \param xmfInVector : DirectX::XMFLOAT3
 */
inline void DXVec3Normalize(DirectX::XMFLOAT3& xmfOutVector, DirectX::XMFLOAT3& xmfInVector)
{
	ALIGN16 DirectX::XMVECTOR inv;
	ALIGN16 DirectX::XMVECTOR outv;

	inv = DirectX::XMLoadFloat3(&xmfInVector);
	outv = DirectX::XMVector3Normalize(inv);

	DirectX::XMStoreFloat3(&xmfOutVector, outv);
}

#endif // MYUTILITIES_H
