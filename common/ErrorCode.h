//////////////////////////////////////////////////////////////////////////
/*
AthrunArthur 2010-4-14
对错误代码的管理，约定小于0时表示出现错误，错误的信息由该管理器记录。
该类用于临时性的管理，不需要串行化发送。
*/
//////////////////////////////////////////////////////////////////////////
#ifndef _ERRORCODE_H
#define _ERRORCODE_H

#include <string>
#include <map>

class ErrorCode
{
public:
	ErrorCode(void);
	virtual ~ErrorCode(void);

	int							addError(const char * strError); //返回值为errorcode，
	
	const char *				getError(int errorCode) const;						//根据errorCode返回strError

protected:
	int					m_iCurrentErrorCode;									//保存当前最小的错误码
	std::map<int,std::string>	m_oError;
};

#endif


