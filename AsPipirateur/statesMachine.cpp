#include "statesMachine.hpp"

TStatesMachine::TStatesMachine(TScreen *pScreen)
{
    screen = pScreen;
    partage = TPartage::getInstance();

    tabStates[RX_DEBUT] = &TStatesMachine::rxDebut;
    tabStates[RX_DATA] = &TStatesMachine::rxData;
    tabStates[RX_FIN] = &TStatesMachine::rxFin;
    state = RX_DEBUT;
    commandePRecue = false;
}

TStatesMachine::~TStatesMachine()
{
}

void TStatesMachine::run(unsigned char data)
{
    state = (this->*tabStates[state])(data);
}

TStatesMachine::states_t TStatesMachine::rxDebut(uint8_t data)
{   
    cntData = 0;
    commandePRecue = false;
    commande = 0;
    memset(bufferData, 0, 100);
    if (data == '<')
        return RX_DATA;
    else
        return RX_DEBUT;
    
}

TStatesMachine::states_t TStatesMachine::rxData(uint8_t data)
{
    states_t retour = RX_DEBUT;
    if (commandePRecue)
    {
        if (data == '>')
        {
            rxFin('V');
            retour = RX_DEBUT;
        }
        else
        {
            bufferData[cntData++] = data;
            retour = RX_DATA;
        }
    }
    else
    {
        switch (data)
        {
        case 'M':
        case 'A':
        case 'D':
            commande = data;
            retour = RX_FIN;
            break;
        case 'P':
            commande = data;
            commandePRecue = true;
            retour = RX_DATA;
            break;
        default:
            break;
        }
    }
    return retour;
}

TStatesMachine::states_t TStatesMachine::rxFin(uint8_t data)
{
    partage->echoRecu = true;
    std::string buffer(bufferData);
    size_t firstDot;
    size_t secondDot;

    if (data == 'V' || data == '>')
    {
        switch (commande)
        {
        case 'M':
            partage->setMRecu(1);
            break;
        case 'A':
            partage->setARecu(1);
            break;
        case 'D':
            partage->setDRecu(1);
            partage->addFileTx("<D>");
            break;
        case 'P':
            firstDot = buffer.find('.');
            secondDot = buffer.find('.', firstDot + 1);
            try
            {
                bufferInt[0] = std::stoi(buffer.substr(0, firstDot));
                bufferInt[1] = std::stoi(buffer.substr(firstDot + 1, secondDot - firstDot - 1));
                bufferInt[2] = std::stoi(buffer.substr(secondDot + 1));
                partage->setFilling(bufferInt);
                partage->setPRecu(1);
            }
            catch (...)
            {
                screen->dispStr(1, 15, "Erreur de conversion");
            }
            break;
        default:
            break;
        }
    }
    return RX_DEBUT;
}