
#define _TEST_SYS_STATE_H
#ifdef _TEST_SYS_STATE_H


#include <iostream>
#include "../SysState.h"

int main(int argc, char *argv[])
{
	/*os::SysState *pSys = os::SysState::getInstance();
	sleep(1);
	int16_t iCPUNums = pSys->getCPUNums();
	int16_t iCPURate = pSys->getCPUUsedRate();

	int16_t iMemTotal = pSys->getPhysicalMem();
	int16_t iMemRate = pSys->getMemUsedRate();

	
	int16_t iLoad1 = pSys->getLoadAvg1();
	int16_t iLoad5 = pSys->getLoadAvg5();
	int16_t iLoad15 = pSys->getLoadAvg15();	
	
	std::cout << "CPU:" << iCPUNums << ", used " << iCPURate << "\%\n";
	std::cout << "Mem:" << iMemTotal << "M, used " << iMemRate << "\%\n";
	std::cout << "LoadAvg: " << iLoad1 << "\%\t " << iLoad5 << "\%\t " << iLoad15 << "\%\n";*/
	return 1;
}

#endif


