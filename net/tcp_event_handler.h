#ifndef FFDV_TCP_EVENT_HANDLER_H_
#define FFDV_TCP_EVENT_HANDLER_H_
#include <network.h>

namespace ffdv
{
class TCPEventHandler
{
public:
    virtual void onConnSucc ( ffnet::TCPConnectionBase *pConn ) {};
    virtual void onLostConn ( ffnet::TCPConnectionBase *pConn ) {};
    virtual void onTimer ( const boost::system::error_code & e ) {};
};//end TCPEventHandler




}//end namespace ffdv



#endif
