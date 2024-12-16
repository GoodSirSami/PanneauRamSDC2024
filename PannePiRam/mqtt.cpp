#include "mqtt.hpp"
#include "screen.hpp"
#include "ram.hpp"
#include "alarme.hpp"

TMqtt::TMqtt(void *shared, const char *id, char *host, int port) : mosqpp::mosquittopp(id)
{
	screen = (TScreen *)shared;
	ram = TRam::getInstance(screen);

	mqttTable["ram/panneau/cmd/Mode"] = &TMqtt::messageCmdMode;
	mqttTable["ram/panneau/cmd/Pompe"] = &TMqtt::messageCmdPompe;
	mqttTable["ram/panneau/cmd/ValveEEC"] = &TMqtt::messageCmdVEEC;
	mqttTable["ram/panneau/cmd/ValveEEF"] = &TMqtt::messageCmdVEEF;
	mqttTable["ram/panneau/cmd/ValveEC"] = &TMqtt::messageCmdVEC;
	mqttTable["ram/panneau/cmd/ValveEF"] = &TMqtt::messageCmdVEF;
	mqttTable["ram/panneau/cmd/ConsNivGB"] = &TMqtt::messageCmdConsNGB;
	mqttTable["ram/panneau/cmd/ConsNivPB"] = &TMqtt::messageCmdConsNPB;
	mqttTable["ram/panneau/cmd/ConsTempPB"] = &TMqtt::messageCmdConsTPB;
	mqttTable["ram/panneau/cmd/ValveGB"] = &TMqtt::messageCmdVGB;
	mqttTable["ram/panneau/cmd/ValvePB"] = &TMqtt::messageCmdVPB;
	mqttTable["ram/valves/etats/Ouverture_PB"] = &TMqtt::messageManValvePB;
	mqttTable["ram/valves/etats/Ouverture_GB"] = &TMqtt::messageManValveGB;
	mqttTable["ram/alarmes/etats/ALR_GB_OVF"] = &TMqtt::messageAlmDGB;
	mqttTable["ram/alarmes/etats/ALR_PB_OVF"] = &TMqtt::messageAlmDPB;
	mqttTable["ram/alarmes/etats/ALR_GB_NIV_MAX"] = &TMqtt::messageAlmNGB;
	mqttTable["ram/alarmes/etats/ALR_PB_NIV_MAX"] = &TMqtt::messageAlmNPB;
	mqttTable["ram/alarmes/cmd/NivLhGB"] = &TMqtt::messageSetAlmLHGB;
	mqttTable["ram/alarmes/cmd/TgNivGB"] = &TMqtt::messageSetAlmTGGB;
	mqttTable["ram/alarmes/cmd/TrNivGB"] = &TMqtt::messageSetAlmTRGB;
	mqttTable["ram/alarmes/cmd/NivLhPB"] = &TMqtt::messageSetAlmLHPB;
	mqttTable["ram/alarmes/cmd/TgNivPB"] = &TMqtt::messageSetAlmTGPB;
	mqttTable["ram/alarmes/cmd/TrNivPB"] = &TMqtt::messageSetAlmTRPB;

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
		screen->dispStr(30, 7, "(Connected : OK)");
		subscribe(NULL, "ram/panneau/cmd/#", 0);
		subscribe(NULL, "ram/valves/etats/#", 0);
		subscribe(NULL, "ram/alarmes/#", 0);
	}
	else
		screen->dispStr(30, 7, "(Connected : Fail)");

}

void TMqtt::on_message(const struct mosquitto_message *message)
{
	string topic = message->topic;
	string payload = (char *)message->payload;

	auto it = mqttTable.find(topic);
	if (it != mqttTable.end())
		it->second(this, payload);
}

void TMqtt::on_subscribe(int mid, int qos_count, const int *granted_pos)
{
	screen->dispStr(50, 7, "(Subcribe)");
}

void TMqtt::messageCmdMode(string payload)
{
	if(payload == "auto")
	{
		ram->setMode(0);
		ram->setPompe(1);
	}
	else if(payload == "manuel")
	{
		ram->setMode(1);
		ram->init();
	}
}

void TMqtt::messageCmdPompe(string payload)
{
	if(ram->getMode())
	{
		bool pompe = (payload == "on") ? true : false;
		ram->setPompe(pompe);
	}	
}

void TMqtt::messageCmdVEEC(string payload)
{
	if (ram->getMode())
	{
		ram->setEauChaude((payload == "on") ? 1 : 0);
	}
	
}

