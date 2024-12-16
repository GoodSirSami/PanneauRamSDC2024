//============================================================================
// Name        : partage.cpp
// Author      : SDC
// Version     : 2.0.0
// Copyright   : Your copyright notice
// Description : Demonsatration de l'utilisation d'un singleton de partage
//============================================================================

#include "partage.hpp"

TPartage::TPartage(void)
{
    mutex = new TMutex();
}

TPartage::~TPartage() 
{
    delete mutex;
}

uint8_t TPartage::getCmd()
{
    TLock lock(mutex);
    return lastCmd;
}

void TPartage::setCmd(uint8_t cmd)
{
    TLock lock(mutex);
    lastCmd = cmd;
}

bool TPartage::getValueRecieved()
{
    TLock lock(mutex);
    return valueRecieved;
}

void TPartage::resetValueRecieved()
{
    TLock lock(mutex);
    valueRecieved = false;
}

void TPartage::setValueRecieved()
{
    TLock lock(mutex);
    valueRecieved = true;
}

bool TPartage::getSendCmd()
{
    TLock lock(mutex);
    return sendCmd;
}

void TPartage::setSendCmd()
{
    TLock lock(mutex);
    sendCmd = true;
}

void TPartage::resetSendCmd()
{
    TLock lock(mutex);
    sendCmd = false;
}

void TPartage::setTrameCmd(uint8_t *trame, uint8_t len)
{
    TLock lock(mutex);
    memcpy(trameCmd, trame, len);
    trameLen = len;
}

uint8_t *TPartage::getTrameCmd()
{
    TLock lock(mutex);
    return trameCmd;
}

uint8_t TPartage::getTrameCmdLen()
{
    TLock lock(mutex);
    return trameLen;
}

void TPartage::setVAN(float val)
{
    TLock lock(mutex);
    van = val;
}

void TPartage::setVBN(float val)
{
    TLock lock(mutex);
    vbn = val;
}

void TPartage::setVAB(float val)
{
    TLock lock(mutex);
    vab = val;
}

void TPartage::setIA(float val)
{
    TLock lock(mutex);
    ia = val;
}

void TPartage::setIB(float val)
{
    TLock lock(mutex);
    ib = val;
}

void TPartage::setKWH(float val)
{
    TLock lock(mutex);
    kwh = val;
}

void TPartage::setKW(float val)
{
    TLock lock(mutex);
    kw = val;
}

void TPartage::setPF(float val)
{
    TLock lock(mutex);
    pf = val;
}

float TPartage::getVAN()
{
    TLock lock(mutex);
    return van;
}

float TPartage::getVBN()
{
    TLock lock(mutex);
    return vbn;
}

float TPartage::getVAB()
{
    TLock lock(mutex);
    return vab;
}

float TPartage::getIA()
{
    TLock lock(mutex);
    return ia;
}

float TPartage::getIB()
{
    TLock lock(mutex);
    return ib;
}

float TPartage::getKWH()
{
    TLock lock(mutex);
    return kwh;
}

float TPartage::getKW()
{
    TLock lock(mutex);
    return kw;
}

float TPartage::getPF()
{
    TLock lock(mutex);
    return pf;
}

void TPartage::setChangeTable(uint8_t index, bool val)
{
    TLock lock(mutex);
    changeTable[index] = val;
}

bool TPartage::getChangeTable(uint8_t index)
{
    TLock lock(mutex);
    return changeTable[index];
}

void TPartage::setTimeoutChecker(bool val)
{
    TLock lock(mutex);
    timeoutChecker = val;
}

bool TPartage::getTimeoutChecker()
{
    TLock lock(mutex);
    return timeoutChecker;
}


