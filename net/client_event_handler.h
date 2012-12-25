#ifndef FFDV_CLIENT_EVENT_HANDLER_H_
#define FFDV_CLIENT_EVENT_HANDLER_H_
#include "tcp_event_handler.h"

namespace ffdv
{
class ClientEventHandler : public TCPEventHandler
{
public:
    ClientEventHandler();
    virtual void onConnSucc ( ffnet::TCPConnectionBase *pConn );
    virtual void onLostConn ( ffnet::TCPConnectionBase *pConn );
    virtual void onTimer ( const boost::system::error_code & e );
protected:
    void	sendHeartBeatMsg ( ffnet::EndpointPtr_t tp );
    ffnet::EndpointPtr_t m_pMasterEndpointPtr;
};//end class ClientEventHandler

}//end namespace ffdv


#endif
