#ifndef SEM_H
#define SEM_H

#ifdef WIN32
#include "sem_detail/win_sem.h"
#else
#include "sem_detail/linux_sem.h"
#endif

#endif


