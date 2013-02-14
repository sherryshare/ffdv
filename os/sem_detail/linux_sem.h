#ifndef _LINUX_SEM_H
#define _LINUX_SEM_H

#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
///#include "../../log/log.h" //temporarily removed
namespace os{
class semaphore{
public:
	semaphore(unsigned long ulTimeOut = 10,int iMaxSemCount=1024*1024,int iInitSemCount=0)
	{
		int iRet =0;
		iRet = sem_init(&m_oSem,0,0);
		assert(!iRet);
		if(iRet)
			exit(0);
		iRet = sem_init(&m_oMaxCtrl,0,iMaxSemCount);
		assert(!iRet);
		if(iRet)
			exit(0);
		m_ulTimeOut = ulTimeOut;
	}
	virtual ~semaphore()
	{
		sem_destroy(&m_oSem);
		sem_destroy(&m_oMaxCtrl);
	}

	//p操作,大于0时表示成功，小于时表示超时
	int	p()
	{
		int iRet =0;
		while(0!=(iRet = sem_wait(&m_oSem)))
		{
			if(errno == EINTR)
				continue;
			else
			{assert(0);
			return -1;}
		}
		iRet = sem_post(&m_oMaxCtrl);
		assert(iRet==0);
		return 1;
	}

	//v操作,成功时返回值大于0，否则返回值小于0
	int	v()
	{
		int iRet =0;
		while(0!=(iRet = sem_wait(&m_oMaxCtrl)))
		{
			if(errno == EINTR)
				continue;
			else
			{assert(0);
			return -1;}
		}
		iRet = sem_post(&m_oSem);
		assert(iRet == 0);
		return 1;
	}
protected:
	sem_t		 m_oSem;
	sem_t		 m_oMaxCtrl;	//ctrl the max;
	unsigned long m_ulTimeOut;
	//struct timespec  m_timeout;
};

}//end namespace os
#endif


