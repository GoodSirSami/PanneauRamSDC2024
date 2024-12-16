//============================================================================
// Name        : lock.hpp
// Author      : SDC
// Version     : Labo 4
// Copyright   : Your copyright notice
// Description : Classe de gestion des verrous
//============================================================================

#ifndef TLOCK_HPP
#define TLOCK_HPP

#include "mutex.hpp"
#include "sem.hpp"

class TLock
{
private:
    TSemaphore *_sem;
    TMutex *_mutex; 

public:
    TLock(TMutex *mutex);
    TLock(TSemaphore *sem);
    ~TLock();
};

#endif // TLOCK_HPP
