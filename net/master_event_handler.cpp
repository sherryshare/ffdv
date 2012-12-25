#include "master_event_handler.h"
#include "net_layer.h"

#include "handler/event.h"
namespace ffdv
{
using namespace ffnet::event;
using namespace ffnet::event::more;

MasterEventHandler::MasterEventHandler()
{
    boost::shared_ptr<NetLayer> p = NetLayer::instance();
    p->nervure().addNeedToRecvPkg<FFDV::HeartBeat> (
        boost::bind ( &MasterEventHandler::onRecvHeartBeat,this, _1, _2 ) );
}

void MasterEventHandler::onConnSucc ( ffnet::TCPConnectionBase* pConn )
{
    //TODO Record the IP message
    m_oTCPConnections.push_back ( pConn );
    std::cout << "Connection push_back!" << std::endl;
}


void MasterEventHandler::onRecvHeartBeat ( boost::shared_ptr<FFDV::HeartBeat> pHeartBeat, ffnet::EndpointPtr_t pEP )
{
    std::cout << "HeartBeat received!\n";
}

void MasterEventHandler::onLostConn ( ffnet::TCPConnectionBase* pConn )
{
    std::cout<<"lost connection!"<<std::endl;
    //TODO delete the IP message
    for ( TCPConnectionBaseContainer_t::iterator it = m_oTCPConnections.begin();
            it != m_oTCPConnections.end(); it++ ) {

        if ( *it == pConn ) {
            std::cout << "Connection erased!" << std::endl;
            m_oTCPConnections.erase ( it );
            break;
        }
    }


}


}//end namespace ffdv
