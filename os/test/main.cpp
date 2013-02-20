
#define _TEST_SYS_STATE_H
#ifdef _TEST_SYS_STATE_H


#include <iostream>
#include "../SysState.h"

int main(int argc, char *argv[])
{
	os::SysState *pSys = os::SysState::getInstance();
	sleep(1);
	int iCPUNums = pSys->getCPUNums();
	int iCPURate = pSys->getCPUUsedRate();

	int iMemTotal = pSys->getPhysicalMem();
	int iMemRate = pSys->getMemUsedRate();

	
	int iLoad1 = pSys->getLoadAvg1();
	int iLoad5 = pSys->getLoadAvg5();
	int iLoad15 = pSys->getLoadAvg15();	
	
	std::cout << "CPU:" << iCPUNums << ", used " << iCPURate << "\%\n";
	std::cout << "Mem:" << iMemTotal << "M, used " << iMemRate << "\%\n";
	std::cout << "LoadAvg: " << iLoad1 << "\%\t " << iLoad5 << "\%\t " << iLoad15 << "\%\n";
	return 1;
}

#endif


