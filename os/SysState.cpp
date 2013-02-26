#include "SysState.h"

namespace os
{
using namespace std;
SysState::SysState ( void )
    :m_iCPUNums ( -1 ),
     m_iPhysicalMem ( -1 )
{
    string res = getOSVersion();
    string::size_type  pos;
    pos = res.find ( "Linux" );
    if ( pos!=string::npos ) {	//linux
        string::size_type end_pos;
        istringstream iss;
        char tmp;
        //cpu number
        ifstream cpu_info_file ( "/proc/cpuinfo" );
        while ( !cpu_info_file.eof() ) {
            cpu_info_file.get ( tmp );
            if ( tmp == '\0' ) {
                res.push_back ( ' ' );
            } else if ( tmp == '\t' ) {
                res.push_back ( ' ' );
            } else {
                res.push_back ( tmp );
            }
        }
        cpu_info_file.close();
        if ( !res.empty() ) {
            string temp;
            char none_use[255];
            pos = res.rfind ( "processor" );
            if ( pos != std::string::npos ) {
                end_pos = res.find ( "\n", pos );
                temp = res.substr ( pos, end_pos-pos );
                sscanf ( temp.c_str(), "%s %s %d\n", none_use, none_use, &m_iCPUNums );
                m_iCPUNums++;
            }
        }

        //cpu time
        ifstream stat_file ( "/proc/stat" );
        getline ( stat_file, res );
        stat_file.close();
        if ( !res.empty() ) {
            uint64_t user, nice, sys, idle, iowait, irq, softirq;
            // skip "cpu"
            pos = res.find_first_not_of ( " ", 3 );
            end_pos = res.find ( "\n", pos );
            iss.str ( res.substr ( pos, end_pos - pos ) );
            iss >> user >> nice >> sys >> idle >> iowait >> irq >> softirq;
            iss.clear();
            m_ulCputime = user + nice + sys + idle + iowait + irq + softirq;
            m_ulIdletime = idle;
        }

        //mem info
        ifstream meminfo_file ( "/proc/meminfo" );
        getline ( meminfo_file, res );
        meminfo_file.close();
        if ( !res.empty() ) {
            pos = res.find_first_of ( ':' );
            pos++;
            end_pos = res.find_first_of ( 'k' );
            iss.str ( res.substr ( pos, end_pos - pos ) );
            iss >> m_iPhysicalMem;
            iss.clear();
        }
    } else {		//other os
        assert ( 0 );
    }
}

SysState::~SysState ( void )
{
}

SysState * SysState::getInstance()
{
    static SysState instance;
    return &instance;
}

int32_t SysState::getCPUNums() const
{
    if ( m_iCPUNums < 0 ) {
        return -1;    //unsupported
    }
    return m_iCPUNums;
}

int32_t SysState::getCPUUsedRate()
{
    uint64_t cputime;
    string::size_type pos,end_pos;
    istringstream iss;
    string res;
    ifstream stat_file ( "/proc/stat" );
    getline ( stat_file, res );
    stat_file.close();
    if ( res.empty() ) {
        return -1;//error:unsupported
    }
    uint64_t user, nice, sys, idle, iowait, irq, softirq;
    // skip "cpu"
    pos = res.find_first_not_of ( " ", 3 );
    end_pos = res.find ( "\n", pos );
    iss.str ( res.substr ( pos, end_pos - pos ) );
    iss >> user >> nice >> sys >> idle >> iowait >> irq >> softirq;
    iss.clear();
    cputime = user + nice + sys + idle + iowait + irq + softirq;
    int32_t iUsedRate;
    if ( cputime == m_ulCputime ) {
        iUsedRate = 0;
    } else {
        iUsedRate = static_cast<int32_t> ( 100-100.0* ( idle-m_ulIdletime ) / ( cputime-m_ulCputime ) );
        m_ulCputime = cputime;
        m_ulIdletime = idle;
    }
    return iUsedRate;
}

int32_t SysState::getLoadAvg1() const
{
    string res;
    istringstream iss;
    float la1, la5, la15;
    ifstream loadavg_file ( "/proc/loadavg" );
    getline ( loadavg_file, res );
    loadavg_file.close();
    if ( res.empty() ) {
        return -1;//error:unsupported
    }
    iss.str ( res );
    iss >> la1 >> la5 >> la15;
    iss.clear();
    return static_cast<int32_t> ( la1*100 );
}

int32_t SysState::getLoadAvg5() const
{
    string res;
    istringstream iss;
    float la1, la5, la15;
    ifstream loadavg_file ( "/proc/loadavg" );
    getline ( loadavg_file, res );
    loadavg_file.close();
    if ( res.empty() ) {
        return -1;//error:unsupported
    }
    iss.str ( res );
    iss >> la1 >> la5 >> la15;
    iss.clear();
    return static_cast<int32_t> ( la5*100 );
}

int32_t SysState::getLoadAvg15() const
{
    string res;
    istringstream iss;
    float la1, la5, la15;
    ifstream loadavg_file ( "/proc/loadavg" );
    getline ( loadavg_file, res );
    loadavg_file.close();
    if ( res.empty() ) {
        return -1;//error:unsupported
    }
    iss.str ( res );
    iss >> la1 >> la5 >> la15;
    iss.clear();
    return static_cast<int32_t> ( la15*100 );
}

int32_t SysState::getPhysicalMem() const
{
    if ( m_iPhysicalMem <= 0 ) {
        return -1;    //unsupported
    }
    return m_iPhysicalMem / 1024;
}

int32_t SysState::getMemUsedRate() const
{
    string::size_type pos=0,end_pos;
    istringstream iss;
    string res;
    uint32_t used_mem = m_iPhysicalMem,unused_mem;
    ifstream meminfo_file ( "/proc/meminfo" );
    getline ( meminfo_file, res );
    if ( res.empty() ) {
        return -1;//error:unsupported
    }
    for ( uint32_t i = 0; i < 3; i++ ) {
        getline ( meminfo_file, res );//get a new line
        pos = res.find_first_of ( ':' );
        pos++;
        end_pos = res.find_first_of ( 'k' );
        iss.str ( res.substr ( pos, end_pos - pos ) );
        iss >> unused_mem;
        used_mem -= unused_mem;
    }
    iss.clear();
    meminfo_file.close();
    return static_cast<int32_t> ( 100.0* used_mem  / m_iPhysicalMem );
}

string SysState::getOSVersion() const
{
    string res;
    ifstream version_file ( "/proc/version" );
    getline ( version_file, res );
    version_file.close();
    if ( res.empty() ) {
        return string();//error:unsupported
    }
    return res;
}

}//end namespace os
