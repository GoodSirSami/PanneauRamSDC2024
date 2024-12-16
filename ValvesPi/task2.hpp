#ifndef TTASK2_HPP
#define TTASK2_HPP

#include "thread.hpp"
#include "screen.hpp"
#include "ads1115.h"
#include "mqtt.hpp"
#include <string>

class TTask2 : public TThread
{
private:
	TScreen *screen;
	ADS1115 *ads1115;
	TMqtt *mqtt;

	uint16_t valGB;
	uint16_t valPB;

public:
	TTask2(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu = -1);
	~TTask2();

	void task(void);
};

#endif // TTASK1_HPP
