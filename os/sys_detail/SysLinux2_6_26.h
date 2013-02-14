//////////////////////////////////////////////////////////////////////////
/*
AthrunArthur 2010-5-4，本类针对linux内核2.6.26版本，对其他版本不保证兼容性，
本类使用了/proc文件系统中的文件，由于不同版本的内核对文件中字段的定义有一定的差别，
因此在其他版本的内核中使用时，需谨慎。
*/
//////////////////////////////////////////////////////////////////////////
#ifndef _SYS_LINUX_2_6_26_H
#define _SYS_LINUX_2_6_26_H

///#include "../../common/TMheader.h" //temporarily removed
#include "ISysInfo.h"

#include <sys/time.h>
#include <unistd.h>

namespace os{
class SysLinux2_6_26 : public ISysInfo
{
public:
	SysLinux2_6_26(void);
	virtual ~SysLinux2_6_26(void);

	//implementation
//For OS
	//得到操作系统的版本信息
	inline virtual std::string	getOSVersion();

//For CPU
	//得到系统CPU个数,成功时返回CPU个数，否则返回值小于0，返回值可以用getError（）得到错误原因
	inline virtual int			getCPUNums();

	//得到系统中总计的CPU使用率，如使用率为48%，则该值为48
	//成功时返回值大于0，否则返回值小于0
	virtual int			getCPUUsedRate();

	//得到系统核个数，成功时返回核的个数，否则返回值小于0，返回值可以用getError()得到错误原因
	//核个数=CPU数*每个CPU上的核数，例如，双路四核则共有八个核
	//成功时返回值大于0，否则返回值小于0
	inline virtual int			getCoreNums();

	//得到系统CPU的详细信息
	virtual std::vector<cpu_info_detail>	getCPUInfo();

//For memory
	//得到系统中物理内存大小，单位为M
	//成功时返回值大于0，否则返回值小于0
	inline virtual int			getPhysicalMem();

	//得到系统中已经使用的物理内存大小，单位为M
	//成功时返回值大于0，否则返回值小于0
	virtual int			getPhysicalUsedMem();

	//得到系统中块大小，单位为K
	//成功时返回值大于0，否则返回值小于0
	inline virtual int			getBlockSize();

	//得到系统中页面大小,单位为K
	//成功时返回值大于0，否则返回值小于0
	inline virtual int			getPageSize();

//For load average
	//得到系统中过去1分钟内的平均队列长度，结果为		实际长度*100
	//成功时返回值大于0，否则返回值小于0
	virtual int			getLoadAvg1();

	//得到系统中过去5分钟内的平均队列长度，结果为		实际长度*100
	//成功时返回值大于0，否则返回值小于0
	virtual int			getLoadAvg5();

	//得到系统中过去15分钟内的平均队列长度，结果为		实际长度*100
	//成功时返回值大于0，否则返回值小于0
	virtual int			getLoadAvg15();

//For processes
	//得到系统中进程数
	//成功时返回值大于0，否则返回值小于0
	virtual int			getProcessNums();

	//得到进程的详细信息
	//成功时返回值大于0，否则返回值小于0
	virtual std::vector<proc_info_detail>	getProcInfo();

//For disk
	//得到磁盘的相关信息
	virtual fsinfo		getFSInfo();

//For network
	//得到网络的入速率和出速率，单位为KBps
	virtual int			getInNetRate();
	virtual int			getOutNetRate();

protected:
	//读\proc\...file,返回整个文件的字符串。
	std::string			readProcFile(const char * filePath);

protected:
//For OS
	std::string			m_strOSVersion;	//操作系统版本信息，构造初始化
//For CPU
	int					m_iCPUNums;		//CPU数量，构造初始化
	int					m_iCoreNums;	//核的数量，构造初始化

	std::vector<cpu_info_detail>	m_oCPUInfo;	//CPU信息，调用时重新赋值。

	unsigned long		m_ulCputime;	//上次调用时的CPU时间
	unsigned long		m_ulIdletime;	//上次调用时的空闲CPU时间
//For Memory
	int					m_iPhysicalMem;	//物理内存大小，单位为M，构造初始化
	int					m_iBlockSize;	//系统中块大小，单位为K，构造初始化
	int					m_iPageSize;	//系统中页面大小，单位为K，构造初始化
//For process
	std::vector<proc_info_detail>	m_oProcInfo;	//进程信息，调用时赋值。

//For file system
	fsinfo				m_ofsinfo;

//For network
	struct timeval		m_oLastCalltime;	//上次获得网络情况的时间，构造初始化
	long				m_lLastInBytes;		//上次获得网络情况时对应的入字节数
	long				m_lLastOutBytes;	//上次获得网络情况时对应的出字节数
};
}//end namespace os

#endif

