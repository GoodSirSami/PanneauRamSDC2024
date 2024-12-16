#include "statesMachine.hpp"

TStatesMachine::TStatesMachine(TScreen *pScreen)
{
    screen = pScreen;
    partage = TPartage::getInstance();

    tabStates[RX_STX] = &TStatesMachine::rxStx;
    tabStates[RX_MOT_A] = &TStatesMachine::rxMotA;
    tabStates[RX_MOT_B] = &TStatesMachine::rxMotB;
    tabStates[RX_MOT_C] = &TStatesMachine::rxMotC;
    tabStates[RX_POIDS] = &TStatesMachine::rxPoids;
    tabStates[RX_TARE] = &TStatesMachine::rxTare;
    tabStates[RX_CR] = &TStatesMachine::rxCr;
    tabStates[RX_CHECKSUM] = &TStatesMachine::rxChecksum;

    state = RX_STX;
    pointIndex = 0;
    brut = 0;
    unite = 0;
    checksum = 0;
    poidsIndex = 0;
    tareIndex = 0;

    screen->dispStr(1, 6, "Poids: ");
    screen->dispStr(1, 7, "Tare: ");
    screen->dispStr(1, 8, "Unité: ");
    screen->dispStr(1, 9, "Brut: ");
}

TStatesMachine::~TStatesMachine()
{
}

void TStatesMachine::run(unsigned char data)
{
    state = (this->*tabStates[state])(data);
}

TStatesMachine::states_t TStatesMachine::rxStx(unsigned char data)
{
    checksum = data;
    if (data == 0x02) // STX
    {
        poidsIndex = 0;
        tareIndex = 0;
        unite = 0;
        brut = 0;
        screen->dispStr(1, 10, "STX     ");
        return RX_MOT_A;
    }
    else
        return RX_STX;
}

TStatesMachine::states_t TStatesMachine::rxMotA(unsigned char data)
{
    checksum += data;
    pointIndex = 8 - (data & 0b00000111);
    screen->dispStr(1, 10, "MOT_A     ");
    return RX_MOT_B;
}

TStatesMachine::states_t TStatesMachine::rxMotB(unsigned char data)
{
    checksum += data;
    screen->dispStr(1, 10, "MOT_B    ");
    if (data & 0b00000110)
        return RX_STX;
    else
    {
        brut = data & 0b00000001;
        unite = (data & 0b00010000)?1:0;
        return RX_MOT_C;
    }
}

TStatesMachine::states_t TStatesMachine::rxMotC(unsigned char data)
{
    checksum += data;
    screen->dispStr(1, 10, "MOT_C    ");
    return RX_POIDS;
}

TStatesMachine::states_t TStatesMachine::rxPoids(unsigned char data)
{
    checksum += data;
    screen->dispStr(1, 10, "POIDS    ");
    if (poidsIndex == pointIndex)
        poids[poidsIndex++] = '.';
    poids[poidsIndex++] = (data == 0x20) ? '0' : data;
    if (poidsIndex == 7)
    {
        poids[poidsIndex] = '\0';
        return RX_TARE;
    }
    else
    {
        return RX_POIDS;
    }
}

TStatesMachine::states_t TStatesMachine::rxTare(unsigned char data)
{
    checksum += data;
    screen->dispStr(1, 10, "TARE     ");
    if (tareIndex == pointIndex)
        tare[tareIndex++] = '.';
    tare[tareIndex++] = (data == 0x20) ? '0' : data;
    if (tareIndex == 7)
    {
        tare[tareIndex] = '\0';
        return RX_CR;
    }
    else
        return RX_TARE;
}

TStatesMachine::states_t TStatesMachine::rxCr(unsigned char data)
{
    checksum += data;
    screen->dispStr(1, 10, "CR     ");
    if (data == 0x0D)
        return RX_CHECKSUM;
    else
        return RX_STX;
}

TStatesMachine::states_t TStatesMachine::rxChecksum(unsigned char data)
{
    checksum += data;
    checksum &= 0x7F;
    if (checksum == 0)
    {
        checksumOk = 1;
        screen->dispStr(1, 11, "Checksum OK   ");
        
        if (poids != partage->getSPoids())
        {
            screen->dispStr(8, 6, poids);
            partage->setSPoids(poids);
            partage->setChangeTable(0, true);
        }
        
        if (tare != partage->getSTare())
        {
            screen->dispStr(8, 7, tare);
            partage->setSTare(tare);
            partage->setChangeTable(1, true);
        }
        
        if (unite != partage->getUnite())
        {
            screen->dispStr(8, 8, unite ? "Kg":"Lb");
            partage->setUnite(unite);
            partage->setChangeTable(2, true);
        }
        
        if (brut != partage->getBrut())
        {
            screen->dispStr(8, 9, brut ? "Net":"Brut");
            partage->setBrut(brut);
            partage->setChangeTable(3, true);
        }
    }
    else
    {
        checksumOk = 0;
        screen->dispStr(1, 11, "Checksum error");
    }
    return RX_STX;
}