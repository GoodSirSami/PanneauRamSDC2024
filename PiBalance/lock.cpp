//============================================================================
// Name        : lock.cpp
// Author      : SDC
// Version     : Labo 4
// Copyright   : Your copyright notice
// Description : Classe de gestion des verrous
//============================================================================

#include "lock.hpp"

TLock::TLock(TMutex *mutex)
{
    _mutex = mutex;
    _mutex->take();
    _sem = nullptr;

}
TLock::TLock(TSemaphore *sem)
{
    _sem = sem;
    _sem->take();
    _mutex = nullptr;
}

TLock::~TLock()
{
    if (_mutex)
    {
        _mutex->release();
    }
    if (_sem)
    {
        _sem->release();
    }
    
}