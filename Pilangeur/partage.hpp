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
#include <deque>
#include "sem.hpp"
#include "singleton.hpp"
#include "mutex.hpp"
#include "lock.hpp"
#include "screen.hpp"


class TPartage : public TSingleton<TPartage>
{
    friend class TSingleton<TPartage>;

public:
    struct recette_t
    {
        int qtyA;
        int qtyB;
        int qtyC;
    };

    enum recetteGo_e
    {
        RECETTE_GO = 0,
        RECETTE_STOP = 1,
        RECETTE_FLUSH = 2
    };

    struct melangeur_t
    {
        bool m_Mode;
        bool m_Moteurs[3];
        std::deque<recette_t> m_Recette;
        int m_nBRecette;
        recetteGo_e m_RecetteGo;
        std::deque<std::string> m_SendTx;
        bool m_EchoGood;
        float b_Poids;
        bool s_ShopVacFinished;
    };

    struct partageMel_t
    {
        bool changeTable[4];
        melangeur_t status;
        TSemaphore semAuto;
        TMutex *mutex;
    };
    
private:
    partageMel_t partageMelangeur;
    TScreen *screen;
    
    TPartage(void);
    ~TPartage();

    TPartage(const TPartage &) = delete;
	TPartage &operator=(const TPartage &) = delete;

public:
    void setMode(bool mode);
    bool getMode(void);

    void setMoteurA(bool moteurA);
    bool getMoteurA(void);

    void setMoteurB(bool moteurB);
    bool getMoteurB(void);

    void setMoteurC(bool moteurC);
    bool getMoteurC(void);

    void addRecette(int recette[3]);
    recette_t getRecette(int index);
    void removeRecette();

    void setPoids(float poids);
    float getPoids(void);

    void setRecetteGo(recetteGo_e recetteGo);
    recetteGo_e getRecetteGo(void);

    bool getChangeTable(int index);
    void setChangeTable(int index, bool change);

    partageMel_t *getPartageMel(void);

    bool getSendTx(void);
    std::string getOldestTx(void);
    void setSendTx(std::string sendTx);
};
#endif //PARTAGE_HPP
