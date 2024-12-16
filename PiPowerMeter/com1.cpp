#include "com1.hpp"

TCom1::TCom1(const char *name,void *shared,int32_t priority, baudrate_t baudRate, parity_t parite, dimData_t dimData, int32_t timeoutRx_ms) : TCom(name, shared ,priority, baudRate, parite, dimData, timeoutRx_ms)
{
    screen = (TScreen *)shared;
    partage = TPartage::getInstance();
    statesMachine = new TStatesMachine(screen);
    screen->dispStr(1, 2, "Com1 : ");
    screen->dispStr(7, 2, name);
    screen->dispStr(19, 2, com);
    index = 0;
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
        partage->setChangeTable(TPartage::ALARM, true);
        partage->setTimeoutChecker(true);
    }
}

std::string TCom1::getCom1Config(std::string nameFichierConfig)
{
    TConfiguration config(nameFichierConfig);
	return config.getConfiguration();
}