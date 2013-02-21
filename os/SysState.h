#ifndef FFDV_OS_SYSSTATE_H
#define FFDV_OS_SYSSTATE_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <assert.h>

namespace os{
class SysState
{
private:
	SysState(void);
public:
	virtual ~SysState(void);
	
	//singleton
	static SysState *		getInstance();

	int						getCPUNums()const;

	int						getCPUUsedRate();

	int						getLoadAvg1()const;

	int						getLoadAvg5()const;

	int						getLoadAvg15()const;
	
	int						getPhysicalMem()const;

	int						getMemUsedRate()const;
	
protected:
	std::string				getOSVersion() const;
protected:
	int m_iCPUNums;	
	unsigned long m_ulCputime;
	unsigned long m_ulIdletime;
	unsigned int m_iPhysicalMem;
};
}//end namespace os

#endif
