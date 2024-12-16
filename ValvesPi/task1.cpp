#include "task1.hpp"
#include "temps.hpp"

TTask1::TTask1(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu) : TThread(name, shared, policy, priority, cpu)
{
	screen = (TScreen *)shared;
	screen->dispStr(1, 6, "Task 1 (CPU :  ) :");

	ready = false;
	alimCir1 = false;
	alimCir2 = false;

	outputInputChip = nullptr;
	outputLine10 = nullptr;
	outputLine17 = nullptr;
	inputLine9 = nullptr;
	inputLine27 = nullptr;
}

TTask1::~TTask1()
{
	freeGpio();
}

void TTask1::task(void)
{
	// variable locale
	char strCar[2] = {'-', '\0'};
	char bufStr[50];

	// synchronisation démarrage tâche
	signalStart();

	ready = initGpio();

	if (ready)
		screen->dispStr(1, 9, "Init GPIO : OK");
	else
		screen->dispStr(1, 9, "Init GPIO : ERROR");

	while (1)
	{
		// traitement
		sprintf(bufStr, "%d", sched_getcpu());
		screen->dispStr(15, 6, bufStr);
		if (strCar[0] == '|')
			strCar[0] = '-';
		else
			strCar[0] = '|';
		screen->dispStr(20, 6, strCar);

		if (ready)
		{
			alimCir1 = gpiod_line_get_value(inputLine27);
			sprintf(bufStr, "%1d", alimCir1);
			screen->dispStr(20, 14, bufStr);

			alimCir2 = gpiod_line_get_value(inputLine9);
			sprintf(bufStr, "%1d", alimCir2);
			screen->dispStr(20, 15, bufStr);

			usleep(500000);
		}

		usleep(500000);
	}
}

void TTask1::toggleAlimCir1(void)
{
	if (ready)
		gpiod_line_set_value(outputLine17, (alimCir1) ? 0 : 1);
}

void TTask1::toggleAlimCir2(void)
{
	if (ready)
		gpiod_line_set_value(outputLine10, (alimCir2) ? 0 : 1);
}

bool TTask1::initGpio(void)
{
	bool result = true;

	if ((outputInputChip = gpiod_chip_open_lookup("gpiochip0")) != nullptr)
	{
		// output
		ioOffset = 10; // gpio10
		if ((outputLine10 = gpiod_chip_get_line(outputInputChip, ioOffset)) != nullptr)
		{
			// if(gpiod_line_request_output_flags(outputLine10,"AlimCir2",GPIOD_LINE_REQUEST_FLAG_ACTIVE_LOW,
			//	                                                                GPIOD_LINE_ACTIVE_STATE_HIGH) < 0)
			if (gpiod_line_request_output(outputLine10, "AlimCir2", GPIOD_LINE_ACTIVE_STATE_LOW) < 0)
			{
				result = false;
			}
			else
			{
				gpiod_line_set_value(outputLine10, 0);
			}
		}
		else
		{
			result = false;
		}

		ioOffset = 17; // gpio17
		if ((outputLine17 = gpiod_chip_get_line(outputInputChip, ioOffset)) != nullptr)
		{
			if (gpiod_line_request_output(outputLine17, "AlimCir1", GPIOD_LINE_ACTIVE_STATE_LOW) < 0)
			{
				result = false;
			}
			else
			{
				gpiod_line_set_value(outputLine17, 0);
			}
		}
		else
		{
			result = false;
		}

		// input
		ioOffset = 9; // gpio9
		if ((inputLine9 = gpiod_chip_get_line(outputInputChip, ioOffset)) != nullptr)
		{
			if (gpiod_line_request_input(inputLine9, "fbAlimCir2") < 0)
			{
				result = false;
			}
		}
		else
		{
			result = false;
		}

		ioOffset = 27; // gpio27
		if ((inputLine27 = gpiod_chip_get_line(outputInputChip, ioOffset)) != nullptr)
		{
			if (gpiod_line_request_input(inputLine27, "fbAlimCir1") < 0)
			{
				result = false;
			}
		}
		else
		{
			result = false;
		}
	}
	else
	{
		result = false;
	}

	return result;
}

void TTask1::freeGpio(void)
{
	if (outputLine10)
		gpiod_line_release(outputLine10);
	if (outputLine17)
		gpiod_line_release(outputLine17);
	if (inputLine9)
		gpiod_line_release(inputLine9);
	if (inputLine27)
		gpiod_line_release(inputLine27);
	if (outputInputChip)
		gpiod_chip_close(outputInputChip);
}