void TMqtt::messageCmdVEEF(string payload)
{
	if (ram->getMode())
	{
		ram->setEauFroide((payload == "on") ? 1 : 0);
	}
}

void TMqtt::messageCmdVEC(string payload)
{
	if (ram->getMode())
	{
		double valve = stod(payload);
		ram->setValveEauChaude(valve);
	}
}

void TMqtt::messageCmdVEF(string payload)
{
	if (ram->getMode())
	{
		double valve = stod(payload);
		ram->setValveEauFroide(100.0 - valve);
	}
}

void TMqtt::messageCmdConsNGB(string payload)
{
	double consigne = stod(payload);
	if (consigne >= 0 && consigne <= 100)
	{
		ram->setConsigneNiveauGrosBassin(consigne);
	}
}

void TMqtt::messageCmdConsNPB(string payload)
{
	double consigne = stod(payload);
	if (consigne >= 0 && consigne <= 100)
	{
		ram->setConsigneNiveauPetitBassin(consigne);
	}
}

void TMqtt::messageCmdConsTPB(string payload)
{
	double consigne = stod(payload);
	if (consigne >= 0 && consigne <= 100)
	{
		ram->setConsigneTemperaturePetitBassin(consigne);
	}
}

void TMqtt::messageCmdVGB(string payload)
{
	double valve = stod(payload);
	if (ram->getMode())
	{
		ram->setValveGrosBassin(100.0 - valve);
	}
	
}

void TMqtt::messageCmdVPB(string payload)
{
	double valve = stod(payload);
	if (ram->getMode())
	{
		ram->setValvePetitBassin(valve);
	}
}

void TMqtt::messageManValvePB(string payload)
{
	double valve = stod(payload);
	if (valve >= 0 && valve <= 100)
	{
		ram->setManValvePB(valve);
	}
}

void TMqtt::messageManValveGB(string payload)
{
	double valve = stod(payload);
	if (valve >= 0 && valve <= 100)
	{
		ram->setManValveGB(valve);
	}
}

void TMqtt::messageAlmDGB(string payload)
{
	if (payload == "ACK")
	{
		ram->getPartageRam()->alarmes.ackDGB = 1;
	}
}

void TMqtt::messageAlmDPB(string payload)
{
	if (payload == "ACK")
	{
		ram->getPartageRam()->alarmes.ackDPB = 1;
	}
}

void TMqtt::messageAlmNGB(string payload)
{
	if (payload == "ACK")
	{
		ram->getPartageRam()->alarmes.ackNGB = 1;
	}
}

void TMqtt::messageAlmNPB(string payload)
{
	if (payload == "ACK")
	{
		ram->getPartageRam()->alarmes.ackNPB = 1;
	}
}

void TMqtt::messageSetAlmLHGB(string payload)
{
	double niveau;
	try
	{
		niveau = stod(payload);
		if (niveau >= 0 && niveau <= 100)
		{
			ram->getPartageRam()->alarmes.niveauANGB = niveau;
		}
	}
	catch(...)
	{
		
	}
}

void TMqtt::messageSetAlmTGGB(string payload)
{
	double temps;
	try
	{
		temps = stod(payload);
		if (temps >= 0)
		{
			ram->getPartageRam()->alarmes.tempsGenNGB = temps;
		}
	}
	catch(...)
	{
		
	}
}

void TMqtt::messageSetAlmTRGB(string payload)
{
	double temps;
	try
	{
		temps = stod(payload);
		if (temps >= 0)
		{
			ram->getPartageRam()->alarmes.tempsRepNGB = temps;
		}
	}
	catch(...)
	{
		
	}
}

void TMqtt::messageSetAlmLHPB(string payload)
{
	double niveau;
	try
	{
		niveau = stod(payload);
		if (niveau >= 0 && niveau <= 100)
		{
			ram->getPartageRam()->alarmes.niveauANPB = niveau;
		}
	}
	catch(...)
	{
		
	}
}

void TMqtt::messageSetAlmTGPB(string payload)
{
	double temps;
	try
	{
		temps = stod(payload);
		if (temps >= 0)
		{
			ram->getPartageRam()->alarmes.tempsGenNPB = temps;
		}
	}
	catch(...)
	{
		
	}
}

void TMqtt::messageSetAlmTRPB(string payload)
{
	double temps;
	try
	{
		temps = stod(payload);
		if (temps >= 0)
		{
			ram->getPartageRam()->alarmes.tempsRepNPB = temps;
		}
	}
	catch(...)
	{
		
	}
}

void TMqtt::getName(string &name)
{
	name = "172.17.15.102";
}
