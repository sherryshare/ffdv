#include <iostream>
#include <network.h>
#include "message.pb.h"
#include "common/log.h"
#include "net_layer.h"
#include "tcp_event_handler.h"
#include "client_event_handler.h"
int main(int argc, char **argv) {
	ffnet::Log::init(ffnet::Log::ERROR, "clnt.log");
	ffnet::NervureConfigure nc("../clnt_net_conf.ini");
	ffdv::NetLayer::instance()->initialize(nc,ffdv::NetLayer::client);
	ffdv::NetLayer::instance()->addHandler(new ffdv::ClientEventHandler);	
	ffdv::NetLayer::instance()->run();
    return 0;
}