
#define _TEST_SYS_STATE_H
#ifdef _TEST_SYS_STATE_H


#include <iostream>
#include "../SysState.h"
//#include <network.h>

int main(int argc, char *argv[])
{
	ffnet::Singleton<os::SysState>::instance_ref().run();
	sleep(1);
	int16_t iCPUNums = ffnet::Singleton<os::SysState>::instance_ref().getCPUNums();
	int16_t iCPURate = ffnet::Singleton<os::SysState>::instance_ref().getCPUUsedRate();

	int16_t iMemTotal = ffnet::Singleton<os::SysState>::instance_ref().getPhysicalMem();
	int16_t iMemRate = ffnet::Singleton<os::SysState>::instance_ref().getMemUsedRate();

	
	int16_t iLoad1 = ffnet::Singleton<os::SysState>::instance_ref().getLoadAvg1();
	int16_t iLoad5 = ffnet::Singleton<os::SysState>::instance_ref().getLoadAvg5();
	int16_t iLoad15 = ffnet::Singleton<os::SysState>::instance_ref().getLoadAvg15();	
	
	std::cout << "CPU:" << iCPUNums << ", used " << iCPURate << "\%\n";
	std::cout << "Mem:" << iMemTotal << "M, used " << iMemRate << "\%\n";
	std::cout << "LoadAvg: " << iLoad1 << "\%\t " << iLoad5 << "\%\t " << iLoad15 << "\%\n";
	return 1;
}

#endif


