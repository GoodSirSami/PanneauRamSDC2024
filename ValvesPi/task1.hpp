#ifndef TTASK1_HPP
#define TTASK1_HPP

#include "thread.hpp"
#include "screen.hpp"
#include <gpiod.h>

class TTask1 : public TThread
{
private:
	TScreen *screen;

	struct gpiod_chip *outputInputChip;
	struct gpiod_line *outputLine10;
	struct gpiod_line *outputLine17;
	struct gpiod_line *inputLine9;
	struct gpiod_line *inputLine27;
	unsigned int ioOffset;
	bool ready;
	bool alimCir1;
	bool alimCir2;

	bool initGpio(void);
	void freeGpio(void);

public:
	TTask1(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu = -1);
	~TTask1();

	void task(void);

	void toggleAlimCir1(void);
	void toggleAlimCir2(void);
};

#endif // TTASK1_HPP
