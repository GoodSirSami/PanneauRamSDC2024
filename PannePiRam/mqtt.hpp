#ifndef MQTT_HPP
#define MQTT_HPP

#include <mosquittopp.h>
#include <string>
#include <unordered_map>
#include <functional>
#include "screen.hpp"
#include "ram.hpp"

using namespace std;

class TMqtt : public mosqpp::mosquittopp
    {
    private:
		unordered_map<string,std::function<void (TMqtt *,string)>> mqttTable;
		TScreen *screen;
		TRam *ram;

		void messageCmdMode(string payload);
		void messageCmdPompe(string payload);
		void messageCmdVEEC(string payload);
		void messageCmdVEEF(string payload);
		void messageCmdVEC(string payload);
		void messageCmdVEF(string payload);
		void messageCmdConsNGB(string payload);
		void messageCmdConsNPB(string payload);
		void messageCmdConsTPB(string payload);
		void messageCmdVGB(string payload);
		void messageCmdVPB(string payload);
		void messageManValvePB(string payload);
		void messageManValveGB(string payload);
		void messageAlmDGB(string payload);
		void messageAlmDPB(string payload);
		void messageAlmNGB(string payload);
		void messageAlmNPB(string payload);
		void messageSetAlmLHGB(string payload);
		void messageSetAlmTGGB(string payload);
		void messageSetAlmTRGB(string payload);
		void messageSetAlmLHPB(string payload);
		void messageSetAlmTGPB(string payload);
		void messageSetAlmTRPB(string payload);

    public:
		TMqtt(void *shared,const char *id,char *host,int port = 1883);
		virtual ~TMqtt();

		void on_connect(int rc);
		void on_message(const struct mosquitto_message *message);
		void on_subscribe(int mid,int qos_count,const int *granted_pos);

		static void getName(string &name);
    };

#endif //MQTT_HPP
