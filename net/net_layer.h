#ifndef FFDV_NET_LAYER_H_
#define FFDV_NET_LAYER_H_

#include <network.h>
namespace ffdv
{
class TCPEventHandler;
class NetLayer
{
public:
    typedef enum {
        master,
        client,
    } NetRoll_t;
public:
    static boost::shared_ptr<NetLayer>		instance();
    bool addHandler ( TCPEventHandler * pHandler );
    void initialize ( const ffnet::NervureConfigure & nc, NetRoll_t nr );
    ffnet::ProtoBufNervure & nervure();
    void run();
protected:
    NetLayer();

    void	onConnSucc ( ffnet::TCPConnectionBase *pConn );
    void onLostConn ( ffnet::TCPConnectionBase *pConn );
    void onTimer ( boost::asio::deadline_timer * pTimer,  uint32_t seconds, const boost::system::error_code & e );

protected:
    typedef std::list<TCPEventHandler * > TCPEventHandlerContainer_t;
    typedef boost::shared_ptr<boost::asio::deadline_timer> Timer_t;
    static boost::shared_ptr<NetLayer>		s_pInstance;
    TCPEventHandlerContainer_t m_oTCPEventHandlers;
    ffnet::ProtoBufNervure m_oProtoBufNervure;
    Timer_t 		m_pTimer;

};//end of class NetLayer
}//end of namespace ffdv

#endif

