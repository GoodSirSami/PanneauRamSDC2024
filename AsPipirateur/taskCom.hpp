#ifndef TTASKCOM_HPP
#define TTASKCOM_HPP

#include "thread.hpp"
#include "partage.hpp"
#include "screen.hpp"
#include <string>
#include "com.hpp"
#include "com1.hpp"

class TTaskCom : public TThread
{
private:
    TScreen *screen;
    TPartage *partage;
    TCom1 *com1;

public:
    TTaskCom(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu = -1);
    ~TTaskCom();

    void task(void);
};

#endif // TTASKCOM_HPP