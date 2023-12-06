#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>
#include <thread>
#include <ctime>

#include <pdh.h>
#pragma comment(lib, "pdh.lib")

#include "defines.h"

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! TIME Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class TIME Profiler.h "Profiler.h"
/*! \brief Class to calculate high precision time using QueryPerformance
 *  \brief 
 */
class TIME
{
public:
	TIME() = delete;
	~TIME() {}

	static void InitTimer();

	//! \fn void CalculateFrames()
	/*! \brief calculate frames for high precision time
	 *  \brief
	 */
	static void CalculateFrames();

	//! \fn double GetFrameTime()
	/*! \brief returns the Delta Time as float
	 *  \brief
	 *  \return m_dDeltaTime
	 */
	static double GetFrameTime()
	{
		return m_fFrameTime;
	}

private:
	static int64_t m_iStartTime;
	static double m_fFrameTime;
	static float m_fTicksPerMs;
};



///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! FPS Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class FPS Profiler.h "Profiler.h"
/*! \brief Class to calculate Frame Rate
 *  \brief
 */
class FPS
{
public:
	FPS() = delete;
	~FPS() {}

	static void InitFPS();

	//! \fn void CountFrames()
	/*! \brief calculate frames for FPS
	 *  \brief
	 */
	static void CountFrames();

	//! \fn double GetFrameTime()
	/*! \brief returns the Delta Time as float
	 *  \brief
	 *  \return m_dDeltaTime
	 */
	static double FPSCap(double fpsCap = g_FrameRateCap)
	{
		return (1000.0 / fpsCap);
	}

	static int GetFPSCount()
	{
		return m_iFPS;
	}

private:
	static int64_t m_iFrameCount;
	static int64_t m_iStartTime;
	static int m_iFPS;
};



///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! CPU Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! \class CPU Profiler.h "Profiler.h"
/*! \brief Class to calculate CPU usage
 *  \brief
 */
class CPU
{
public:
	CPU() = delete;
	~CPU() {}

	static void EndQuery()
	{
		if (m_canReadCpu)
		{
			PdhCloseQuery(m_queryHandle);
		}
	}
	
	static void InitCPU();
	static void CountFrames();
	static int GetCpuPercentage();

private:
	static HQUERY m_queryHandle;
	static HCOUNTER m_counterHandle;
	static int64_t m_iLastSampleTime;
	static int64_t m_iCpuUsage;
	static bool m_canReadCpu;
};

#endif // !PROFILER_H
