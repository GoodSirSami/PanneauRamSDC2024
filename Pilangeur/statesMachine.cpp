#include "statesMachine.hpp"

TStatesMachine::TStatesMachine(TScreen *pScreen)
{
    screen = (TScreen*)pScreen;
    state = RX_DEBUT;
    partage = TPartage::getInstance();
    partageMelangeur = partage->getPartageMel();
    
    tabStates[RX_DEBUT] = &TStatesMachine::rxDebut;
    tabStates[RX_M] = &TStatesMachine::rxM;
    tabStates[RX_CMD] = &TStatesMachine::rxCmd;
    tabStates[RX_DATA] = &TStatesMachine::rxData;
    tabStates[RX_FIN] = &TStatesMachine::rxFin;
}

TStatesMachine::~TStatesMachine()
{
}

void TStatesMachine::run(unsigned char data)
{
    state = (this->*tabStates[state])(data);
}


states_t TStatesMachine::rxDebut(uint8_t data)
{
    commande = ' ';
    etatsIndex = 0;
    echo = false;
    if (data == '<')
    {
        return RX_M;
    }
    return RX_DEBUT;
}

states_t TStatesMachine::rxM(uint8_t data)
{
    if (data == 'M')
    {
        return RX_CMD;
    }
    return RX_DEBUT;
}

states_t TStatesMachine::rxCmd(uint8_t data)
{
    if (data == 'M'  || data == 'A' || data == 'E')
    {
        commande = data;
        return RX_DATA;
    }
    return RX_DEBUT;
}

states_t TStatesMachine::rxData(uint8_t data)
{
    if (commande == 'E')
    {
        if (data == '>')
        {
            return rxFin('F');
        }
        else
        {
            etats[etatsIndex++] = data;
            return RX_DATA;
        }
    }
    else if (data == 'A')
    {
        moteur = 1;
        return RX_FIN;
    }
    else if (data == 'B')
    {
        moteur = 2;
        return RX_FIN;
    }
    else if (data == 'C')
    {
        moteur = 3;
        return RX_FIN;
    }
    
    return RX_DEBUT;
}

states_t TStatesMachine::rxFin(uint8_t data)
{
    if (data == '>' || data == 'F')
    {
        partageMelangeur->status.m_EchoGood = true;
        if (commande == 'A')
        {
            screen->dispStr(80, 1, "Moteur on");
            partageMelangeur->status.m_Moteurs[moteur - 1] = 0;
            partageMelangeur->changeTable[moteur - 1] = true;
        }
        else if (commande == 'M')
        {
            screen->dispStr(80, 1, "Moteur off");
            partageMelangeur->status.m_Moteurs[moteur - 1] = 1;
            partageMelangeur->changeTable[moteur - 1] = true;
        }
        else if (commande == 'E')
        {
            for (uint8_t i = 0; i < etatsIndex; i++)
            {
                if (etats[i] == 'A')
                {
                    if (partageMelangeur->status.m_Moteurs[i] == 1)
                        partage->setChangeTable(i, true);
                    partageMelangeur->status.m_Moteurs[i] = 0;
                }
                else if (etats[i] == 'M')
                {
                    if (partageMelangeur->status.m_Moteurs[i] == 0)
                        partage->setChangeTable(i, true);
                    partageMelangeur->status.m_Moteurs[i] = 1;
                }
            }
            
        }
    }
    return RX_DEBUT;
}




