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
#include <string>
#include "sem.hpp"
#include "singleton.hpp"
#include "mutex.hpp"
#include "lock.hpp"

class TPartage : public TSingleton<TPartage>
{
    friend class TSingleton<TPartage>;
private:
    TMutex *mutex;
    std::string sPoids;
    std::string sTare;
    bool unite;
    bool brut;
    bool changeTable[4];
    bool timeoutChecker;
    bool ack = false;
    
    TPartage(void);
    ~TPartage();

    TPartage(const TPartage &) = delete;
	TPartage &operator=(const TPartage &) = delete;

public:
    void setChangeTable(int index, bool value);
    bool getChangeTable(int index);
    float getFPoids(void);
    float getFTare(void);

    void setSPoids(std::string poids);
    void setSTare(std::string tare);
    std::string getSPoids(void) const;
    std::string getSTare(void) const;

    bool getUnite(void);
    bool getBrut(void);
    void setUnite(bool value);
    void setBrut(bool value);

    void setTimeoutChecker(void);
    void resetTimeoutChecker(void);
    bool getTimeoutChecker(void);

    void setAck(void);
    void resetAck(void);
    bool getAck(void);
};
#endif //PARTAGE_HPP