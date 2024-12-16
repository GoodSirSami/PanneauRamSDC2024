#include "alarme.hpp"

TAlarme::TAlarme(const char *name, void *shared, int policy, int priority, int noCpu) : TThread(name, shared, policy, priority, noCpu)
{
    screen = (TScreen *)shared;

    ram = TRam::getInstance(screen);
    partageRam = ram->getPartageRam();

    screen->dispStr(1, 6, "Alarme :");
    alarmeNGB = NORMAL;
    alarmeNPB = NORMAL;
    tempsNGB = new TTemps();
    tempsNPB = new TTemps();
}

TAlarme::~TAlarme()
{
    delete tempsNGB;
    delete tempsNPB;
}

void TAlarme::task(void)
{
    // variable locale
    char strCar[2] = {'-', '\0'};

    // synchronisation démarrage tâche
    signalStart();

    while (1)
    {
        // attendre acquisition complété
        partageRam->synAlarme.take();

        // traitement

        if (strCar[0] == '|')
            strCar[0] = '-';
        else
            strCar[0] = '|';
        screen->dispStr(11, 6, strCar);

        switch (alarmeNGB)
        {
        case NORMAL:
            if (partageRam->status.niveauGrosBassin > partageRam->alarmes.niveauANGB)
            {
                alarmeNGB = LIMITE_HAUTE;
                tempsNGB->startMesure();
            }
            break;
        case LIMITE_HAUTE:
            tempsNGB->stopMesure();
            if (partageRam->status.niveauGrosBassin > partageRam->alarmes.niveauANGB)
            {
                if (tempsNGB->mesure_us()/1000000 >= partageRam->alarmes.tempsGenNGB)
                {
                    partageRam->alarmes.niveauGrosBassin = 1;
                    alarmeNGB = ALARME;
                }
            }
            else
            {
                partageRam->alarmes.niveauGrosBassin = 0;
                alarmeNGB = NORMAL;
            }
            break;
        case ALARME:
            if(partageRam->alarmes.ackNGB == 1)
            {
                partageRam->alarmes.ackNGB = 0;
                alarmeNGB = RECONNAISSANCE;
                tempsNGB->startMesure();
            }
            break;
        case RECONNAISSANCE:
            tempsNGB->stopMesure();
            if(partageRam->status.niveauGrosBassin < partageRam->alarmes.niveauANPB)
            {
                partageRam->alarmes.niveauGrosBassin = 0;
                alarmeNGB = NORMAL;
            }
            else if (tempsNGB->mesure_us()/1000000 >= partageRam->alarmes.tempsRepNGB)
            {
                partageRam->alarmes.niveauGrosBassin = 1;
                alarmeNGB = ALARME;
            }
            break;
        default:
            break;
        }

        switch (alarmeNPB)
        {
        case NORMAL:
            if (partageRam->status.niveauPetitBassin > partageRam->alarmes.niveauANPB)
            {
                alarmeNPB = LIMITE_HAUTE;
                tempsNPB->startMesure();
            }
            break;
        case LIMITE_HAUTE:
            tempsNPB->stopMesure();
            if (partageRam->status.niveauPetitBassin > partageRam->alarmes.niveauANPB)
            {
                if (tempsNPB->mesure_us()/1000000 >= partageRam->alarmes.tempsGenNPB)
                {
                    partageRam->alarmes.niveauPetitBassin = 1;
                    alarmeNPB = ALARME;
                }
            }
            else
            {
                partageRam->alarmes.niveauPetitBassin = 0;
                alarmeNPB = NORMAL;
            }
            break;
        case ALARME:
            if(partageRam->alarmes.ackNPB == 1)
            {
                partageRam->alarmes.ackNPB = 0;
                alarmeNPB = RECONNAISSANCE;
                tempsNPB->startMesure();
            }
            break;
        case RECONNAISSANCE:
            tempsNPB->stopMesure();
            if(partageRam->status.niveauPetitBassin < partageRam->alarmes.niveauANPB)
            {
                partageRam->alarmes.niveauPetitBassin = 0;
                alarmeNPB = NORMAL;
            }
            else if (tempsNPB->mesure_us()/1000000 >= partageRam->alarmes.tempsRepNPB)
            {
                partageRam->alarmes.niveauPetitBassin = 1;
                alarmeNPB = ALARME;
            }
            break;
        default:
            break;
        }
    }
}
