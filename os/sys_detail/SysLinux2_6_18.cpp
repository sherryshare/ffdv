#include "SysLinux2_6_18.h"
#include <stdio.h>
#include <sys/vfs.h>
#include "../../utils/PUtils.h"

namespace os{
SysLinux2_6_18::SysLinux2_6_18( void )
: m_iBlockSize(-1)
, m_iCoreNums(-1)
, m_iCPUNums(-1)
, m_iPageSize(-1)
, m_iPhysicalMem(-1)
{
	//版本信息
	m_strOSVersion = readProcFile("/proc/version");
	std::string res;
	std::string temp;
	std::string::size_type pos;
	std::string::size_type pos2;
	char none_use[255];
	//CPU信息
	res = readProcFile("/proc/cpuinfo");
	if(!res.empty())
	{
		pos = res.rfind("processor");
		if(pos!=std::string::npos)
		{
			//2.6.18版本内核对多核的信息提示并不完整,只有逻辑CPU的数量
			pos2=res.find('\n',pos);
			temp = res.substr(pos,pos2-pos);

			sscanf(temp.c_str(),"%s %s %d\n",none_use,none_use,&m_iCPUNums);
			m_iCPUNums ++;
			m_iCoreNums = m_iCPUNums;
		}
	}

	res = readProcFile("/proc/stat");
	if(!res.empty())
	{
		//在内核2.6.18中，有8个值，其中第八个位无用；此处只使用前7个值。
		unsigned long user,nice,sys,idle,iowait,irq,softirq;	//按顺序出现的七个值
		unsigned long cputime, usedtime;
		std::string::size_type pos,pos2;
		std::string temp;
		pos = res.find_first_not_of("cpu ");
		pos2 = res.find('\n',pos);
		temp = res.substr(pos,pos2-pos);
		sscanf(temp.c_str(),"%lu %lu %lu %lu %lu %lu %lu ",&user,&nice,&sys,&idle,&iowait,&irq,&softirq);
		cputime = user+nice+sys+idle+iowait+irq+softirq;
		m_ulCputime = cputime;
		m_ulIdletime = idle;
	}


	//内存信息
	res = readProcFile("/proc/meminfo");
	if(!res.empty())
	{
		pos = res.find_first_not_of("MemTotal: ");
		pos2 = res.find('\n',pos);
		temp = res.substr(pos,pos2-pos);
		long int iTemp;
		sscanf(temp.c_str(),"%ld",&iTemp);
		m_iPhysicalMem = iTemp/1024;		//物理内存大小
		m_iBlockSize = -1;					//块大小未给出
/*
		pos = res.find("Hugepagesize:");
		pos = res.find_first_not_of("Hugepagesize: ",pos);
		pos2 = res.find('\n',pos);
		temp = res.substr(pos,pos2-pos);
		sscanf(temp.c_str(),"%d",&iTemp);*/
		m_iPageSize= -1;					//页面大小未给出
	}

	//文件系统信息，此处为磁盘大小
	struct statfs diskinfo;
	statfs("/",&diskinfo);
	switch(diskinfo.f_type)
	{
	case 0x4d44:
		m_ofsinfo.f_type = f_fat;
		break;
	case 0x5346544e:
	case 0x65735546:
		m_ofsinfo.f_type = f_ntfs;
		break;
	case 0xef53:
	case 0xef51:
		m_ofsinfo.f_type = f_ext2;
		break;

	default:
		assert(0);
		break;
	}
	m_ofsinfo.f_bfree = diskinfo.f_bfree;
	m_ofsinfo.f_blocks = diskinfo.f_blocks;
	m_ofsinfo.f_bsize = diskinfo.f_bsize;
	m_ofsinfo.f_ffree = diskinfo.f_ffree;
	m_ofsinfo.f_files = diskinfo.f_files;
	m_ofsinfo.f_namelen = diskinfo.f_namelen;

	//网络情况
	gettimeofday(&m_oLastCalltime,NULL);
	res = readProcFile("/proc/net/dev");
	if(!res.empty())
	{
		pos = res.find("eth");
		pos = res.find(":",pos);
		pos2 = res.find_first_not_of("1234567890 ",pos);
		pos = res.find_first_not_of(":",pos);
		//printf("%d %d %s", pos, pos2, res.c_str());
		temp = res.substr(pos,pos2-pos);	//得到网卡的流量情况
		int i;
		sscanf(temp.c_str(),"%ld",&m_lLastInBytes);
		for(int i=0; i<8;i++)
		{
			pos = temp.find_first_not_of(' ');
			pos2 = temp.find_first_not_of("1234567890", pos);
			temp = temp.substr(pos2,temp.size()-pos2);
		}
		sscanf(temp.c_str(),"%ld",&m_lLastOutBytes);

	}
	else
		assert(0);
}

SysLinux2_6_18::~SysLinux2_6_18( void )
{

}

std::string SysLinux2_6_18::getOSVersion()
{
	return m_strOSVersion;
}

int SysLinux2_6_18::getCPUNums()
{
	if(m_iCPUNums<=0)
		return addError("unsupported!");
	return m_iCPUNums;
}

int SysLinux2_6_18::getCPUUsedRate()
{
	std::string res = readProcFile("/proc/stat");
	if(res.empty())
		return addError("unsupported: cannot find file /proc/stat !");

	//在内核2.6.18中，有八个值，其中第八个位虚拟机时间，无用；此处只使用前7个值。
	unsigned long user,nice,sys,idle,iowait,irq,softirq;	//按顺序出现的七个值
	unsigned long cputime, usedtime;
	std::string::size_type pos,pos2;
	std::string temp;
	pos = res.find_first_not_of("cpu ");
	pos2 = res.find('\n',pos);
	temp = res.substr(pos,pos2-pos);
	sscanf(temp.c_str(),"%lu %lu %lu %lu %lu %lu %lu ", &user, &nice,&sys,&idle,&iowait,&irq,&softirq);
	cputime = user+nice+sys+idle+iowait+irq+softirq;
	usedtime = cputime-idle;

	//assert(cputime!=m_ulCputime);			//modified by A.A,
	int iUsedRate;
	if(cputime == m_ulCputime)
		iUsedRate = 0;
	else{
		iUsedRate = static_cast<int>(100-100.0*(idle-m_ulIdletime)/(cputime-m_ulCputime));
		m_ulCputime = cputime;
		m_ulIdletime = idle;
	}
	return iUsedRate;
}

int SysLinux2_6_18::getCoreNums()
{
	if(m_iCoreNums<=0)
		return addError("unsupported!");
	return m_iCoreNums;
}

std::vector<cpu_info_detail> SysLinux2_6_18::getCPUInfo()
{
	m_oCPUInfo.clear();
	return m_oCPUInfo;
}

int SysLinux2_6_18::getPhysicalMem()
{
	if(m_iPhysicalMem<=0)
		return addError("unsupported!");
	return m_iPhysicalMem;
}

int SysLinux2_6_18::getPhysicalUsedMem()
{
	std::string res = readProcFile("/proc/meminfo");
	if(res.empty())
		return addError("unsupported: cannot find file /proc/meminfo !");
	long int memtotal;
	long int memfree,buffers,cached;
	std::string::size_type pos,pos2;
	std::string temp;
	pos = res.find("MemTotal:");
	pos = res.find_first_not_of("MemTotal: ",pos);
	pos2 = res.find('\n',pos);
	temp = res.substr(pos,pos2-pos);
	sscanf(temp.c_str(),"%ld",&memtotal);		//总的内存大小

	pos = res.find("MemFree:");
	pos = res.find_first_not_of("MemFree: ",pos);
	pos2 = res.find('\n',pos);
	temp = res.substr(pos,pos2-pos);
	sscanf(temp.c_str(),"%ld",&memfree);			//闲置的内存大小

	pos = res.find("Buffers:");
	pos = res.find_first_not_of("Buffers: ",pos);
	pos2 = res.find('\n',pos);
	temp = res.substr(pos,pos2-pos);
	sscanf(temp.c_str(),"%ld",&buffers);			//缓冲大小

	pos = res.find("Cached:");
	pos = res.find_first_not_of("Cached: ",pos);
	pos2 = res.find('\n',pos);
	temp = res.substr(pos,pos2-pos);
	sscanf(temp.c_str(),"%ld",&cached);			//cache大小

	long int iTemp = memtotal-memfree-buffers-cached;
	int iUsedMem = iTemp/1024;					//换算为以M为单位。
	return iUsedMem;
}

int SysLinux2_6_18::getBlockSize()
{
	if(m_iBlockSize<=0)
		return addError("unsupported!");
	return m_iBlockSize;
}

int SysLinux2_6_18::getPageSize()
{
	if(m_iPageSize<=0)
		return addError("unsupported!");
	return m_iPageSize;
}

int SysLinux2_6_18::getLoadAvg1()
{
	std::string res = readProcFile("/proc/loadavg");
	float la1,la5,la15;
	sscanf(res.c_str(),"%f %f %f ",&la1,&la5,&la15);
	int iRes = static_cast<int>(la1*100);
	return iRes;
}

int SysLinux2_6_18::getLoadAvg5()
{
	std::string res = readProcFile("/proc/loadavg");
	float la1,la5,la15;
	sscanf(res.c_str(),"%f %f %f ",&la1,&la5,&la15);
	int iRes = static_cast<int>(la5*100);
	return iRes;
}

int SysLinux2_6_18::getLoadAvg15()
{
	std::string res = readProcFile("/proc/loadavg");
	float la1,la5,la15;
	sscanf(res.c_str(),"%f %f %f ",&la1,&la5,&la15);
	int iRes = static_cast<int>(la15*100);
	return iRes;
}

int SysLinux2_6_18::getProcessNums()
{
	return addError("unsupported!");
}

std::vector<proc_info_detail> SysLinux2_6_18::getProcInfo()
{
	m_oProcInfo.clear();
	return m_oProcInfo;
}

fsinfo SysLinux2_6_18::getFSInfo()
{
	return m_ofsinfo;
}

int SysLinux2_6_18::getInNetRate()
{
	struct timeval curtime;
	gettimeofday(&curtime,NULL);
	if(curtime.tv_sec == m_oLastCalltime.tv_sec)
		return 0;
	std::string res, temp;
	std::string::size_type pos, pos2;
	long inBytes = m_lLastInBytes;
	long outBytes = m_lLastOutBytes;

	res = readProcFile("/proc/net/dev");
	if(!res.empty())
	{
		pos = res.find("eth");
		pos = res.find(":",pos);
		pos2 = res.find_first_not_of("1234567890 ",pos);
		pos = res.find_first_not_of(":",pos);

		temp = res.substr(pos,pos2-pos);	//得到网卡的流量情况
		int i;
		sscanf(temp.c_str(),"%ld",&m_lLastInBytes);
		for(int i=0; i<8;i++)
		{
			pos = temp.find_first_not_of(' ');
			pos2 = temp.find_first_not_of("1234567890", pos);
			temp = temp.substr(pos2,temp.size()-pos2);
		}
		sscanf(temp.c_str(),"%ld",&m_lLastOutBytes);
	}

	int inRate = static_cast<int>(1.0*(m_lLastInBytes - inBytes)/(curtime.tv_sec - m_oLastCalltime.tv_sec));
	m_oLastCalltime.tv_sec = curtime.tv_sec;
	m_oLastCalltime.tv_usec = curtime.tv_usec;
	return inRate;
}

int SysLinux2_6_18::getOutNetRate()
{
	struct timeval curtime;
	gettimeofday(&curtime,NULL);
	if(curtime.tv_sec == m_oLastCalltime.tv_sec)
		return 0;
	std::string res, temp;
	std::string::size_type pos, pos2;
	long inBytes = m_lLastInBytes;
	long outBytes = m_lLastOutBytes;

	res = readProcFile("/proc/net/dev");
	if(!res.empty())
	{
		pos = res.find("eth");
		pos = res.find(":",pos);
		pos2 = res.find_first_not_of("1234567890 ",pos);
		pos = res.find_first_not_of(":",pos);
		temp = res.substr(pos,pos2-pos);	//得到网卡的流量情况
		int i;
		sscanf(temp.c_str(),"%ld",&m_lLastInBytes);
		for(int i=0; i<8;i++)
		{
			pos = temp.find_first_not_of(' ');
			pos2 = temp.find_first_not_of("1234567890", pos);
			temp = temp.substr(pos2,temp.size()-pos2);
		}
		sscanf(temp.c_str(),"%ld",&m_lLastOutBytes);
	}

	int outRate = static_cast<int>(1.0*(m_lLastOutBytes - outBytes)/(curtime.tv_sec - m_oLastCalltime.tv_sec));
	m_oLastCalltime.tv_sec = curtime.tv_sec;
	m_oLastCalltime.tv_usec = curtime.tv_usec;
	return outRate;
}

std::string SysLinux2_6_18::readProcFile( const char * filePath )
{
	FILE *fp = fopen(filePath,"r");
	std::string res;
	if(fp == NULL)
		return res;
	char tmp;
	while((tmp=fgetc(fp))!=EOF)
	{
		if(tmp=='\0')
			res.push_back(' ');
		else if(tmp == '\t')
			res.push_back(' ');
		else
			res.push_back(tmp);
	}
	fclose(fp);
	return res;
}
}//end namespace os