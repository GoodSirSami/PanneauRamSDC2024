/*
	Author 	: SG (Modifier SDC)
	Version : Labo 5
	File : mqtt.cpp
	Description : Ce programme permet de controler 6 modules par le clavier
	Date 	: 15/10/2024
*/

#include "mqtt.hpp"
#include "configuration.hpp"

using namespace std;

TMqtt::TMqtt(void *shared, const char *id, const char *host, int port) : mosqpp::mosquittopp(id)
{
	screen = (TScreen *)shared;
	partage = TPartage::getInstance();

	mqttTable["ram/melangeur/cmd/mode"] = &TMqtt::messageMode;
	mqttTable["ram/melangeur/cmd/motA"] = &TMqtt::messageMoteurA;
	mqttTable["ram/melangeur/cmd/motB"] = &TMqtt::messageMoteurB;
	mqttTable["ram/melangeur/cmd/motC"] = &TMqtt::messageMoteurC;
	mqttTable["ram/melangeur/cmd/recette"] = &TMqtt::messageRecette;
	mqttTable["ram/melangeur/cmd/recetteGo"] = &TMqtt::messageRecetteGo;
	mqttTable["ram/balance/etats/poids"] = &TMqtt::messagePoids;
	mqttTable["ram/shopvac/etats/sequence"] = &TMqtt::messageShopVac;

	mosqpp::lib_init();

	connect(host, port, 120);
}

TMqtt::~TMqtt()
{
	mosqpp::lib_cleanup();
}

void TMqtt::on_connect(int rc)
{
	if (rc == 0)
	{
		screen->dispStr(30, 4, "Connected : OK  ");
		subscribe(NULL, "ram/melangeur/cmd/#", 0);
		subscribe(NULL, "ram/shopvac/etats/#", 0);
		subscribe(NULL, "ram/balance/etats/#", 0);
	}
	else
		screen->dispStr(1, 4, "Connected : Fail");
}

void TMqtt::on_message(const struct mosquitto_message *message)
{
	string topic = message->topic;
	string payload = (char *)message->payload;

	auto it = mqttTable.find(topic);
	if (it != mqttTable.end())
		it->second(this, payload);

#if 0
	if(topic.substr(0,7) == "module/")
		{
		if(topic.substr(7,8) == "1")
			{
			partage.setModule1( (payload == "on")? true : false );
			}
		else if(topic.substr(7,8) == "2")
			{
			partage.setModule2( (payload == "on")? true : false );
			}
		else if(topic.substr(7,8) == "3")
			{
			partage.setModule3( (payload == "on")? true : false );
			}
		else if(topic.substr(7,8) == "4")
			{
			partage.setModule4( (payload == "on")? true : false );
			}
		else if(topic.substr(7,8) == "5")
			{
			partage.setModule5( (payload == "on")? true : false );
			}
		else if(topic.substr(7,8) == "6")
			{
			partage.setModule6( (payload == "on")? true : false );
			}
		}
#endif
}

void TMqtt::on_subscribe(int mid, int qos_count, const int *granted_pos)
{
	//screen->dispStr(1, 4, "on_subcribe");
}

void TMqtt::messageMode(string val)
{
	partage->setMode((val == "auto") ? true : (val == "manuel") ? false : partage->getMode());
	if (val == "manuel")
	{
		partage->setRecetteGo(TPartage::RECETTE_STOP);
		partage->setChangeTable(3, true);
	}
	
}

void TMqtt::messageMoteurA(string val)
{
	if (partage->getMode() == 0)
	{
		if (val == "on")
		{
			partage->setSendTx("<MMA>");
		}
		else
		{
			partage->setSendTx("<MAA>");
		}
	}
}

void TMqtt::messageMoteurB(string val)
{
	if (partage->getMode() == 0)
	{
		if (val == "on")
		{
			partage->setSendTx("<MMB>");
		}
		else
		{
			partage->setSendTx("<MAB>");
		}
	}
	
}

void TMqtt::messageMoteurC(string val)
{
	if (partage->getMode() == 0) // MANUEL
	{
		if (val == "on")
		{
			partage->setSendTx("<MMC>");
		}
		else
		{
			partage->setSendTx("<MAC>");
		}
	}
}

void TMqtt::messageRecette(string val)
{
	int recette[3];
	size_t pos = 0;
	string delimiter = "-";
	string token;
	int i = 0;

	while ((pos = val.find(delimiter)) != string::npos)
	{
		token = val.substr(0, pos);
		recette[i] = stoi(token);
		val.erase(0, pos + delimiter.length());
		i++;
	}
	recette[i] = stoi(val);
	partage->addRecette(recette);
}

void TMqtt::messageRecetteGo(string val)
{
	
	if (val == "go" && partage->getMode() == 1)
	{
		partage->setRecetteGo(TPartage::RECETTE_GO);
		partage->setChangeTable(3, true);
	}
	else if (val == "stop")
	{
		partage->setRecetteGo(TPartage::RECETTE_STOP);
		partage->setChangeTable(3, true);
	}
	else if (val == "flush")
	{
		partage->setRecetteGo(TPartage::RECETTE_FLUSH);
		partage->setChangeTable(3, true);
	}
	
}

void TMqtt::messagePoids(string val)
{
	try
	{
		partage->setPoids(stof(val));
	}
	catch(...)
	{
		screen->dispStr(60,9,"Erreur de conversion");
	}
}

void TMqtt::messageShopVac(string val)
{
	if (val == "FINISHED")
	{
		partage->getPartageMel()->status.s_ShopVacFinished = true;
	}
	else
	{
		partage->getPartageMel()->status.s_ShopVacFinished = false;
	}
}

string TMqtt::getMqttConfig(string nameFichierConfig)
{
	TConfiguration config(nameFichierConfig);
	return config.getConfiguration();
}
