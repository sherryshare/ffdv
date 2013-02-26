#ifndef FFDV_OS_SYSSTATE_H
#define FFDV_OS_SYSSTATE_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <assert.h>
#include <network.h>
namespace os{
class SysState
{
private:
	SysState(void);
public:
	virtual ~SysState(void);
	
	//singleton
	static SysState *		getInstance();

	int32_t						getCPUNums()const;

	int32_t						getCPUUsedRate();

	int32_t						getLoadAvg1()const;

	int32_t						getLoadAvg5()const;

	int32_t						getLoadAvg15()const;
	
	int32_t						getPhysicalMem()const;

	int32_t						getMemUsedRate()const;
	
protected:
	std::string				getOSVersion() const;
protected:
	int32_t m_iCPUNums;	
	uint64_t m_ulCputime;
	uint64_t m_ulIdletime;
	uint32_t m_iPhysicalMem;
};
}//end namespace os

#endif
