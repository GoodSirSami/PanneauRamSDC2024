#include "task2.hpp"
#include "screen.hpp"

TTask2::TTask2(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu) : TThread(name, shared, policy, priority, cpu)
{
	screen = (TScreen *)shared;
	ads1115 = new ADS1115("/dev/i2c-1");
	valGB = 0.0;
	valPB = 0.0;

	std::string host = "172.17.15.102";
	mqtt = new TMqtt(shared, "Valves MQTT", host.c_str());

	screen->dispStr(1, 20, "Task MQTT (CPU :  ) :");
	screen->dispStr(30, 20, host.c_str());

	screen->dispStr(1, 7, "Task 2 (CPU :  ) :");
}

TTask2::~TTask2()
{
	if (ads1115)
		delete ads1115;
	if (mqtt)
		delete mqtt;
}

void TTask2::task(void)
{
	// variable locale
	char strCar[2] = {'-', '\0'};
	char bufStr[100];
	std::string strMessageMqtt;

	// synchronisation démarrage tâche
	signalStart();

	while (1)
	{
		// traitement
		sprintf(bufStr, "%d", sched_getcpu());
		screen->dispStr(15, 7, bufStr);
		if (strCar[0] == '|')
			strCar[0] = '-';
		else
			strCar[0] = '|';
		screen->dispStr(20, 7, strCar);

		valGB = ads1115->readChannelADC(1);
		sprintf(bufStr, "Circuit 1 GB (Canal 1) : %04X %07d %0.2f %%", valGB, valGB, ((valGB / 26000.0) * 100));
		screen->dispStr(1, 17, bufStr);
		sprintf(bufStr, "%0.2f", ((valGB / 26000.0) * 100));
		strMessageMqtt = bufStr;
		mqtt->publish(NULL, "ram/valves/etats/Ouverture_GB", strMessageMqtt.length(), strMessageMqtt.c_str());

		valPB = ads1115->readChannelADC(0);
		sprintf(bufStr, "Circuit 2 LB (Canal 0) : %04X %07d %0.2f %%", valPB, valPB, ((valPB / 26000.0) * 100));
		screen->dispStr(1, 18, bufStr);
		sprintf(bufStr, "%0.2f", ((valPB / 26000.0) * 100));
		strMessageMqtt = bufStr;
		mqtt->publish(NULL, "ram/valves/etats/Ouverture_PB", strMessageMqtt.length(), strMessageMqtt.c_str());

		if(mqtt->loop(200) != 0)
		{
			mqtt->reconnect();
		}
		usleep(1000000); // 500 ms
	}
}
