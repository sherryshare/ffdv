#include "net_layer.h"
#include "tcp_event_handler.h"
namespace ffdv
{
boost::shared_ptr<NetLayer> NetLayer::s_pInstance;
boost::shared_ptr< NetLayer > NetLayer::instance()
{
    if ( !s_pInstance ) {
        s_pInstance = boost::shared_ptr<NetLayer> ( new NetLayer() );
    }
    return s_pInstance;
}

NetLayer::NetLayer()
{
}

bool NetLayer::addHandler ( TCPEventHandler* pHandler )
{
    if ( pHandler ) {
        m_oTCPEventHandlers.push_back ( pHandler );
        return true;
    } else {
        return false;
    }
}

ffnet::ProtoBufNervure& NetLayer::nervure()
{
    return m_oProtoBufNervure;
}

void NetLayer::run()
{
    m_oProtoBufNervure.run();
}

void NetLayer::initialize ( const ffnet::NervureConfigure& nc, NetLayer::NetRoll_t nr )
{
    if ( nr==master ) {
        uint16_t port = nc.get<uint16_t> ( "tcp-server.port" );
        m_oProtoBufNervure.initTCPServer ( port );
    } else if ( nr == client ) {
        String ip = nc.get<String> ( "tcp-client.target-svr-ip-addr" );
        uint16_t port = nc.get<uint16_t> ( "tcp-client.target-svr-port" );
        ffnet::EndpointPtr_t ep ( new ffnet::Endpoint ( ffnet::tcp_v4, boost::asio::ip::address_v4::from_string ( ip ), port ) );
        m_oProtoBufNervure.addTCPClient ( ep );
        uint32_t seconds = nc.get<uint32_t> ( "conf.heart-beat-interval" );
        m_pTimer = Timer_t ( new boost::asio::deadline_timer ( m_oProtoBufNervure.getIOService(),
                             boost::posix_time::seconds ( seconds )
                                                             ) );
        m_pTimer->async_wait ( boost::bind ( &NetLayer::onTimer, this, m_pTimer.get(), seconds,  _1 ) );
    }

    ffnet::event::Event<ffnet::event::tcp_get_connection>::listen (
        &m_oProtoBufNervure, boost::bind ( &NetLayer::onConnSucc, this, _1 ) );
    ffnet::event::Event<ffnet::event::tcp_lost_connection>::listen
    ( &m_oProtoBufNervure,boost::bind ( &NetLayer::onLostConn, this, _1 ) );
}



void NetLayer::onConnSucc ( ffnet::TCPConnectionBase* pConn )
{
    for ( TCPEventHandlerContainer_t::iterator it = m_oTCPEventHandlers.begin();
            it != m_oTCPEventHandlers.end(); ++it ) {
        ( *it )->onConnSucc ( pConn );
    }
}

void NetLayer::onLostConn ( ffnet::TCPConnectionBase* pConn )
{
    for ( TCPEventHandlerContainer_t::iterator it = m_oTCPEventHandlers.begin();
            it != m_oTCPEventHandlers.end(); it++ ) {
        ( *it )->onLostConn ( pConn );
    }
}

void NetLayer::onTimer ( boost::asio::deadline_timer* pTimer, uint32_t seconds,const boost::system::error_code& e )
{
    if ( !e ) {
        for ( TCPEventHandlerContainer_t::iterator it = m_oTCPEventHandlers.begin();
                it != m_oTCPEventHandlers.end(); it++ ) {
            ( *it )->onTimer ( e );
        }

        pTimer->expires_from_now ( boost::posix_time::seconds ( seconds ) );
        pTimer->async_wait ( boost::bind ( &NetLayer::onTimer, this, pTimer, seconds, _1 ) );
    } else {
        //std::cout << e.message()<<std::endl;
        //std::cout<< m_oProtoBufNervure.getIOService().stopped()<<std::endl;
    }
}



}//end namespace ffdv;
