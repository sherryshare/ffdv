#include "client_event_handler.h"
#include "net_layer.h"
#include "message.pb.h"
namespace ffdv
{
ClientEventHandler::ClientEventHandler()
{

}

void ClientEventHandler::onConnSucc ( ffnet::TCPConnectionBase* pConn )
{
    m_pMasterEndpointPtr = pConn->getRemoteEndpointPtr();
    std::cout<<"connect success"<<std::endl;
    ffnet::EndpointPtr_t tpp = pConn->getRemoteEndpointPtr();
    sendHeartBeatMsg ( tpp );
}

void ClientEventHandler::onLostConn ( ffnet::TCPConnectionBase* pConn )
{
    m_pMasterEndpointPtr = ffnet::EndpointPtr_t();
    std::cout<<"Server lost!"<<std::endl;
    NetLayer::instance()->nervure().stop();
}

void ClientEventHandler::onTimer ( const boost::system::error_code& e )
{
    if ( m_pMasterEndpointPtr ) {
        sendHeartBeatMsg ( m_pMasterEndpointPtr );
    }
}

void ClientEventHandler::sendHeartBeatMsg ( ffnet::EndpointPtr_t tp )
{
    boost::shared_ptr<FFDV::HeartBeat> pMsg ( new FFDV::HeartBeat() );
    pMsg->set_msg ( "ping from client" );
    pMsg->set_id ( 1 );

    ffnet::NetNervure::send ( pMsg, tp );

    std::cout<<"service running..."<<std::endl;

}




}//end namespace ffdv
