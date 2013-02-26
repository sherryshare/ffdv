#include <iostream>
#include <network.h>
#include "message.pb.h"
//#include <log.h>
#include "net_layer.h"
#include "tcp_event_handler.h"
#include "master_event_handler.h"
int main(int argc, char **argv) {
	ffnet::Log::init(ffnet::Log::ERROR, "clnt.log");	
	ffnet::NervureConfigure nc("../master_net_conf.ini");
	ffdv::NetLayer::instance()->initialize(nc,ffdv::NetLayer::master);
	ffdv::NetLayer::instance()->addHandler(new ffdv::MasterEventHandler());	
	ffdv::NetLayer::instance()->run();
    return 0;
}