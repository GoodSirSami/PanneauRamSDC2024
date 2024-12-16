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

	//mqttTable["alarmes/etats/CNX_BAL"] = &TMqtt::messageCNX;

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
		screen->dispStr(1, 21, "Connected : OK  ");
		subscribe(NULL, "ram/valves/etats/#");
	}
	else
	{
		screen->dispStr(1, 21, "Connected : Fail");
	}
		
}

void TMqtt::on_message(const struct mosquitto_message *message)
{
	string topic = message->topic;
	string payload = (char *)message->payload;

	// screen->dispStr(1, 12, (char *)message->topic);
	// screen->dispStr(15, 12, "    ");
	// screen->dispStr(15, 12, (char *)message->payload);

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

string TMqtt::getMqttConfig(string nameFichierConfig)
{
	TConfiguration config(nameFichierConfig);
	return config.getConfiguration();
}
