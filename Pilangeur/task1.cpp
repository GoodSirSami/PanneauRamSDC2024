/*
	Author 	: SG (Modifier SDC)
	Version : Labo 5
	File : task1.cpp
	Description : Tache Aquisition
	Date 	: 15/10/2024
*/

#include "task1.hpp"

TTask1::TTask1(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu) : TThread(name, shared, policy, priority, cpu)
{
	screen = (TScreen *)shared;
	partage = TPartage::getInstance();
	partageMelangeur = partage->getPartageMel();
	screen->dispStr(1, 3, "Task1 : ");
}

TTask1::~TTask1()
{
}

void TTask1::task(void)
{
	// variable locale
	char strCar[2] = {'-', '\0'};
	std::string strMessage;

	screen->dispStr(1, 8, "Mode      :");
	screen->dispStr(1, 9, "Moteur A  :");
	screen->dispStr(1, 10, "Moteur B  :");
	screen->dispStr(1, 11, "Moteur C  :");
	screen->dispStr(1, 12, "RecetteGo :");
	screen->dispStr(1, 13, "RecetteActuelle :");

	// synchronisation démarrage tâche
	signalStart();
	sleep(2);

	while (1)
	{
		if (partage->getMode()) // mode automatique
		{
			// declenche TaskAuto
			partageMelangeur->semAuto.release();
		}
	
		// traitement
		if (strCar[0] == '|')
			strCar[0] = '-';
		else
			strCar[0] = '|';
		screen->dispStr(9, 3, strCar);

		strMessage = (partage->getMode()) ? "Auto  " : "Manuel";
		screen->dispStr(14, 8, strMessage.c_str());
		strMessage = (partage->getMoteurA()) ? "On " : "Off";
		screen->dispStr(14, 9, strMessage.c_str());
		strMessage = (partage->getMoteurB()) ? "On " : "Off";
		screen->dispStr(14, 10, strMessage.c_str());
		strMessage = (partage->getMoteurC()) ? "On " : "Off";
		screen->dispStr(14, 11, strMessage.c_str());
		strMessage = (partage->getRecetteGo() == TPartage::RECETTE_GO) ? "Go    " : (partage->getRecetteGo() == TPartage::RECETTE_STOP) ? "Stop " : "Flush";
		screen->dispStr(14, 12, strMessage.c_str());

		if (partageMelangeur->status.m_nBRecette > 0)
		{
			char buffer[100];
			sprintf(buffer, "%d,%d,%d          ", partage->getRecette(0).qtyA, partage->getRecette(0).qtyB, partage->getRecette(0).qtyC);
			screen->dispStr(20, 13, buffer);
		}
		else
		{
			screen->dispStr(20, 13, "                                        ");
		}
        
		usleep(50000); // 50 ms
	}
}