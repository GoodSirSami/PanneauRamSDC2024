/*
 * acquisition.cpp
 *
 *  Created on: 2018-11-23
 *  Author: SG
 */

#include "acquisition.hpp"

TAcquisition::TAcquisition(const char *name, void *shared, int policy, int priority, int noCpu) : TThread(name, shared, policy, priority, noCpu)
{
	screen = (TScreen *)shared;

	ram = TRam::getInstance(screen);
	partageRam = ram->getPartageRam();

	screen->dispStr(1, 4, "Acquisition : ");
}

TAcquisition::~TAcquisition()
{
	syncDestruction.take();
}

void TAcquisition::task(void)
{
	char strCar[2] = {'-', '\0'};

	// synchronisation démarrage tâche
	signalStart();

	while (1)
	{
		// traitement
		syncDestruction.take();

		ram->acquisition();

		ram->format(); // format donnee brute

		// déclenche tâche alarme
		partageRam->synAlarme.release();
		if (ram->getMode() != 1) // mode auto
		{
			// déclenche tâche control
			partageRam->synControl.release();
		}

		if(ram->getDebordementGrosBassin() == 0)
		{
			ram->setPompe(0);
			partageRam->alarmes.debordementGrosBassin = 1;
			if (partageRam->alarmes.ackDGB == 1)
			{
				partageRam->alarmes.ackDGB = 0;
			}
		}
		else
		{
			partageRam->alarmes.debordementGrosBassin = 0;
		}

		if(ram->getDebordementPetitBassin() == 0)
		{
			ram->setPompe(0);
			partageRam->alarmes.debordementPetitBassin = 1;
			if (partageRam->alarmes.ackDPB == 1)
			{
				partageRam->alarmes.ackDPB = 0;
			}
		}
		else
		{
			partageRam->alarmes.debordementPetitBassin = 0;
		}

		syncDestruction.release();

		if (strCar[0] == '|')
			strCar[0] = '-';
		else
			strCar[0] = '|';
		screen->dispStr(15, 4, strCar);

		char message[50];
        sprintf(message, "Niveau Gros Bassin : %5.2f", partageRam->status.niveauGrosBassin);
        screen->dispStr(1, 10, message);
        sprintf(message, "Niveau Petit Bassin : %5.2f", partageRam->status.niveauPetitBassin);
        screen->dispStr(1, 11, message);
        sprintf(message, "Valve Gros Bassin : %5.2f", partageRam->status.valveGrosBassin);
        screen->dispStr(1, 12, message);
        sprintf(message, "Valve Petit Bassin : %5.2f", partageRam->status.valvePetitBassin);
        screen->dispStr(1, 13, message);
		sprintf(message, "Température Gros Bassin : %5.2f", partageRam->status.temperatureGrosBassin);
		screen->dispStr(1, 14, message);
		sprintf(message, "Température Petit Bassin : %5.2f", partageRam->status.temperaturePetitBassin);
		screen->dispStr(1, 15, message);
		sprintf(message, "Consigne Niveau Gros Bassin : %5.2f", partageRam->status.consigneNiveauGrosBassin);
		screen->dispStr(1, 16, message);
		sprintf(message, "Consigne Niveau Petit Bassin : %5.2f", partageRam->status.consigneNiveauPetitBassin);
		screen->dispStr(1, 17, message);
		sprintf(message, "Consigne Température Petit Bassin : %5.2f", partageRam->status.consigneTemperaturePetitBassin);
		screen->dispStr(1, 18, message);
		sprintf(message, "Eau Chaude : %5.2f", partageRam->status.valveEauChaude);
		screen->dispStr(50, 12, message);
		sprintf(message, "Eau Froide : %5.2f", partageRam->status.valveEauFroide);
		screen->dispStr(50, 13, message);
		sprintf(message, "Pompe : %d", ram->getPompe());
		screen->dispStr(50, 14, message);
		sprintf(message, "Mode : %d", ram->getMode());
		screen->dispStr(50, 15, message);
		sprintf(message, "Eau Chaude : %d", ram->getEauChaude());
		screen->dispStr(50, 16, message);
		sprintf(message, "Eau Froide : %d", ram->getEauFroide());
		screen->dispStr(50, 17, message);
		sprintf(message, "Valve Purge GB : %5.2f", ram->getManValveGB());
		screen->dispStr(50, 18, message);
		sprintf(message, "Valve Purge PB : %5.2f", ram->getManValvePB());
		screen->dispStr(50, 19, message);

		usleep(50000); // 50ms
	}
}
