#ifndef MYUTILITIES_H
#define MYUTILITIES_H

#include <fstream>
#include <sstream>
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

inline std::string GetCurrentTimestamp()
{
	// Get the current time for timestamp
	std::time_t now = std::time(nullptr);
	struct tm timeinfo;
	localtime_s(&timeinfo, &now);

	char buffer[80];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

	return buffer;
}

inline void LogToFile(const char* errorMessage)
{
	// Create a filename with the timestamp
	std::string filename = "logs/Log_" + GetCurrentTimestamp().substr(0, 10) + ".txt";

	// Open the file in append mode
	std::ofstream logFile(filename, std::ios::app);

	if (logFile.is_open())
	{
		// Write the error message along with the timestamp to the file
		logFile << "[" << GetCurrentTimestamp() << "]" << std::endl;
		logFile << errorMessage << std::endl;

		// Close the file
		logFile.close();
	}
	else
	{
		// Output to console if unable to open the file
		std::cerr << "Error: Unable to open error log file." << std::endl;
	}
}

#endif // MYUTILITIES_H