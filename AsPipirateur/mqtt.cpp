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

	mqttTable["ram/balance/etats/poids"] = &TMqtt::messagePoids;
	mqttTable["ram/shopvac/cmd/force"] = &TMqtt::messageForce;
	mqttTable["ram/melangeur/etats/recetteStatut"] = &TMqtt::messageMelangeur;

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
		screen->dispStr(50, 3, "Connected : OK  ");
		subscribe(NULL, "ram/shopvac/cmd/#", 0);
		subscribe(NULL, "ram/balance/etats/#", 0);
		subscribe(NULL, "ram/melangeur/etats/#", 0);
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

void TMqtt::messagePoids(string val)
{
	float poids;
	try
	{
		poids = stof(val);
		screen->dispStr(10, 4, val.c_str());
		partage->setPoids(poids);
	}
	catch(...)
	{
		screen->dispStr(1, 15, "Erreur de conversion");
	}
}

void TMqtt::messageForce(string val)
{
	if (val == "GO")
	{
		partage->addFileTx("<M>");
	}
	else if (val == "STOP")
	{
		partage->addFileTx("<A>");
	}
	screen->dispStr(10, 5, val.c_str());
}

void TMqtt::messageMelangeur(string val)
{
	if (val == "FINISHED")
	{
		partage->addFileTx("<M>");
	}
	val += "      ";
	screen->dispStr(10, 10, val.c_str());
}


string TMqtt::getMqttConfig(string nameFichierConfig)
{
	TConfiguration config(nameFichierConfig);
	return config.getConfiguration();
}
