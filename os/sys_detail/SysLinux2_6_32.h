/*
 * =====================================================================================
 *
 *       Filename:  SysLinux2_6_32.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010年07月11日 20时06分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef _SYS_LINUX2_6_32_H
#define _SYS_LINUX2_6_32_H

///#include "../../common/TMheader.h" //temporarily removed
#include "ISysInfo.h"
#include <sys/time.h>
#include <unistd.h>

namespace os{
class SysLinux2_6_32 : public ISysInfo
{
	public:
		SysLinux2_6_32();
		virtual ~SysLinux2_6_32();

		inline virtual std::string getOSVersion();
		inline virtual int getCPUNums();
		virtual int getCPUUsedRate();
		inline virtual int getCoreNums();
		virtual std::vector<cpu_info_detail> getCPUInfo();

		inline virtual int getPhysicalMem();
		virtual int getPhysicalUsedMem();
		inline virtual int getBlockSize();
		inline virtual int getPageSize();

		virtual int getLoadAvg1();
		virtual int getLoadAvg5();
		virtual int getLoadAvg15();

		virtual int getProcessNums();
		virtual std::vector<proc_info_detail> getProcInfo();

		virtual fsinfo getFSInfo();

		virtual int getInNetRate();
		virtual int getOutNetRate();

	protected:
		std::string readProcFile(const char * filePath);
		std::string m_strOSVersion;
		int m_iCPUNums;
		int m_iCoreNums;
		std::vector<cpu_info_detail> m_oCPUInfo;
		unsigned long m_ulCputime;
		unsigned long m_ulIdletime;
		int m_iPhysicalMem;
		int m_iBlockSize;
		int m_iPageSize;
		std::vector<proc_info_detail> m_oProcInfo;
		fsinfo m_ofsinfo;
		struct timeval m_oLastCalltime;
		long m_lLastInBytes;
		long m_lLastOutBytes;

};

}//end namespace os
#endif
