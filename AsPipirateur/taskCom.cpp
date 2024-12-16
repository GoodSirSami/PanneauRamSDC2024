#include "taskCom.hpp"

TTaskCom::TTaskCom(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu) : TThread(name, shared, policy, priority, cpu)
{
    screen = (TScreen *)shared;
    partage = TPartage::getInstance();
    com1 = new TCom1(TCom1::getCom1Config().c_str(), screen, 91, TCom::b9600, TCom::pNONE, TCom::dS8, 4000);
    com1->start();
    com1->setSignalTimeout(true);
}

TTaskCom::~TTaskCom()
{
    if (com1)
        delete com1;
}

void TTaskCom::task(void)
{
    // Donnee locales
    char txBuffer[100] = {0};
    bool bigD = false;
    signalStart();

    sleep(2);
    while (1)
    {
        if (partage->echoRecu)
        {
            partage->echoRecu = false;
            if (partage->getSizeFileTxNotZero())
            {
                std::string str = partage->getFileTx();
                if (str == "<D>")
                {
                    bigD = true;
                }
                com1->sendTx((void *)str.c_str(), str.size());
                screen->dispStr(1, 11, "TX: " + str + "         ");
            }
            else
            {
                sprintf(txBuffer, "<P%07.1f>", partage->getPoids());
                com1->sendTx((void *)txBuffer, 10);
                std::string tx(txBuffer);
                screen->dispStr(1, 11, "TX: " + tx);
            }
        }
        else if (bigD)
        {
            bigD = false;
            partage->echoRecu = true;
        }
        

        usleep(1000000); // 1s
    }
}
