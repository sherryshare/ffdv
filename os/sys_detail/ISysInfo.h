//////////////////////////////////////////////////////////////////////////
/*
AthrunArthur 2010-5-4���Բ�ͬϵͳ��ͳһ�ӿڣ��õ�ϵͳ�е������Ϣ
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
	
//CPU��ϸ��Ϣ
struct cpu_info_detail{
	int			m_iCoreNums;	//����CPU�Ϻ˵ĸ���
	int			m_iMHZ;			//�������ϵ���Ƶ����Ϊ�����˵���Ƶ��һ�µġ�
	std::vector<int>			m_iUsedRate;	//�����˵�ʹ���ʰٷֱȣ���ʹ����Ϊ48%�����ֵΪ48
	//more information can be added here
	//���糬�̣߳���ˮ����ȣ�cache��С��
};

//������ϸ��Ϣ
struct proc_info_detail{
	unsigned long		m_ulPID;	//���̱�ʶ
	unsigned long		m_ulParentPID;	//�����̱�ʶ
	int			m_iKUsedMem;			//����ռ�õ��ڴ�
	int			m_iUsedCPU;				//����ռ�õ�CPU����*100
	int			m_iThrdNums;			//�����е��߳���
	
	std::string	m_strProcName;		//������
	std::string	m_strProcInfo;		//��������
};

//�ļ�ϵͳ��Ϣ
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
	long		f_bsize;	//���С���ֽ���
	long		f_blocks;	//�ļ�ϵͳ���ݿ�����
	long		f_bfree;	//���ÿ���
	long		f_files;	//�ļ��ڵ�����
	long		f_ffree;	//�����ļ��ڵ���
	long		f_namelen;	//�ļ�������󳤶�
}fsinfo;

class ISysInfo{
public:
	const char *		getError(int errCode) const{return m_oErr.getError(errCode);};			//�õ�������Ϣ

//For OS
	//�õ�����ϵͳ�İ汾��Ϣ
	virtual std::string	getOSVersion() =0;

//For CPU
	//�õ�ϵͳCPU����,�ɹ�ʱ����CPU���������򷵻�ֵС��0������ֵ������getError�����õ�����ԭ��
	virtual int			getCPUNums() =0;
	
	//�õ�ϵͳ���ܼƵ�CPUʹ���ʣ���ʹ����Ϊ48%�����ֵΪ48
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getCPUUsedRate() = 0;

	//�õ�ϵͳ�˸������ɹ�ʱ���غ˵ĸ��������򷵻�ֵС��0������ֵ������getError()�õ�����ԭ��
	//�˸���=CPU��*ÿ��CPU�ϵĺ��������磬˫·�ĺ����а˸���
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getCoreNums() =0;

	//�õ�ϵͳCPU����ϸ��Ϣ
	virtual std::vector<cpu_info_detail>	getCPUInfo() =0;

//For memory
	//�õ�ϵͳ�������ڴ��С����λΪM
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getPhysicalMem() =0;

	//�õ�ϵͳ���Ѿ�ʹ�õ������ڴ��С����λΪM
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getPhysicalUsedMem() = 0;

	//�õ�ϵͳ�п��С����λΪK
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getBlockSize() = 0;

	//�õ�ϵͳ��ҳ���С����λΪK
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getPageSize() = 0;

//For load average
	//�õ�ϵͳ�й�ȥ1�����ڵ�ƽ�����г��ȣ����Ϊ		ʵ�ʳ���*100
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getLoadAvg1() = 0;

	//�õ�ϵͳ�й�ȥ5�����ڵ�ƽ�����г��ȣ����Ϊ		ʵ�ʳ���*100
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getLoadAvg5() = 0;

	//�õ�ϵͳ�й�ȥ15�����ڵ�ƽ�����г��ȣ����Ϊ		ʵ�ʳ���*100
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getLoadAvg15() = 0;

//For processes
	//�õ�ϵͳ�н�����
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual int			getProcessNums() = 0;

	//�õ����̵���ϸ��Ϣ
	//�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	virtual std::vector<proc_info_detail>	getProcInfo() = 0;

//For disk
	//�õ����̵������Ϣ
	virtual fsinfo		getFSInfo() = 0;

//For network	added by A.A 2010 5-27
	//�õ�����������ʺͳ����ʣ���λΪKBps
	virtual int			getInNetRate() =0;
	virtual int			getOutNetRate() = 0;

protected:
	int					addError(const char *strErr){return m_oErr.addError(strErr);};
protected:
	ErrorCode			m_oErr;
};

}//end namespace os

#endif


