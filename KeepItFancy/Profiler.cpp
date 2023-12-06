#include "Profiler.h"

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! TIME Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int64_t TIME::m_iStartTime;
double TIME::m_fFrameTime;
float TIME::m_fTicksPerMs;

void TIME::InitTimer()
{
	int64_t i64Frequency;

	QueryPerformanceFrequency((LARGE_INTEGER*)&i64Frequency);
	if (i64Frequency == 0)
	{
		i64Frequency = CLOCKS_PER_SEC;
	}

	// Find out how many times the frequency counter ticks every millisecond.
	m_fTicksPerMs = static_cast<float>(i64Frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_iStartTime);
}

void TIME::CalculateFrames()
{
	int64_t iEndTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&iEndTime);
	double dDeltaTime = static_cast<double>(iEndTime - m_iStartTime);
	m_fFrameTime = dDeltaTime / m_fTicksPerMs;
	m_iStartTime = iEndTime;

}



///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! FPS Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int64_t FPS::m_iFrameCount;
int64_t FPS::m_iStartTime;
int FPS::m_iFPS;

void FPS::InitFPS()
{
	m_iStartTime = timeGetTime();
}

void FPS::CountFrames()
{
	m_iFrameCount++;

	if (timeGetTime() >= (m_iStartTime + 1000))
	{
		m_iFPS = m_iFrameCount;
		m_iFrameCount = 0;

		m_iStartTime = timeGetTime();
	}
}



///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! CPU Class
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HQUERY CPU::m_queryHandle;
HCOUNTER CPU::m_counterHandle;
int64_t CPU::m_iLastSampleTime;
int64_t CPU::m_iCpuUsage;
bool CPU::m_canReadCpu;

void CPU::InitCPU()
{
	PDH_STATUS status;

	// Initialize the flag indicating whether this object can read the system cpu usage or not.
	m_canReadCpu = true;

	// Create a query object to poll cpu usage.
	status = PdhOpenQuery(NULL, 0, &m_queryHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	// Set query object to poll all cpus in the system.
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canReadCpu = false;
	}

	m_iLastSampleTime = GetTickCount64();

	m_iCpuUsage = 0;
}

void CPU::CountFrames()
{
	PDH_FMT_COUNTERVALUE value;

	if (m_canReadCpu)
	{
		if ((m_iLastSampleTime + 1000) < GetTickCount64())
		{
			m_iLastSampleTime = GetTickCount64();

			PdhCollectQueryData(m_queryHandle);

			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

			m_iCpuUsage = static_cast<int64_t>(value.longValue);
		}
	}
}

int CPU::GetCpuPercentage()
{
	int usage;

	if (m_canReadCpu)
	{
		usage = static_cast<int>(m_iCpuUsage);
	}
	else
	{
		usage = 0;
	}

	return usage;
}
