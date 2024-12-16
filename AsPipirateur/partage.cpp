//============================================================================
// Name        : partage.cpp
// Author      : SDC
// Version     : 2.0.0
// Copyright   : Your copyright notice
// Description : Demonsatration de l'utilisation d'un singleton de partage
//============================================================================

#include "partage.hpp"
#include <string>

TPartage::TPartage(void)
{
    mutex = new TMutex();
}

TPartage::~TPartage() 
{
    delete mutex;
}

void TPartage::setMRecu(bool recu)
{
    TLock lock(mutex);
    mRecu = recu;
}

void TPartage::setARecu(bool recu)
{
    TLock lock(mutex);
    aRecu = recu;
}

void TPartage::setDRecu(bool recu)
{
    TLock lock(mutex);
    dRecu = recu;
}

void TPartage::setPRecu(bool recu)
{
    TLock lock(mutex);
    pRecu = recu;
}

bool TPartage::getMRecu()
{
    TLock lock(mutex);
    return mRecu;
}

bool TPartage::getARecu()
{
    TLock lock(mutex);
    return aRecu;
}

bool TPartage::getDRecu()
{
    TLock lock(mutex);
    return dRecu;
}

bool TPartage::getPRecu()
{
    TLock lock(mutex);
    return pRecu;
}

void TPartage::setPoids(float pds)
{
    TLock lock(mutex);
    poids = pds;
}

float TPartage::getPoids()
{
    TLock lock(mutex);
    return poids;
}

void TPartage::setFilling(int *fill)
{
    TLock lock(mutex);
    for (int i = 0; i < 3; i++)
        filling[i] = fill[i];
}

int TPartage::getFilling(int index)
{
    TLock lock(mutex);
    return filling[index];
}

void TPartage::addFileTx(std::string str)
{
    TLock lock(mutex);
    fileTx.push_back(str);
}

std::string TPartage::getFileTx()
{
    TLock lock(mutex);
    std::string str = fileTx.front();
    fileTx.pop_front();
    return str;
}

bool TPartage::getSizeFileTxNotZero()
{
    TLock lock(mutex);
    return fileTx.size() > 0;
}


