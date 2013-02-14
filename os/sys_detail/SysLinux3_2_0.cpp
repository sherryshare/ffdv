/*
 * =====================================================================================
 *
 *       Filename:  SysLinux3_2_0.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010年07月11日 20时05分59秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include "SysLinux3_2_0.h"
#include "../../utils/PUtils.h"
#include <stdio.h>
#include <sys/vfs.h>

namespace os{
SysLinux3_2_0::SysLinux3_2_0()
	: m_iBlockSize(-1),
	m_iCoreNums(-1),
	m_iCPUNums(-1),
	m_iPageSize(-1),
	m_iPhysicalMem(-1)
{
	m_strOSVersion = readProcFile("/proc/version");
	std::string res;
	std::string temp;
	std::string::size_type pos;
	std::string::size_type pos2;
	char none_use[255];
	res = readProcFile("/proc/cpuinfo");
	std::cout << "Start New!\n";//added
	if(!res.empty())
	{
		pos = res.rfind("processor");
		if(pos != std::string::npos)
		{
			pos2 = res.find("\n", pos);
			temp = res.substr(pos, pos2-pos);
			sscanf(temp.c_str(), "%s %s %d\n", none_use, none_use, &m_iCPUNums);
			m_iCPUNums++;
			m_iCoreNums = m_iCPUNums;
		}
	}

	res = readProcFile("/proc/stat");
	if(!res.empty())
	{
		unsigned long user, nice, sys, idle, iowait, irq, softirq;
		unsigned long cputime, usedtime;
		std::string::size_type pos, pos2;
		std::string temp;
		pos = res.find_first_not_of("cpu ");
		pos2 = res.find("\n", pos);
		temp = res.substr(pos, pos2-pos);
		sscanf(temp.c_str(), "%lu %lu %lu %lu %lu %lu %lu ", &user, &nice, &sys, &idle, &iowait, &irq, &softirq);//changed
		cputime = user + nice + sys + idle + iowait + irq + softirq;
		m_ulCputime = cputime;
		m_ulIdletime = idle;
	}

	res = readProcFile("/proc/meminfo");
	if(!res.empty())
	{
		pos = res.find_first_not_of("MemTotal: ");
		pos2 = res.find("\n", pos);
		temp = res.substr(pos, pos2-pos);
		long int iTemp;
		sscanf(temp.c_str(), "%ld", &iTemp);//%d changed to %ld
		m_iPhysicalMem = iTemp / 1024;
		m_iBlockSize = -1;	

		pos = res.find("Hugepagesize: ");
		pos = res.find_first_not_of("Hugepagesize: ", pos);
		pos2 = res.find("\n", pos);
		temp = res.substr(pos, pos2-pos);
		sscanf(temp.c_str(), "%ld", &iTemp);//%d changed to %ld
		m_iPageSize = iTemp;
		//printf("Mem=%d Size=%d\n",m_iPhysicalMem,m_iPageSize);//added
	}

	struct statfs diskinfo;
	statfs("/", &diskinfo);
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
	//printf("type=%d f_bfree=%d f_blocks=%d f_bsize=%d f_ffree=%d f_files=%d f_namelen=%d\n",m_ofsinfo.f_type,m_ofsinfo.f_bfree,m_ofsinfo.f_blocks,m_ofsinfo.f_bsize,m_ofsinfo.f_ffree,m_ofsinfo.f_files,m_ofsinfo.f_namelen);//added
	
	gettimeofday(&m_oLastCalltime, NULL);
	res = readProcFile("/proc/net/dev");
	if(!res.empty())
	{
		pos = res.find("eth");
		pos = res.find(":", pos);
		pos2 = res.find_first_not_of("1234567890 ", pos);
		pos = res.find_first_not_of(":", pos);
		temp = res.substr(pos, pos2-pos);
		int i;
		sscanf(temp.c_str(), "%ld", &m_lLastInBytes);//%d changed to %ld
		for(int i = 0; i < 8; i++)
		{
			pos = temp.find_first_not_of(' ');
			pos2 = temp.find_first_not_of("1234567890",pos);
			temp = temp.substr(pos2, temp.size()-pos2);
		}
		sscanf(temp.c_str(), "%ld", &m_lLastOutBytes);//%d changed to %ld
		//printf("m_lLastInBytes=%d m_lLastOutBytes=%d\n",m_lLastInBytes,m_lLastOutBytes);//added
	}
	else
		assert(0);
}

SysLinux3_2_0::~SysLinux3_2_0()
{
}

std::string SysLinux3_2_0::getOSVersion()
{
	return m_strOSVersion;
}

int SysLinux3_2_0::getCPUNums()
{
	if(m_iCPUNums < 0)
		return addError("unsupported!");
	return m_iCPUNums;
}

int SysLinux3_2_0::getCPUUsedRate()
{
	std::string res = readProcFile("/proc/stat");
	if(res.empty())
		return addError("unsupported: cannot find file /proc/stat !");
	unsigned long user, nice, sys, idle, iowait, irq, softirq;
	unsigned long cputime, usedtime;
	std::string::size_type pos, pos2;
	std::string temp;
	pos = res.find_first_not_of("cpu ");
	pos2 = res.find('\n', pos);
	temp = res.substr(pos, pos2-pos);
	sscanf(temp.c_str(), "%lu %lu %lu %lu %lu %lu %lu ", &user, &nice, &sys, &idle, &iowait, &irq, &softirq);//%d changed to %lu
	cputime = user + nice + sys + idle + iowait + irq + softirq;
	usedtime = cputime - idle;

	int iUsedRate;
	if(cputime == m_ulCputime)
		iUsedRate = 0;
	else
	{
		//Altered, the difference value must be changed to integer for the division
		iUsedRate = static_cast<int>(100-100.0*(idle-m_ulIdletime)/(cputime-m_ulCputime));
		m_ulCputime = cputime;
		m_ulIdletime = idle;
	}
	return iUsedRate;
}

int SysLinux3_2_0::getCoreNums()
{
	if(m_iCoreNums <= 0)
		return addError("unsupported!");
	return m_iCoreNums;
}

std::vector<cpu_info_detail> SysLinux3_2_0::getCPUInfo()
{
	m_oCPUInfo.clear();
	return m_oCPUInfo;
}

int SysLinux3_2_0::getPhysicalMem()
{
	if(m_iPhysicalMem <= 0)
		return addError("unsupported!");
	return m_iPhysicalMem;
}

int SysLinux3_2_0::getPhysicalUsedMem()
{
	std::string res = readProcFile("/proc/meminfo");
	if(res.empty())
		return addError("unsupported: cannot find file /proc/meminfo !");
	long int memtotal;
	long int memfree, buffers, cached;
	std::string::size_type pos, pos2;
	std::string temp;
	pos = res.find("MemTotal:");
	pos = res.find_first_not_of("MemTotal: ", pos);
	pos2 = res.find('\n', pos);
	temp = res.substr(pos, pos2-pos);
	sscanf(temp.c_str(), "%ld", &memtotal);//%d changed to %ld

	pos = res.find("MemFree:");
	pos = res.find_first_not_of("MemFree: ", pos);
	pos2 = res.find('\n', pos);
	temp = res.substr(pos, pos2-pos);
	sscanf(temp.c_str(), "%ld", &memfree);//%d changed to %ld

	pos = res.find("Buffers:");
	pos = res.find_first_not_of("Buffers: ", pos);
	pos2 = res.find('\n', pos);
	temp = res.substr(pos, pos2-pos);
	sscanf(temp.c_str(), "%ld", &buffers);//%d changed to %ld

	pos = res.find("Cached:");
	pos = res.find_first_not_of("Cached: ", pos);
	pos2 = res.find('\n', pos);
	temp = res.substr(pos, pos2-pos);
	sscanf(temp.c_str(), "%ld", &cached);//%d changed to %ld

	long int iTemp = memtotal - memfree - buffers - cached;
	int iUsedMem = iTemp / 1024;
	return iUsedMem;
}

int SysLinux3_2_0::getBlockSize()
{
	if(m_iBlockSize <= 0)
		return addError("unsupported!");
	return m_iBlockSize;
}

int SysLinux3_2_0::getPageSize()
{
	if(m_iPageSize <= 0)
		return addError("unsupported!");
	return m_iPageSize;
}

int SysLinux3_2_0::getLoadAvg1()
{
	std::string res = readProcFile("/proc/loadavg");
	float la1, la5, la15;
	sscanf(res.c_str(), "%f %f %f ", &la1, &la5, &la15);
	int iRes = static_cast<int>(la1*100);
	return iRes;
}

int SysLinux3_2_0::getLoadAvg5()
{
	std::string res = readProcFile("/proc/loadavg");
	float la1, la5, la15;
	sscanf(res.c_str(), "%f %f %f ", &la1, &la5, &la15);
	int iRes = static_cast<int>(la5*100);
	return iRes;
}

int SysLinux3_2_0::getLoadAvg15()
{
	std::string res = readProcFile("/proc/loadavg");
	float la1, la5, la15;
	sscanf(res.c_str(), "%f %f %f ", &la1, &la5, &la15);
	int iRes = static_cast<int>(la15*100);
	return iRes;
}

int SysLinux3_2_0::getProcessNums()
{
	return addError("unsupported!");
}

std::vector<proc_info_detail> SysLinux3_2_0::getProcInfo()
{
	m_oProcInfo.clear();
	return m_oProcInfo;
}

std::string SysLinux3_2_0::readProcFile(const char * filepath)
{
	FILE *fp = fopen(filepath, "r");
	std::string res;
	if(fp == NULL)
		return res;
	char tmp;
	while((tmp = fgetc(fp)) != EOF)
	{
		if(tmp == '\0')
			res.push_back(' ');
		else if(tmp == '\t')
			res.push_back(' ');
		else
			res.push_back(tmp);
	}
	fclose(fp);
	return res;
}

fsinfo SysLinux3_2_0::getFSInfo()
{
	return m_ofsinfo;
}

int SysLinux3_2_0::getInNetRate()
{
	struct timeval curtime;
	gettimeofday(&curtime, NULL);
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
		pos = res.find(":", pos);
		pos2 = res.find_first_not_of("1234567890 ", pos);
		pos = res.find_first_not_of(":", pos);

		temp = res.substr(pos, pos2-pos);
		int i;
		sscanf(temp.c_str(), "%ld", &m_lLastInBytes);//%d changed to %ld
		for(i = 0; i < 8; i++)
		{
			pos = temp.find_first_not_of(' ');
			pos2 = temp.find_first_not_of("1234567890", pos);
			temp = temp.substr(pos2, temp.size()-pos2);
		}
		sscanf(temp.c_str(), "%ld", &m_lLastOutBytes);//%d changed to %ld
	}
	//printf("m_lLastInBytes=%d m_lLastOutBytes=%d\n",m_lLastInBytes,m_lLastOutBytes);//added
	
	int inRate = static_cast<int>(1.0*(m_lLastInBytes-inBytes)/(curtime.tv_sec-m_oLastCalltime.tv_sec));
	m_oLastCalltime.tv_sec = curtime.tv_sec;
	m_oLastCalltime.tv_usec = curtime.tv_usec;
	return inRate;
}

int SysLinux3_2_0::getOutNetRate()
{
	struct timeval curtime;
	gettimeofday(&curtime, NULL);
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
		pos = res.find(":", pos);
		pos2 = res.find_first_not_of("1234567890 ", pos);
		pos = res.find_first_not_of(":", pos);

		temp = res.substr(pos, pos2-pos);
		int i;
		sscanf(temp.c_str(), "%ld", &m_lLastInBytes);//%d changed to %ld
		for(i = 0; i < 8; i++)
		{
			pos = temp.find_first_not_of(' ');
			pos2 = temp.find_first_not_of("1234567890", pos);
			temp = temp.substr(pos2, temp.size()-pos2);
		}
		sscanf(temp.c_str(), "%ld", &m_lLastOutBytes);//%d changed to %ld
	}
	//printf("m_lLastInBytes=%d m_lLastOutBytes=%d\n",m_lLastInBytes,m_lLastOutBytes);//added
	
	int outRate = static_cast<int>(1.0*(m_lLastOutBytes-outBytes)/(curtime.tv_sec-m_oLastCalltime.tv_sec));
	m_oLastCalltime.tv_sec = curtime.tv_sec;
	m_oLastCalltime.tv_usec = curtime.tv_usec;
	return outRate;
}
}//end namespace os
