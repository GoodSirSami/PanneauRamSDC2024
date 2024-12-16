#ifndef TTASK1_HPP
#define TTASK1_HPP

#include "thread.hpp"
#include "partage.hpp"
#include "screen.hpp"
#include <string>

class TTask1 : public TThread
{
private:
    TScreen *screen;
    TPartage *partage;
    TPartage::partageMel_t *partageMelangeur;

public:
    TTask1(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu = -1);
    ~TTask1();

    void task(void);
};

#endif // TTASK1_HPP
