#ifndef TTASK1_HPP
#define TTASK1_HPP

#include "thread.hpp"
#include "mqtt.hpp"
#include "partage.hpp"
#include "screen.hpp"
#include <string>
#include "temps.hpp"

class TTask1 : public TThread
{
private:
    TMqtt *mqtt;
    TScreen *screen;
    TPartage *partage;
    TTemps *temps;

public:
    TTask1(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu = -1);
    ~TTask1();

    void task(void);
};

#endif // TTASK1_HPP
