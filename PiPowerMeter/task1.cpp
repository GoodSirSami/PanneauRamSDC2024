/*
	Author 	: SG (Modifier SDC)
	Version : Labo 5
	File : task1.cpp
	Description : Ce programme permet de controler 6 modules par le clavier
	Date 	: 15/10/2024
*/

#include "task1.hpp"
#include "temps.hpp"
#include "Elkor.h"

TTask1::TTask1(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu) : TThread(name, shared, policy, priority, cpu)
{
	std::string nameMqtt;

	screen = (TScreen *)shared;
	partage = TPartage::getInstance();

	nameMqtt = TMqtt::getMqttConfig();
	mqtt = new TMqtt(screen, "Mqtt - PM", nameMqtt.c_str());

	screen->dispStr(1, 3, "Task MQTT (CPU :  ) :");
	screen->dispStr(30, 3, nameMqtt.c_str());
	tabCmds[TPartage::VAN] = 0x14;
	tabCmds[TPartage::VBN] = 0x16;
	tabCmds[TPartage::VAB] = 0x1A;
	tabCmds[TPartage::IA] = 0x20;
	tabCmds[TPartage::IB] = 0x22;
	tabCmds[TPartage::KW] = 0x02;
	tabCmds[TPartage::PF] = 0x38;
	tabCmds[TPartage::KWH] = 0x00;

	screen->dispStr(1, 5, "VAN : ");
	screen->dispStr(1, 6, "VBN : ");
	screen->dispStr(1, 7, "VAB : ");
	screen->dispStr(1, 8, "IA : ");
	screen->dispStr(1, 9, "IB : ");
	screen->dispStr(1, 10, "KW : ");
	screen->dispStr(1, 11, "PF : ");
	screen->dispStr(1, 12, "KWH : ");
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

        if (partage->getValueRecieved())
		{
			partage->resetValueRecieved();
			uint8_t trame[256];
			trame[0] = 0x02;
			trame[1] = 0x03;
			trame[2] = 0x03;
			trame[3] = tabCmds[partage->getCmd()];
			trame[4] = 0x00;
			trame[5] = 0x02;
			uint16_t crc = calculerCRC(trame, 6);
			trame[6] = crc >> 8;
			trame[7] = crc & 0xff;
			partage->setTrameCmd(trame, 8);
			partage->setSendCmd();
		}

		if (partage->getChangeTable(TPartage::VAN))
		{
			partage->setChangeTable(TPartage::VAN, false);
			screen->dispStr(7, 5, partage->getVAN());
			std::string str = std::to_string((partage->getVAN())? partage->getVAN() : 0.0);
			mqtt->publish(NULL, "ram/powermeter/etats/Van", str.length(), str.c_str(), 0, true);
		}

		if (partage->getChangeTable(TPartage::VBN))
		{
			partage->setChangeTable(TPartage::VBN, false);
			screen->dispStr(7, 6, partage->getVBN());
			std::string str = std::to_string((partage->getVBN())? partage->getVBN() : 0.0);
			mqtt->publish(NULL, "ram/powermeter/etats/Vbn", str.length(), str.c_str(), 0, true);
		}

		if (partage->getChangeTable(TPartage::VAB))
		{
			partage->setChangeTable(TPartage::VAB, false);
			screen->dispStr(7, 7, partage->getVAB());
			std::string str = std::to_string((partage->getVAB())? partage->getVAB() : 0.0);
			mqtt->publish(NULL, "ram/powermeter/etats/Vab", str.length(), str.c_str(), 0, true);
		}

		if (partage->getChangeTable(TPartage::IA))
		{
			partage->setChangeTable(TPartage::IA, false);
			screen->dispStr(7, 8, partage->getIA());
			std::string str = std::to_string((partage->getIA())? partage->getIA() : 0.0);
			mqtt->publish(NULL, "ram/powermeter/etats/Ia", str.length(), str.c_str(), 0, true);
		}

		if (partage->getChangeTable(TPartage::IB))
		{
			partage->setChangeTable(TPartage::IB, false);
			screen->dispStr(7, 9, partage->getIB());
			std::string str = std::to_string((partage->getIB())? partage->getIB() : 0.0);
			mqtt->publish(NULL, "ram/powermeter/etats/Ib", str.length(), str.c_str(), 0, true);
		}

		if (partage->getChangeTable(TPartage::KW))
		{
			partage->setChangeTable(TPartage::KW, false);
			screen->dispStr(7, 10, partage->getKW());
			std::string str = std::to_string((partage->getKW())? partage->getKW() : 0.0);
			mqtt->publish(NULL, "ram/powermeter/etats/KW", str.length(), str.c_str(), 0, true);
		}

		if (partage->getChangeTable(TPartage::PF))
		{
			partage->setChangeTable(TPartage::PF, false);
			screen->dispStr(7, 11, partage->getPF());
			std::string str = std::to_string((partage->getPF())? partage->getPF() : 0.0);
			mqtt->publish(NULL, "ram/powermeter/etats/FP", str.length(), str.c_str(), 0, true);
		}

		if (partage->getChangeTable(TPartage::KWH))
		{
			partage->setChangeTable(TPartage::KWH, false);
			screen->dispStr(7, 12, partage->getKWH());
			std::string str = std::to_string((partage->getKWH())? partage->getKWH() : 0.0);
			mqtt->publish(NULL, "ram/powermeter/etats/KWh", str.length(), str.c_str(), 0, true);
		}
		
		if (partage->getChangeTable(TPartage::ALARM))
		{
			partage->setChangeTable(TPartage::ALARM, false);
			mqtt->publish(NULL, "ram/alarmes/etats/CNX_PWR", 2, "ON", 0, true);
		}
		

		// Alarm checking
		if (mqtt->loop() != 0)
			mqtt->reconnect();
		usleep(200000); // 200 ms
	}
}