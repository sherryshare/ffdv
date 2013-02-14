#ifndef _WIN_SEM_H
#define _WIN_SEM_H

#include <Windows.h>
#include <stdio.h>

namespace os{
//windows�µ��ź���
class semaphore{
public:
	semaphore(unsigned long ulTimeOut = 10,int iMaxSemCount=1024*1024,int iInitSemCount=0)
	{
		m_hSemaphore = CreateSemaphore(NULL,iInitSemCount,iMaxSemCount,NULL);
		if(m_hSemaphore == NULL)
		{
			printf("CreateSemaphore error: %d\n",GetLastError());
			exit(0);
		}
		m_ulTimeOut = ulTimeOut;
	};
	virtual ~semaphore()
	{
		CloseHandle(m_hSemaphore);
	};

	int		p()		//p����,����0ʱ��ʾ�ɹ���С��ʱ��ʾ��ʱ
	{
		DWORD dwWaitResult = WaitForSingleObject(m_hSemaphore,(DWORD)m_ulTimeOut);
		int	iRet;
		switch(dwWaitResult)
		{
		case WAIT_OBJECT_0:
			iRet = 1;
			break;
		case WAIT_TIMEOUT:
			iRet = -1;
			break;
		}
		return iRet;
	};
	int		v()		//v����,�ɹ�ʱ����ֵ����0�����򷵻�ֵС��0
	{
		BOOL bRes = ReleaseSemaphore(m_hSemaphore,1,NULL);
		int iRet;
		if(bRes)
			iRet = 1;
		else
			iRet = -1;
		return iRet;
	};
protected:
	HANDLE	m_hSemaphore;
	unsigned long	m_ulTimeOut;
};
}//end namespace os

#endif