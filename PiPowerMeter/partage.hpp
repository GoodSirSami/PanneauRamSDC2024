//============================================================================
// Name        : partage.hpp
// Author      : SDC
// Version     : 2.0.0
// Copyright   : Your copyright notice
// Description : Demonsatration de l'utilisation d'un singleton de partage
//============================================================================
#ifndef TPARTAGE_HPP
#define TPARTAGE_HPP

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "sem.hpp"
#include "singleton.hpp"
#include "mutex.hpp"
#include "lock.hpp"

class TPartage : public TSingleton<TPartage>
{
    friend class TSingleton<TPartage>;
private:
    TMutex *mutex;
    uint8_t lastCmd;
    bool valueRecieved = true;
    bool sendCmd = false;
    uint8_t trameCmd[256];
    uint8_t trameLen = 0;
    bool changeTable[9] = {false};
    bool timeoutChecker = false;

    float van;
    float vbn;
    float vab;
    float ia;
    float ib;
    float kwh;
    float kw;
    float pf;
    
    TPartage(void);
    ~TPartage();

    TPartage(const TPartage &) = delete;
	TPartage &operator=(const TPartage &) = delete;

    

public:
    uint8_t getCmd();
    void setCmd(uint8_t cmd);

    bool getValueRecieved();
    void resetValueRecieved();
    void setValueRecieved();

    bool getSendCmd();
    void setSendCmd();
    void resetSendCmd();

    void setTrameCmd(uint8_t *trame, uint8_t len);
    uint8_t *getTrameCmd();
    uint8_t getTrameCmdLen();

    void setVAN(float val);
    void setVBN(float val);
    void setVAB(float val);
    void setIA(float val);
    void setIB(float val);
    void setKWH(float val);
    void setKW(float val);
    void setPF(float val);

    float getVAN();
    float getVBN();
    float getVAB();
    float getIA();
    float getIB();
    float getKWH();
    float getKW();
    float getPF();

    void setChangeTable(uint8_t index, bool val);
    bool getChangeTable(uint8_t index);

    void setTimeoutChecker(bool val);
    bool getTimeoutChecker();

    enum address_t
	{
		VAN,
		VBN,
		VAB,
		IA,
		IB,
		KWH,
		KW,
		PF,
        ALARM
	};

};
#endif // PARTAGE_HPP