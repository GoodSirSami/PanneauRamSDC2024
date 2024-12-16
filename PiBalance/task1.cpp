/*
	Author 	: SG (Modifier SDC)
	Version : Labo 5
	File : task1.cpp
	Description : Ce programme permet de controler 6 modules par le clavier
	Date 	: 15/10/2024
*/

#include "task1.hpp"
#include "temps.hpp"

TTask1::TTask1(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu) : TThread(name, shared, policy, priority, cpu)
{
	std::string nameMqtt;

	screen = (TScreen *)shared;
	partage = TPartage::getInstance();
	temps = new TTemps();

	nameMqtt = TMqtt::getMqttConfig();
	mqtt = new TMqtt(screen, "Mqtt - Balance", nameMqtt.c_str());

	screen->dispStr(1, 3, "Task MQTT (CPU :  ) :");
	screen->dispStr(30, 3, nameMqtt.c_str());
}

TTask1::~TTask1()
{
	if (mqtt)
		delete mqtt;
	if (temps)
		delete temps;
}

void TTask1::task(void)
{
	// variable locale
	char strCar[2] = {'-', '\0'};

	// synchronisation démarrage tâche
	signalStart();

	sleep(2);

	while (1)
	{
		// traitement
		if (strCar[0] == '|')
			strCar[0] = '-';
		else
			strCar[0] = '|';
		screen->dispStr(23, 3, strCar);

        if (partage->getChangeTable(0))
        {
			partage->setChangeTable(0, false);
            mqtt->publish(NULL, "ram/balance/etats/poids", partage->getSPoids().length(), partage->getSPoids().c_str(), 0, true);
			//mqtt->publish(NULL, "ram/alarmes/etats/CNX_BAL", 0, NULL, 0, true);
        }

        if (partage->getChangeTable(1))
        {
			partage->setChangeTable(1, false);
            mqtt->publish(NULL, "ram/balance/etats/tare", partage->getSTare().length(), partage->getSTare().c_str(), 0, true);
			//mqtt->publish(NULL, "ram/alarmes/etats/CNX_BAL", 0, NULL, 0, true);
        }

        if (partage->getChangeTable(2))
        {
			partage->setChangeTable(2, false);
            mqtt->publish(NULL, "ram/balance/etats/unite", partage->getUnite() ? 2 : 3, partage->getUnite() ? "Kg" : "Lb", 0, true);
			//mqtt->publish(NULL, "ram/alarmes/etats/CNX_BAL", 0, NULL, 0, true);
        }

        if (partage->getChangeTable(3))
		{
			partage->setChangeTable(3, false);
			//mqtt->publish(NULL, "ram/alarmes/etats/CNX_BAL", 2, "ON", 0, true);
		}
			
		
		// Alarm checking
		if (mqtt->loop(200) != 0)
			mqtt->reconnect();
	}
}