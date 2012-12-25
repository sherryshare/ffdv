#ifndef FFDV_MASTER_EVENT_HANDLER_H_
#define FFDV_MASTER_EVENT_HANDLER_H_
#include "tcp_event_handler.h"
#include "message.pb.h"
namespace ffdv
{
using namespace boost::asio;
using namespace boost::asio::ip;
class MasterEventHandler : public TCPEventHandler
{
public:
    MasterEventHandler();
    virtual void onConnSucc ( ffnet::TCPConnectionBase *pConn );
    virtual void onLostConn ( ffnet::TCPConnectionBase *pConn );
protected:
    void onRecvHeartBeat ( boost::shared_ptr<FFDV::HeartBeat> pHeartBeat, ffnet::EndpointPtr_t pEP );
protected:
    typedef std::list<ffnet::TCPConnectionBase *> TCPConnectionBaseContainer_t;
    TCPConnectionBaseContainer_t m_oTCPConnections;
};//end class ClientEventHandler

}//end namespace ffdv


#endif
