// Billion Network Tech Software Inc.
// project:Network 
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

//#include "../common/Header.h"

#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <algorithm>
//#include "win32.h"
#include "PUtils.h"


using namespace std;
S64 PUtils::localMilliClock()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	S64 t = time.tv_sec;
	t *= 1000 ;
	t += time.tv_usec/1000;
	return t;
}
S64 PUtils::localClock()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	S64 t = time.tv_sec;
	t *= 1000 * 1000;
	t += time.tv_usec;
	return t;
}

S32	PUtils::atoi( const S8 * int_str )
{
	return ::atoi(int_str );
}

S8 * PUtils::itoa ( S32 value, S8* str, S32 radix)
{
	assert(10 == radix);
	sprintf(str, "%d", value);
	return str;
}

S64 		PUtils::milliSeconds()
{
#ifdef _MSC_VER
	struct __timeb64 timebuffer;
	_ftime64( &timebuffer);
	return timebuffer.time*1000 + timebuffer.millitm;

#else
	time_t t;
	time(&t);
	return t*1000;
#endif
}

S64 		PUtils::localMilliSeconds()
{
#if (defined WIN32 || defined _WIN32) && defined _MSC_VER
	struct __timeb64 timebuffer;
	_ftime64( &timebuffer);
	return ( timebuffer.time - timebuffer.timezone*60) *1000 + timebuffer.millitm;
#else
	// TODO: need to adjust  to consider local timezone (linux version).
	time_t t;
	time(&t);
	return t*1000;
#endif
}


S32	PUtils::seconds()
{
#ifdef _MSC_VER

	struct __timeb64 timebuffer;
	_ftime64( &timebuffer);
	return (S32)timebuffer.time ;//- timebuffer.timezone*60;
	////__time32_t t = _time32(0);
	////return t;
#else
	time_t t;
	time(&t);
	return t;
#endif

}

S32	PUtils::localSeconds()
{
#ifdef _MSC_VER
	struct __timeb64 timebuffer;
	_ftime64( &timebuffer);
	return (S32)timebuffer.time - timebuffer.timezone*60;
	/*__time32_t t = _time32(0);
	return t;*/
#else
	// TODO: need to adjust  to consider local timezone (linux version).
	time_t t;
	time(&t);
	return t;
#endif

}


void	PUtils::getCurrentDirectory( std::string  & path )
{
	char temp [1024]={0};
#if defined WIN32 || defined _WIN32
	GetCurrentDirectoryA(sizeof(temp), temp);
#else
	getcwd(temp, sizeof(temp));
#endif
	path.append(temp);
}

std::string		PUtils::time2String( U32 time)
{
	// implemented 2008-6-23.tuxuping
	time_t time_temp = (time_t)time;
	const char *fmt = "{%y/%m/%d %H:%M:%S}";
	const U32 TEMP_BUFFER_SIZE = 40;
	char buffer[TEMP_BUFFER_SIZE] = {0};
	struct tm* tmp = ::gmtime(&time_temp);
	strftime(buffer, TEMP_BUFFER_SIZE, fmt, tmp);
	return buffer;
}

void	PUtils::time2String( U32 time, S8 * to, U32 sizeof_to)
{
	time_t time_temp = (time_t)time;
	const char *fmt = "{%y/%m/%d %H:%M:%S}";
	//const U32 TEMP_BUFFER_SIZE = 40;
	//char buffer[TEMP_BUFFER_SIZE] = {0};
	char * buffer = to;
	struct tm* tmp = ::gmtime(&time_temp);
	strftime(buffer, sizeof_to, fmt, tmp);
	return ;
}

U32		PUtils::str2Time( const std::string & str)
{
	return 2000;//TODO: added by tuxuping, 2008-7-25
}

void		PUtils::initSocketEnv( )
{

#if defined WIN32 || defined _WIN32

	WORD wVersionRequested = MAKEWORD( 2, 2 );

	WSADATA wsaData;

	if ( WSAStartup( wVersionRequested, &wsaData ) != 0 )
	{
		// Tell the user that we could not find a usable WinSock DLL.
		//int nErrorNum = WSAGetLastError();


		//LOG_ERROR("Settings","Initialize failed. " );
		//return ERROR;
		exit(-1);
	}
#elif __linux__

	// do nothing.
#endif

}


void		PUtils::uninitSocketEnv( )
{


#if WIN32
	WSACleanup();
#elif __linux__

	// do nothing , when other os platform

#endif


}

S32	PUtils::getFileId( std::string  filename )
{
	return 11; //
}

void PUtils::sleep( U32 millseconds )
{

#if defined WIN32 || defined _WIN32
	Sleep( millseconds );
#else
	usleep(millseconds*1000);
#endif

}



