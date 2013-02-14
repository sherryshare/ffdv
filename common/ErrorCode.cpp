#include "ErrorCode.h"

ErrorCode::ErrorCode(void)
{
	m_iCurrentErrorCode = 0;

	//设定成功的情况。
	m_oError.insert(std::make_pair(1,std::string("No error happened! Success!")));
}

ErrorCode::~ErrorCode(void)
{
}

int ErrorCode::addError( const char * strError )
{
	std::string strErrorContent(strError);
	m_iCurrentErrorCode --;
	m_oError.insert(std::pair<int,std::string>(m_iCurrentErrorCode,strErrorContent));
	return m_iCurrentErrorCode;
}

const char * ErrorCode::getError( int errorCode ) const
{
	std::map<int ,std::string>::const_iterator it;
	if(errorCode>0)
		errorCode = 1;
	it = m_oError.find(errorCode);
	if(it == m_oError.end())
		return NULL;
	else
		return it->second.c_str();
}

