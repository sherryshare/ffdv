
#define _TEST_SYS_STATE_H
#ifdef _TEST_SYS_STATE_H


#include <iostream>
#include "../SysState.h"
//namespace os{
int main(int argc, char *argv[])
{
	os::SysState *pSys = os::SysState::getInstance();
	sleep(1);
	int iCPUNums = pSys->getCPUNums();
	int iCPURate = pSys->getCPUUsedRate();

	int iMemTotal = pSys->getTotalMem();
	int iMemRate = pSys->getMemUsedRate();

	int iNetInSpeed = pSys->getNetInSpeed();
	sleep(1);//Either getNetInSpeed or getNetOutSpeed should sleep before calling, otherwise it may result 0
	int iNetOutSpeed = pSys->getNetOutSpeed();
	
	int iLoad1 = pSys->getLoadAvg1();
	int iLoad5 = pSys->getLoadAvg5();
	int iLoad15 = pSys->getLoadAvg15();	
	
	std::cout << "CPU:" << iCPUNums << ", used " << iCPURate << "\%\n";
	std::cout << "Mem:" << iMemTotal << "M, used " << iMemRate << "\%\n";
	std::cout << "In:" << iNetInSpeed << "B/s\tOut:" << iNetOutSpeed << "B/s\n";
	std::cout << "LoadAvg: " << iLoad1 << "\%\t " << iLoad5 << "\%\t " << iLoad15 << "\%\n";
	return 1;
}
//}//end namespace os
#endif


