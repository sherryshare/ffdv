#include "SysState.h"
#include <stdio.h>
#include "sys_detail/SysLinux2_6_26.h"
#include "sys_detail/SysLinux2_6_18.h"
#include "sys_detail/SysLinux2_6_32.h"
#include "sys_detail/SysLinux2_6_33.h"
#include "sys_detail/SysLinux3_2_0.h"
#include <assert.h>///added

namespace os{
	using namespace std;
SysState::SysState(void)
{
	std::string res = getOSVersion();
	std::string::size_type  pos;
	pos = res.find("Linux");
	if(pos!=std::string::npos)		//linux系统
	{
		pos = res.find("2.6.26");	//linux 2.6.26
		if(pos!=std::string::npos)
			m_pSysInfo = new SysLinux2_6_26();
		else if((pos = res.find("2.6.18"))!=std::string::npos)		//linux 2.6.18
			m_pSysInfo = new SysLinux2_6_18();
		else if((pos = res.find("2.6.32")) != std::string::npos)
			m_pSysInfo = new SysLinux2_6_32();
		else if((pos = res.find("2.6.33")) != std::string::npos)
			m_pSysInfo = new SysLinux2_6_33();
		else if((pos = res.find("3.2.0")) != std::string::npos)
		{
			printf("Find kernel 3.2.0!\n");//added
			m_pSysInfo = new SysLinux3_2_0();
			//m_pSysInfo = new SysLinux2_6_33();
		}
		else{
			printf("Other kernel version!\n");//其他版本的linux系统，暂时不支持
			assert(0);
		}
	}
	else			//非linxu系统,暂不支持
		assert(0);
}

SysState::~SysState(void)
{
	if(m_pSysInfo)
		delete m_pSysInfo;
}

SysState * SysState::getInstance()
{
	static SysState instance;
	return &instance;
}

int SysState::getCPUNums() const
{
	return m_pSysInfo->getCPUNums();
}

int SysState::getCPUUsedRate() const
{
	return m_pSysInfo->getCPUUsedRate();
}

int SysState::getLoadAvg1() const
{
	return m_pSysInfo->getLoadAvg1();
}

int SysState::getLoadAvg5() const
{
	return m_pSysInfo->getLoadAvg5();
}

int SysState::getLoadAvg15() const
{
	return m_pSysInfo->getLoadAvg15();
}

int SysState::getTotalMem() const
{
	return m_pSysInfo->getPhysicalMem();
}

int SysState::getMemUsedRate() const
{
	//Altered!
	return static_cast<int>(100.0*(m_pSysInfo->getPhysicalUsedMem())/(m_pSysInfo->getPhysicalMem()));
}

std::string SysState::getOSVersion() const
{
	FILE *fp = fopen("/proc/version","r");
	if(fp == NULL)
		return std::string();
	std::string res;
	char temp;
	while((temp = fgetc(fp))!=EOF)
	{
		if(temp == '\0')
			res.push_back(' ');
		else if(temp == '\t')
			res.push_back(' ');
		else res.push_back(temp);
	}
	fclose(fp);
	return res;
}

int SysState::getTotalDisk() const
{
	fsinfo buf = m_pSysInfo->getFSInfo();
	return static_cast<int>(1.0* buf.f_blocks * buf.f_bsize/1024/1024/1024);
}

int SysState::getFreeDisk() const
{
	fsinfo buf = m_pSysInfo->getFSInfo();
	return static_cast<int>(1.0*buf.f_bfree * buf.f_bsize/1024/1024/1024);
}

int SysState::getNetInSpeed() const
{
	return m_pSysInfo->getInNetRate();
}

int SysState::getNetOutSpeed() const
{
	return m_pSysInfo->getOutNetRate();
}
}//end namespace os