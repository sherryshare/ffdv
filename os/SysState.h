//////////////////////////////////////////////////////////////////////////
/* 
AthrunArthur 2010-5-4,得到系统的负载及相关信息
*/
//////////////////////////////////////////////////////////////////////////
#ifndef _SYSSTATE_H
#define _SYSSTATE_H

#include "sys_detail/ISysInfo.h"

namespace os{
class SysState
{
private:
	SysState(void);
public:
	virtual ~SysState(void);
	
	//singleton
	static SysState *		getInstance();

	const char *			getError(int errorcode) const{return m_pSysInfo->getError(errorcode);};

	//得到逻辑CPU的个数
	int						getCPUNums()const;

	//得到CPU使用率， *100
	int						getCPUUsedRate()const;

	//得到过去1分钟内的平均负载，*100
	int						getLoadAvg1()const;

	//得到过去5分钟内的平均负载， *100
	int						getLoadAvg5()const;

	//得到过去15分钟内的平均负载，*100
	int						getLoadAvg15()const;
	
	//得到物理内存大小，单位为M
	int						getTotalMem()const;

	//得到内存使用率，*100
	int						getMemUsedRate()const;

	//得到磁盘大小，单位为G
	int						getTotalDisk() const;

	//得到磁盘剩余空间，单位为G
	int						getFreeDisk() const;

	//得到过去一段时间内的网络输入速率，单位为KBps
	int						getNetInSpeed() const;

	//得到过去一段时间内网络的输出速率，单位为KBps
	int						getNetOutSpeed() const;
protected:
	std::string				getOSVersion() const;
protected:
		
	ISysInfo *				m_pSysInfo;
};
}//end namespace os

#endif
