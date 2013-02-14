//////////////////////////////////////////////////////////////////////////
/*
AthrunArthur 2010-5-4，对不同系统的统一接口，得到系统中的相关信息
*/
//////////////////////////////////////////////////////////////////////////
#ifndef _ISYS_INFO_H
#define _ISYS_INFO_H
#include <string>
#include <vector>
///#include "../../common/TMheader.h" //temporarily removed
#include "../../common/ErrorCode.h"

namespace os{
	using namespace std;
	
//CPU详细信息
struct cpu_info_detail{
	int			m_iCoreNums;	//单个CPU上核的个数
	int			m_iMHZ;			//单个核上的主频，认为各个核的主频是一致的。
	std::vector<int>			m_iUsedRate;	//单个核的使用率百分比，如使用率为48%，则该值为48
	//more information can be added here
	//例如超线程，流水线深度，cache大小等
};

//进程详细信息
struct proc_info_detail{
	unsigned long		m_ulPID;	//进程标识
	unsigned long		m_ulParentPID;	//父进程标识
	int			m_iKUsedMem;			//进程占用的内存
	int			m_iUsedCPU;				//进程占用的CPU比例*100
	int			m_iThrdNums;			//进程中的线程数
	
	std::string	m_strProcName;		//进程名
	std::string	m_strProcInfo;		//进程描述
};

//文件系统信息
typedef enum fstype_{
	f_ntfs = 1,
	f_fat,
	f_fat32,
	f_fatex,
	f_ext,
	f_ext2,
	f_ext3,
	f_jsf,
	f_xfs,
	f_msdos,
	f_umsdos,
	f_iso9660,
	f_vfat,
	f_hpfs,
	f_smb,
	f_sysv,
	f_ncpfs,
	f_proc
}fstype;
typedef struct fsinfo_
{
	fstype		f_type;
	long		f_bsize;	//块大小，字节数
	long		f_blocks;	//文件系统数据块总数
	long		f_bfree;	//可用块数
	long		f_files;	//文件节点总数
	long		f_ffree;	//可用文件节点数
	long		f_namelen;	//文件名的最大长度
}fsinfo;

class ISysInfo{
public:
	const char *		getError(int errCode) const{return m_oErr.getError(errCode);};			//得到错误信息

//For OS
	//得到操作系统的版本信息
	virtual std::string	getOSVersion() =0;

//For CPU
	//得到系统CPU个数,成功时返回CPU个数，否则返回值小于0，返回值可以用getError（）得到错误原因
	virtual int			getCPUNums() =0;
	
	//得到系统中总计的CPU使用率，如使用率为48%，则该值为48
	//成功时返回值大于0，否则返回值小于0
	virtual int			getCPUUsedRate() = 0;

	//得到系统核个数，成功时返回核的个数，否则返回值小于0，返回值可以用getError()得到错误原因
	//核个数=CPU数*每个CPU上的核数，例如，双路四核则共有八个核
	//成功时返回值大于0，否则返回值小于0
	virtual int			getCoreNums() =0;

	//得到系统CPU的详细信息
	virtual std::vector<cpu_info_detail>	getCPUInfo() =0;

//For memory
	//得到系统中物理内存大小，单位为M
	//成功时返回值大于0，否则返回值小于0
	virtual int			getPhysicalMem() =0;

	//得到系统中已经使用的物理内存大小，单位为M
	//成功时返回值大于0，否则返回值小于0
	virtual int			getPhysicalUsedMem() = 0;

	//得到系统中块大小，单位为K
	//成功时返回值大于0，否则返回值小于0
	virtual int			getBlockSize() = 0;

	//得到系统中页面大小，单位为K
	//成功时返回值大于0，否则返回值小于0
	virtual int			getPageSize() = 0;

//For load average
	//得到系统中过去1分钟内的平均队列长度，结果为		实际长度*100
	//成功时返回值大于0，否则返回值小于0
	virtual int			getLoadAvg1() = 0;

	//得到系统中过去5分钟内的平均队列长度，结果为		实际长度*100
	//成功时返回值大于0，否则返回值小于0
	virtual int			getLoadAvg5() = 0;

	//得到系统中过去15分钟内的平均队列长度，结果为		实际长度*100
	//成功时返回值大于0，否则返回值小于0
	virtual int			getLoadAvg15() = 0;

//For processes
	//得到系统中进程数
	//成功时返回值大于0，否则返回值小于0
	virtual int			getProcessNums() = 0;

	//得到进程的详细信息
	//成功时返回值大于0，否则返回值小于0
	virtual std::vector<proc_info_detail>	getProcInfo() = 0;

//For disk
	//得到磁盘的相关信息
	virtual fsinfo		getFSInfo() = 0;

//For network	added by A.A 2010 5-27
	//得到网络的入速率和出速率，单位为KBps
	virtual int			getInNetRate() =0;
	virtual int			getOutNetRate() = 0;

protected:
	int					addError(const char *strErr){return m_oErr.addError(strErr);};
protected:
	ErrorCode			m_oErr;
};

}//end namespace os

#endif