S32	PUtils::getLastErrorCode()
{
#if defined WIN32 || defined _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}



std::string    PUtils::readConfigLine(  std::ifstream&  instream )
{
	// Veryfy the *.ini file format
	S8 buff[1024];

	std::string  buffs;
	S8 *pdest;

	// didn't find the '#' char in the line
	// not the comment line.
	while (true)
	{
		memset(buff,0, sizeof(buff));
		//buffs.clear();
		instream.getline( buff, sizeof(buff));
		//buffs.append(buff);
		//std::string::size_type npos =  buffs.find_first_of("#",0) ;
		pdest = strstr(buff, "#");
		//if(npos==-1 ) {
		//	break;
		//}
		if (pdest == NULL)
		{
#ifdef __linux__
			S32 bufLen = strlen(buff);
			if (buff[bufLen - 1] == '\r')
			{
				buff[bufLen - 1] = 0;
			}

			S32 i = 0;
			for (i=0;i<bufLen-1;i++)
			{
				if (buff[i] != ' ')
				{
					break;
				}
			}
			if (i == bufLen-1)
			{
				continue;
			}
#endif
			break;
		}

	}
	buffs.append(buff);
	return buffs;
}


std::string	PUtils::addr2String(const struct sockaddr_in * from)
{
	std::string rets;
	S8 * temp1 = inet_ntoa(from->sin_addr);
	S8 temp2[6];
	memset(temp2,0,sizeof(temp2));
	itoa(from->sin_port, temp2, 10);
	rets.append(" ");
	rets.append(temp1);
	rets.append(":");
	rets.append(temp2);
	rets.append(" ");
	return rets;
}


void		PUtils::addr2String(const struct sockaddr_in * from, S8* to)
{
	S8 * temp1 = inet_ntoa(from->sin_addr);
	S8 temp2[6];
	memset(temp2,0,sizeof(temp2));
	itoa(from->sin_port, temp2, 10);
	strcpy(to,temp1);
	int len = strlen(temp1) ;
	to[len ] = ' ';	
	strcpy(to+len+1, temp2 );
}

U32		    PUtils::addr2Long( const S8*  addr )
{

	return ntohl(inet_addr(addr));

}
void		PUtils::setZero( S8 * buf, S32 len)
{

#if defined WIN32 || defined _WIN32
	memset( buf, 0, len );
#else
	bzero( buf, len );
#endif


}

void	PUtils::exitProcess( U32  excode )
{
#if defined WIN32 || defined _WIN32
	ExitProcess(excode);
#else
	///LOG_ERROR("PUtils","ExitProcess(): no implemented"); //temporarily removed
#endif

}

//std::string	PUtils::long2Addr( U32	ipaddr)
//{
//
//	std::string rets;
//
//	sockaddr_in addr;
//	addr.sin_addr.s_addr = htonl(ipaddr);
//	S8 * temp=inet_ntoa(addr.sin_addr);
//
//	rets.append(temp);
//	return rets;
//}

S8*	PUtils::long2Addr( U32	ipaddr)
{
	sockaddr_in addr;
	addr.sin_addr.s_addr = htonl(ipaddr);
	S8 * temp=inet_ntoa(addr.sin_addr);
	return temp;
}


std::string  PUtils::addr2String( U32  ipaddr, U16 inport)
{

	std::string rets =long2Addr(ipaddr);

	char temp[6];
	memset(temp,0,sizeof(temp));
	itoa(inport,temp,10);

	rets.append(":");
	rets.append(temp);
	return rets;

}

void  PUtils::addr2String( U32  ipaddr, U16 inport, S8* to_str)
{
#if defined WIN32 || defined _WIN32
#include "winsock2.h"
#endif
	sockaddr_in ip2;
	ip2.sin_addr.s_addr = htonl(ipaddr);	
	S8 * temp1 = inet_ntoa(ip2.sin_addr);
	S8 temp2[6];
	memset(temp2,0,sizeof(temp2));
	itoa(inport, temp2, 10);
	strcpy(to_str,temp1);
	int len = strlen(temp1) ;
	to_str[len ] = ' ';	
	strcpy(to_str+len+1, temp2 );

}



S32		PUtils::searchStr( const S8* str, U32	size, U32 offset, const S8 * substr,U32  substrLen )
{


	//B8 found = 0;
	assert(size > offset);
	S8 tempsubstr[100];
	memset(tempsubstr,0,substrLen);

	const S8 * pointer = strstr(str,substr);


	if ( pointer ==NULL) return -1;
	else
		return (S32)(pointer - str );



	/*for( int i =offset; i<= size - substrLen; i ++ ) {

	if( found ==true )
	break;
	for( int j =0 ; j <= substrLen ; j++ ) {

	if( *(str+i+j)!= *(substr + j)) {
	break;
	}
	}
	if( j ==substrLen +1 )
	found =true;
	}

	if( found ==true )
	return i;
	else
	return -1;
	*/
}
U32    PUtils::getRandomWithin( U32  to ,  U32	from  )
{

	if ( to - from == 0 )
		return 0;

	assert( to - from  > 0 );

	U32  i = rand( ) % ( to - from );

	return i + from;

}

void PUtils::getRandomSequence(  U32 to, U32 from, U32 count, std::set< S32 >&  container )
{
	if( to - from <= count )
	{
		for( U32 i = from; i < to ; i++ )
		{
			container.insert(i);
		}
		return ;
	}
	// here , to!=from
	// 1. count is too big relatively to (to-from)
	//
	// 2. other cases

	if( to-from < 2*count  )
	{
		F32 prop = ((F32)count)/(to-from);
		//S32 threshold = from+count;
		std::vector<S32>  left_container;

		for( U32 i = from ; i < to; i++)
		{
			S32  random_num = PUtils::getRandomWithin( to, from);

			if( random_num <prop)
			{
				container.insert(i);
				if(container.size()==count )
					return ;
			}else
			{
				left_container.push_back(i);
			}
		}
		std::size_t size = container.size();

		assert( size < count );

		for( U32 i = 0 ; i < count - size; i++ )
		{
			container.insert( left_container[i]);
		}
	}
	else
	{
		while ( container.size() < count )
		{

			S32  random_num = PUtils::getRandomWithin( to , from ) ;

			if (  ( container.find( random_num ) == container.end() ))
			{
				container.insert( random_num );
			}
		}
	}
}


//implemented by kimi
B8 PUtils::setDefaultPath2ModulePath()
{

#if defined WIN32 || defined _WIN32
	char path[MAX_PATH];
	DWORD len;
	char* last;

	len = GetModuleFileNameA(NULL, path, MAX_PATH);
	if(len == 0)
		return false;

	last = strrchr(path, '\\');
	if(last == NULL)
		return false;

	*last = '\0';
	return TRUE == SetCurrentDirectoryA(path);
#else
	///LOG_ERROR("PUtils","setDefaultPath2ModulePath(): no implemented"); //temporarily removed
	return false;
#endif


}


//************************************************************************/
//*��ʼ����־ϵͳ*/
//************************************************************************/

void PUtils::initLog4cplus()
{
#ifdef LOG_4_CPLUS
	using namespace std;
	using namespace log4cplus;

	SharedAppenderPtr append_1(new ConsoleAppender());
	append_1->setName("First");

	string pattern = "%-8p %c [%d]: %m %n";
	std::auto_ptr<log4cplus::Layout> _layout(new log4cplus::PatternLayout(pattern) );
	append_1->setLayout( _layout );

	Logger::getRoot().addAppender(append_1);

	LOG_INFO("initLog4cplus", "Logging sub-system init ok!");
#endif
}


void PUtils::initLog4cplus(const char* filename)
{
#ifdef LOG_4_CPLUS
	using namespace std;
	using namespace log4cplus;

	SharedAppenderPtr append_1(new FileAppender(filename));
	append_1->setName("First");

	string pattern = "%-8p %c [%d]: %m %n";
	std::auto_ptr<log4cplus::Layout> _layout(new log4cplus::PatternLayout(pattern) );
	append_1->setLayout( _layout );

	Logger::getRoot().addAppender(append_1);

	LOG_INFO("initLog4cplus", "Logging sub-system init ok!");
#endif
}




//����У���?
U16 PUtils::makeCheckSum(const char *data, U16 len)
{
	//��ԭ����check sum�ֶ���Ϊ0

	//����check sum
	return 0;
}


//���У��͡�true: ͨ��false: ʧ�ܡ�
bool PUtils::verifyCheckSum(const char *data, U16 len)
{
	return true;
}


//added by A.A 2010 6-11,�õ���������Ч����
#ifdef __linux__

std::string PUtils::readProcFile( const char * filePath )
{
	FILE *fp = fopen(filePath,"r");
	std::string res;
	if(fp == NULL)
		return res;
	char tmp;
	while((tmp=fgetc(fp))!=EOF)
	{
		if(tmp=='\0')
			res.push_back(' ');
		else if(tmp == '\t')
			res.push_back(' ');
		else
			res.push_back(tmp);
	}
	fclose(fp);
	return res;
}

std::string PUtils::getValidNetDev()
{
	std::string res = readProcFile("/proc/net/dev");
	std::string tmp;
	std::string final;
	std::string::size_type pos, pos2;
	if(!res.empty())
	{
		pos = 0;
		while(pos!=std::string::npos){
			pos = res.find("eth",pos);
			pos2 = res.find(':',pos);
			if(pos == std::string::npos)
				return final;
			final = res.substr(pos,pos2-pos);	//�õ�����������,
			pos2 = res.find(':',pos);
			pos2 = res.find_first_of("0123456789",pos2);
			tmp = res.substr(pos2,std::string::npos);
			int iTemp;
			sscanf(tmp.c_str(),"%d",&iTemp);
			if(iTemp == 0)
				pos = res.find("eth",pos2);
			else
				return final;
		}
		if(!final.empty())
			return final;
	}
	if(final.empty())
		return std::string("lo");
}

#endif
