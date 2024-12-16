#include "com1.hpp"

TCom1::TCom1(const char *name,void *shared,int32_t priority, baudrate_t baudRate, parity_t parite, dimData_t dimData, int32_t timeoutRx_ms) : TCom(name, shared ,priority, baudRate, parite, dimData, timeoutRx_ms)
{
    screen = (TScreen *)shared;
    statesMachine = new TStatesMachine(screen);
    partage = TPartage::getInstance();
    screen->dispStr(1, 5, com);
}

TCom1::~TCom1()
{
    delete statesMachine;
}

void TCom1::rxCar(unsigned char car)
{
    statesMachine->run(car);
}

void TCom1::rxTimeout(void)
{
    if (!partage->getTimeoutChecker())
    {
        partage->setChangeTable(3, true);
        partage->setTimeoutChecker();
    }
}

std::string TCom1::getCom1Config(std::string nameFichierConfig)
{
    TConfiguration config(nameFichierConfig);
	return config.getConfiguration();
}