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
	mqtt = new TMqtt(screen, "Mqtt - Aspi", nameMqtt.c_str());

	screen->dispStr(1, 3, "Task MQTT (CPU :  ) :");
	screen->dispStr(30, 3, nameMqtt.c_str());
	screen->dispStr(1, 4, "Poids :");
	screen->dispStr(1, 5, "Action :");
	screen->dispStr(1, 6, "Etat :");
	screen->dispStr(1, 7, "Niv A :");
	screen->dispStr(1, 8, "Niv B :");
	screen->dispStr(1, 9, "Niv C :");
	screen->dispStr(1, 10, "Mel.. :");
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
	std::string message;

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

        if (partage->getDRecu())
		{
			partage->setDRecu(0);
			message = "FINISHED";
			screen->dispStr(10, 6, message + "    ");
			mqtt->publish(NULL, "ram/shopvac/etats/sequence", message.length(), message.c_str());
		}

		if (partage->getARecu())
		{
			partage->setARecu(0); // que faire avec le A WTF?
			// message = "FINISHED";
			// mqtt->publish(NULL, "ram/shopvac/etats/sequence", message.length(), message.c_str());
		}

		if(partage->getMRecu())
		{
			partage->setMRecu(0);
			message = "PROCESSING";
			screen->dispStr(10, 6, message + "    ");
			mqtt->publish(NULL, "ram/shopvac/etats/sequence", message.length(), message.c_str());
		}

		if (partage->getPRecu())
		{
			partage->setPRecu(0);

			message = std::to_string(partage->getFilling(0)); // A
			screen->dispStr(10, 7, message + "    ");
			mqtt->publish(NULL, "ram/shopvac/etats/NivA", message.length(), message.c_str());

			message = std::to_string(partage->getFilling(1)); // B
			screen->dispStr(10, 8, message + "    ");
			mqtt->publish(NULL, "ram/shopvac/etats/NivB", message.length(), message.c_str());

			message = std::to_string(partage->getFilling(2)); // C
			screen->dispStr(10, 9, message + "    ");
			mqtt->publish(NULL, "ram/shopvac/etats/NivC", message.length(), message.c_str());
		}
		
		// Alarm checking
		if (mqtt->loop() != 0)
			mqtt->reconnect();
		usleep(250000); // 250 ms
	}
}