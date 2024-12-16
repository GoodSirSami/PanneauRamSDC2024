#ifndef TCOM1_HPP
#define TCOM1_HPP

#include "com.hpp"
#include "screen.hpp"
#include "configuration.hpp"
#include "statesMachine.hpp"
#include "partage.hpp"

class TCom1 : public TCom
{
private:
    TScreen *screen;
    int index;
    TStatesMachine *statesMachine;
    TPartage *partage;

public:
    TCom1(const char *name,void *shared,int32_t priority, baudrate_t baudRate = b115200, parity_t parite = pNONE, dimData_t dimData = dS8, int32_t timeoutRx_ms = 25);
    virtual ~TCom1();

    virtual void rxCar(unsigned char car);
    virtual void rxTimeout(void);

    static std::string getCom1Config(std::string nameFichierConfig = "./com.def");
};
#endif //TCOM1_HPP