//////////////////////////////////////////////////////////////////////////
/* 
AthrunArthur 2010-5-4,�õ�ϵͳ�ĸ��ؼ������Ϣ
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

	//�õ��߼�CPU�ĸ���
	int						getCPUNums()const;

	//�õ�CPUʹ���ʣ� *100
	int						getCPUUsedRate()const;

	//�õ���ȥ1�����ڵ�ƽ�����أ�*100
	int						getLoadAvg1()const;

	//�õ���ȥ5�����ڵ�ƽ�����أ� *100
	int						getLoadAvg5()const;

	//�õ���ȥ15�����ڵ�ƽ�����أ�*100
	int						getLoadAvg15()const;
	
	//�õ������ڴ��С����λΪM
	int						getTotalMem()const;

	//�õ��ڴ�ʹ���ʣ�*100
	int						getMemUsedRate()const;

	//�õ����̴�С����λΪG
	int						getTotalDisk() const;

	//�õ�����ʣ��ռ䣬��λΪG
	int						getFreeDisk() const;

	//�õ���ȥһ��ʱ���ڵ������������ʣ���λΪKBps
	int						getNetInSpeed() const;

	//�õ���ȥһ��ʱ���������������ʣ���λΪKBps
	int						getNetOutSpeed() const;
protected:
	std::string				getOSVersion() const;
protected:
		
	ISysInfo *				m_pSysInfo;
};
}//end namespace os

#endif
