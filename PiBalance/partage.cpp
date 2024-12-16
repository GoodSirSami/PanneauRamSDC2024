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
    timeoutChecker = 0;
}

TPartage::~TPartage() 
{
    delete mutex;
}

float TPartage::getFPoids(void)
{
    TLock lock(mutex);
    return std::stof(sPoids);
}

float TPartage::getFTare(void)
{
    TLock lock(mutex);
    return std::stof(sTare);
}

std::string TPartage::getSPoids(void) const
{
    TLock lock(mutex);
    return sPoids;
}

std::string TPartage::getSTare(void) const
{
    TLock lock(mutex);
    return sTare;
}

void TPartage::setSPoids(std::string poids)
{
    TLock lock(mutex);
    sPoids = poids;
}

void TPartage::setSTare(std::string tare)
{
    TLock lock(mutex);
    sTare = tare;
}

bool TPartage::getUnite(void)
{
    TLock lock(mutex);
    return unite;
}

bool TPartage::getBrut(void)
{
    TLock lock(mutex);
    return brut;
}

void TPartage::setUnite(bool value)
{
    TLock lock(mutex);
    unite = value;
}

void TPartage::setBrut(bool value)
{
    TLock lock(mutex);
    brut = value;
}

void TPartage::setChangeTable(int index, bool value)
{
    TLock lock(mutex);
    changeTable[index] = value;
}

bool TPartage::getChangeTable(int index)
{
    TLock lock(mutex);
    return changeTable[index];
}

void TPartage::setTimeoutChecker(void)
{
    TLock lock(mutex);
    timeoutChecker = 1;
}

void TPartage::resetTimeoutChecker(void)
{
    TLock lock(mutex);
    timeoutChecker = 0;
}

bool TPartage::getTimeoutChecker(void)
{
    TLock lock(mutex);
    return timeoutChecker;
}

