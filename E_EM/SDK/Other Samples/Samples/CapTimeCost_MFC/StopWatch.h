// StopWatch.h
#pragma once

class CStopWatch
{
    // Construction
public:
	CStopWatch();
	CStopWatch(int averageCount);
    ~CStopWatch();

    // Implementation
public:
	void Start();
	int Stop();
	void Reset();
    double GetAverageTime();
    
    int GetAverageCount();
    void SetAverageCount(int averageCount);

protected:
    double          m_Average;
    int             m_AverageCount;
    int*            m_pAverageArray;
    long            m_AverageSum;
    int             m_AverageArrayIndex;
    bool            m_bIsReset;
    LARGE_INTEGER	m_liFreq;
    LARGE_INTEGER	m_liStart;
    LARGE_INTEGER	m_liStop;
};
