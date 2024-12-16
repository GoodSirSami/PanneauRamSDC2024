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
#include "screen.hpp"
#include <deque>

class TPartage : public TSingleton<TPartage>
{
    friend class TSingleton<TPartage>;
private:
    TMutex *mutex;
    TScreen *screen;

    bool mRecu;
    bool aRecu;
    bool dRecu;
    bool pRecu;
    bool sendD;
    bool sendA;
    bool sendM;
    bool sendP;
    float poids;
    int filling[3];

    std::deque<std::string> fileTx;
    
    TPartage(void);
    ~TPartage();

    TPartage(const TPartage &) = delete;
	TPartage &operator=(const TPartage &) = delete;

public:
    bool echoRecu = true;
    void setMRecu(bool recu);
    void setARecu(bool recu);
    void setDRecu(bool recu);
    void setPRecu(bool recu);
    void setPoids(float pds);
    void setFilling(int *fill);

    bool getMRecu();
    bool getARecu();
    bool getDRecu();
    bool getPRecu();
    float getPoids();
    int getFilling(int index);


    void addFileTx(std::string message);
    bool getSizeFileTxNotZero();
    std::string getFileTx();
};
#endif //PARTAGE_HPP
