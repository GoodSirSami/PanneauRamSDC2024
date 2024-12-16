#include "statesMachine.hpp"

TStatesMachine::TStatesMachine(TScreen *pScreen)
{
    screen = pScreen;
    partage = TPartage::getInstance();

    tabStates[RXID] = &TStatesMachine::stateRxId;
    tabStates[RXFC] = &TStatesMachine::stateRxFC;
    tabStates[RXDATA0] = &TStatesMachine::stateRxData0;
    tabStates[RXDATAX] = &TStatesMachine::stateRxDataX;
    tabStates[RXCRCH] = &TStatesMachine::stateRxCrcH;
    tabStates[RXCRCL] = &TStatesMachine::stateRxCrcL;

    

    state = RXID;
}

TStatesMachine::~TStatesMachine()
{
}

void TStatesMachine::run(unsigned char data)
{
    state = (this->*tabStates[state])(data);
}


TStatesMachine::states_t TStatesMachine::stateRxId(uint8_t data)
{
    trameIndex = 0;
    trame[trameIndex++] = data;
    return RXFC;
}

TStatesMachine::states_t TStatesMachine::stateRxFC(uint8_t data)
{
    trame[trameIndex++] = data;
    return RXDATA0;
}

TStatesMachine::states_t TStatesMachine::stateRxData0(uint8_t data)
{
    trame[trameIndex++] = data;
    if (trame[1] == 0x80)
    {
        return RXCRCH;
    }
    else if (trame[1] == 0x03)
    {
        nb_Bytes = data;
    }
    return RXDATAX;
}

TStatesMachine::states_t TStatesMachine::stateRxDataX(uint8_t data)
{
    trame[trameIndex++] = data;
    if (--nb_Bytes == 0)
        return RXCRCH;
    return RXDATAX;
}

TStatesMachine::states_t TStatesMachine::stateRxCrcH(uint8_t data)
{
    screen->dispStr(1, 13, "CRC Check");
    trame[trameIndex++] = data;
    return RXCRCL;
}

TStatesMachine::states_t TStatesMachine::stateRxCrcL(uint8_t data)
{
    screen->dispStr(1, 13, "CRC Check");
    trame[trameIndex++] = data;
    uint16_t tmpCRC = calculerCRC(trame, trameIndex - 2);
    char str[32];
    sprintf(str, "CRC : %04X", tmpCRC);
    screen->dispStr(1, 14, str);
    sprintf(str, "CRC : %04X", (((uint16_t)trame[trameIndex-1] ) | (trame[trameIndex-2]<< 8)));
    screen->dispStr(1, 15, str);
    if (tmpCRC == (((uint16_t)trame[trameIndex-1] ) | (trame[trameIndex-2]<< 8)))
    {
        screen->dispStr(1, 13, "CRC OK    ");
        if(trame[1] == 0x03)
        {   
            float value;
            uint8_t *ptr = (uint8_t *)&value;
            ptr[0] = trame[6];
            ptr[1] = trame[5];
            ptr[2] = trame[4];
            ptr[3] = trame[3];
            switch (partage->getCmd())
            {
            case TPartage::VAN:
                if (value != partage->getVAN())
                {
                    partage->setVAN(value);
                    partage->setChangeTable(TPartage::VAN, true);
                }
                partage->setCmd(TPartage::VBN);
                break;
            case TPartage::VBN:
                if (value != partage->getVBN())
                {
                    partage->setVBN(value);
                    partage->setChangeTable(TPartage::VBN, true);
                }
                partage->setCmd(TPartage::VAB);
                break;
            case TPartage::VAB:
                if (value != partage->getVAB())
                {
                    partage->setVAB(value);
                    partage->setChangeTable(TPartage::VAB, true);
                }
                partage->setCmd(TPartage::IA);
                break;
            case TPartage::IA:
                if (value != partage->getIA())
                {
                    partage->setIA(value);
                    partage->setChangeTable(TPartage::IA, true);
                }
                partage->setCmd(TPartage::IB);
                break;
            case TPartage::IB:
                if (value != partage->getIB())
                {
                    partage->setIB(value);
                    partage->setChangeTable(TPartage::IB, true);
                }
                partage->setCmd(TPartage::KW);
                break;
            case TPartage::KW:
                if (value != partage->getKW())
                {
                    partage->setKW(value);
                    partage->setChangeTable(TPartage::KW, true);
                }
                partage->setCmd(TPartage::PF);
                break;
            case TPartage::PF:
                if (value != partage->getPF())
                {
                    partage->setPF(value);
                    partage->setChangeTable(TPartage::PF, true);
                }
                partage->setCmd(TPartage::KWH);
                break;
            case TPartage::KWH:
                if (value != partage->getKWH())
                {
                    partage->setKWH(value);
                    partage->setChangeTable(TPartage::KWH, true);
                }
                partage->setCmd(TPartage::VAN);
                break;
            default:
                break;
            }
            partage->setValueRecieved();
        }
    }
    else
    {
        screen->dispStr(1, 13, "CRC ERROR");
    }
    return RXID;
}
