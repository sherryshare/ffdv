//////////////////////////////////////////////////////////////////////////
/*
AthrunArthur 2010-5-4���������linux�ں�2.6.26�汾���������汾����֤�����ԣ�
����ʹ����/proc�ļ�ϵͳ�е��ļ������ڲ�ͬ�汾���ں˶��ļ����ֶεĶ�����һ���Ĳ��
����������汾���ں���ʹ��ʱ���������
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
	//�õ�����ϵͳ�İ汾��Ϣ
	inline virtual std::string	getOSVersion();

//For CPU
	//�õ�ϵͳCPU����,�ɹ�ʱ����CPU���������򷵻�ֵС��0������ֵ������getError�����õ�����ԭ��
	inline virtual int			getCPUNums();

	//�õ�ϵͳ���ܼƵ�CPUʹ���ʣ���ʹ����Ϊ48%�����ֵΪ48
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getCPUUsedRate();

	//�õ�ϵͳ�˸������ɹ�ʱ���غ˵ĸ��������򷵻�ֵС��0������ֵ������getError()�õ�����ԭ��
	//�˸���=CPU��*ÿ��CPU�ϵĺ��������磬˫·�ĺ����а˸���
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	inline virtual int			getCoreNums();

	//�õ�ϵͳCPU����ϸ��Ϣ
	virtual std::vector<cpu_info_detail>	getCPUInfo();

//For memory
	//�õ�ϵͳ�������ڴ��С����λΪM
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	inline virtual int			getPhysicalMem();

	//�õ�ϵͳ���Ѿ�ʹ�õ������ڴ��С����λΪM
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getPhysicalUsedMem();

	//�õ�ϵͳ�п��С����λΪK
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	inline virtual int			getBlockSize();

	//�õ�ϵͳ��ҳ���С,��λΪK
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	inline virtual int			getPageSize();

//For load average
	//�õ�ϵͳ�й�ȥ1�����ڵ�ƽ�����г��ȣ����Ϊ		ʵ�ʳ���*100
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getLoadAvg1();

	//�õ�ϵͳ�й�ȥ5�����ڵ�ƽ�����г��ȣ����Ϊ		ʵ�ʳ���*100
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getLoadAvg5();

	//�õ�ϵͳ�й�ȥ15�����ڵ�ƽ�����г��ȣ����Ϊ		ʵ�ʳ���*100
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getLoadAvg15();

//For processes
	//�õ�ϵͳ�н�����
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getProcessNums();

	//�õ����̵���ϸ��Ϣ
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual std::vector<proc_info_detail>	getProcInfo();

//For disk
	//�õ����̵������Ϣ
	virtual fsinfo		getFSInfo();

//For network
	//�õ�����������ʺͳ����ʣ���λΪKBps
	virtual int			getInNetRate();
	virtual int			getOutNetRate();

protected:
	//��\proc\...file,���������ļ����ַ�����
	std::string			readProcFile(const char * filePath);

protected:
//For OS
	std::string			m_strOSVersion;	//����ϵͳ�汾��Ϣ�������ʼ��
//For CPU
	int					m_iCPUNums;		//CPU�����������ʼ��
	int					m_iCoreNums;	//�˵������������ʼ��

	std::vector<cpu_info_detail>	m_oCPUInfo;	//CPU��Ϣ������ʱ���¸�ֵ��

	unsigned long		m_ulCputime;	//�ϴε���ʱ��CPUʱ��
	unsigned long		m_ulIdletime;	//�ϴε���ʱ�Ŀ���CPUʱ��
//For Memory
	int					m_iPhysicalMem;	//�����ڴ��С����λΪM�������ʼ��
	int					m_iBlockSize;	//ϵͳ�п��С����λΪK�������ʼ��
	int					m_iPageSize;	//ϵͳ��ҳ���С����λΪK�������ʼ��
//For process
	std::vector<proc_info_detail>	m_oProcInfo;	//������Ϣ������ʱ��ֵ��

//For file system
	fsinfo				m_ofsinfo;

//For network
	struct timeval		m_oLastCalltime;	//�ϴλ�����������ʱ�䣬�����ʼ��
	long				m_lLastInBytes;		//�ϴλ���������ʱ��Ӧ�����ֽ���
	long				m_lLastOutBytes;	//�ϴλ���������ʱ��Ӧ�ĳ��ֽ���
};
}//end namespace os

#endif

