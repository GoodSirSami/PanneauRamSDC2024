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
    partageMelangeur.status.m_Mode = false; // Mode manuel
    partageMelangeur.status.m_Moteurs[0] = false; // Moteur A
    partageMelangeur.status.m_Moteurs[1] = false; // Moteur B
    partageMelangeur.status.m_Moteurs[2] = false; // Moteur C
    partageMelangeur.status.m_RecetteGo = RECETTE_STOP; // Recette non active
    partageMelangeur.status.b_Poids = 0.0;
    partageMelangeur.status.s_ShopVacFinished = false;
    partageMelangeur.status.m_EchoGood = true;
    partageMelangeur.status.m_nBRecette = 0;
    partageMelangeur.semAuto = 0; 
    partageMelangeur.mutex = new TMutex();    
}

TPartage::~TPartage() 
{
    delete partageMelangeur.mutex;
    delete screen;
}

void TPartage::setMode(bool mode)
{
    TLock lock(partageMelangeur.mutex);
    partageMelangeur.status.m_Mode = mode;
}

bool TPartage::getMode(void)
{
    TLock lock(partageMelangeur.mutex);
    return partageMelangeur.status.m_Mode;
}

void TPartage::setMoteurA(bool moteurA)
{
    TLock lock(partageMelangeur.mutex);
    partageMelangeur.status.m_Moteurs[0] = moteurA;
}

bool TPartage::getMoteurA(void)
{
    TLock lock(partageMelangeur.mutex);
    return partageMelangeur.status.m_Moteurs[0];
}

void TPartage::setMoteurB(bool moteurB)
{
    TLock lock(partageMelangeur.mutex);
    partageMelangeur.status.m_Moteurs[1] = moteurB;
}

bool TPartage::getMoteurB(void)
{
    TLock lock(partageMelangeur.mutex);
    return partageMelangeur.status.m_Moteurs[1];
}

void TPartage::setMoteurC(bool moteurC)
{
    TLock lock(partageMelangeur.mutex);
    partageMelangeur.status.m_Moteurs[2] = moteurC;
}

bool TPartage::getMoteurC(void)
{
    TLock lock(partageMelangeur.mutex);
    return partageMelangeur.status.m_Moteurs[2];
}

void TPartage::setRecetteGo(recetteGo_e recetteGo)
{
    TLock lock(partageMelangeur.mutex);
    partageMelangeur.status.m_RecetteGo = recetteGo;
}

TPartage::recetteGo_e TPartage::getRecetteGo(void)
{
    TLock lock(partageMelangeur.mutex);
    return partageMelangeur.status.m_RecetteGo;
}

void TPartage::addRecette(int recette[3])
{
    TLock lock(partageMelangeur.mutex);
    recette_t recetteTmp;
    recetteTmp.qtyA = recette[0];
    recetteTmp.qtyB = recette[1];
    recetteTmp.qtyC = recette[2];
    partageMelangeur.status.m_nBRecette++;
    partageMelangeur.status.m_Recette.push_back(recetteTmp);
}

TPartage::recette_t TPartage::getRecette(int index)
{
    TLock lock(partageMelangeur.mutex);
    return partageMelangeur.status.m_Recette[index];
}

void TPartage::removeRecette()
{
    TLock lock(partageMelangeur.mutex);
    partageMelangeur.status.m_Recette.pop_front();
    partageMelangeur.status.m_nBRecette--;
    if (partageMelangeur.status.m_nBRecette == 0)
    {
        partageMelangeur.status.m_RecetteGo = RECETTE_STOP;
    }
    else if (partageMelangeur.status.m_nBRecette < 0)
    {
        partageMelangeur.status.m_nBRecette = 0;
    }
    
}

void TPartage::setPoids(float poids)
{
    TLock lock(partageMelangeur.mutex);
    partageMelangeur.status.b_Poids = poids;
}

float TPartage::getPoids(void)
{
    TLock lock(partageMelangeur.mutex);
    return partageMelangeur.status.b_Poids;
}

bool TPartage::getChangeTable(int index)
{
    TLock lock(partageMelangeur.mutex);
    return partageMelangeur.changeTable[index];
}

void TPartage::setChangeTable(int index, bool change)
{
    TLock lock(partageMelangeur.mutex);
    partageMelangeur.changeTable[index] = change;
}

TPartage::partageMel_t *TPartage::getPartageMel()
{
    return &partageMelangeur;
}

bool TPartage::getSendTx(void)
{
    TLock lock(partageMelangeur.mutex);
    return partageMelangeur.status.m_SendTx.size() > 0;
}

std::string TPartage::getOldestTx(void)
{
    TLock lock(partageMelangeur.mutex);
    std::string str = partageMelangeur.status.m_SendTx.front();
    partageMelangeur.status.m_SendTx.pop_front();
    return str;
}

void TPartage::setSendTx(std::string sendTx)
{
    TLock lock(partageMelangeur.mutex);
    partageMelangeur.status.m_SendTx.push_back(sendTx);
}







