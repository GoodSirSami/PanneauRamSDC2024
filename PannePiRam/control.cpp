#include "control.hpp"
#include <math.h>
#include <vector>
#include <cmath>

TControl::TControl(const char *name, void *shared, int policy, int priority, int noCpu) : TThread(name, shared, policy, priority, noCpu)
{
    screen = (TScreen *)shared;

    ram = TRam::getInstance(screen);
    partageRam = ram->getPartageRam();

    screen->dispStr(1, 5, "Control :");
}

TControl::~TControl()
{
}

void TControl::task(void)
{
    // variable locale
    char strCar[2] = {'-', '\0'};
    //char strMessage[50];

    // synchronisation démarrage tâche
    signalStart();

    while (1)
    {
        // attendre acquisition complété
        partageRam->synControl.take();

        // traitement

        if (strCar[0] == '|')
            strCar[0] = '-';
        else
            strCar[0] = '|';
        screen->dispStr(11, 5, strCar);

        if (ram->getMode() == 0) // Auto
        {
            consigneGB = partageRam->status.consigneNiveauGrosBassin;
            consignePB = partageRam->status.consigneNiveauPetitBassin;
            consTemp = partageRam->status.consigneTemperaturePetitBassin;

            // PID pour le gros bassin
            if (consigneGB > 0)
            {
                pidGrosBassin.setSetpoint(consigneGB);

                double valvePurge = -0.0941 * ram->getManValveGB() + 2.6052;
                if (valvePurge > 0)
                    valvePurge = 0;
                double constanteGB = (-valvePurge + 3.358) / 0.1329;
                
                double valveGB = pidGrosBassin.compute(ram->getNiveauGrosBassin()) + constanteGB;
                if (valveGB > 100)
                    valveGB = 100;
                if (valveGB < 0)
                    valveGB = 0;
                
                ram->setValveGrosBassin(100 - valveGB);
            }
            else
            {
                ram->setValveGrosBassin(100);
            }

            // PID pour le petit bassin
            if (consignePB > 0)
            {
                pidPetitBassin.setSetpoint(consignePB);

                double valvePurge = -0.0012 * pow(ram->getManValvePB(), 2) - 0.032 * ram->getManValvePB() + 0.839;
                double valveEauChaude = 0.001 * pow(ram->getValveEauChaude(), 2) + 0.0087 * ram->getValveEauFroide();
                double valveEauFroide = 0.0352 * ram->getValveEauFroide();
                if (valvePurge > 0)
                    valvePurge = 0;
                double constantePB = (0.0647 + sqrt(0.00416609  - 0.0092 * (0.7531 + valvePurge))) / 0.0046 - valveEauChaude - valveEauFroide;

                double valvePB = pidPetitBassin.compute(ram->getNiveauPetitBassin()) + constantePB;
                if (valvePB > 100)
                    valvePB = 100;
                if (valvePB < 0)
                    valvePB = 0;

                ram->setValvePetitBassin(valvePB);
            }
            else
            {
                ram->setValvePetitBassin(0);
            }

            pidTempPB.setSetpoint(consTemp);

            double valvesTemp = pidTempPB.compute(ram->getTemperaturePetitBassin());
            if (valvesTemp > 100)
                valvesTemp = 100;  
            if (valvesTemp < 0)
                valvesTemp = 0;
            
            ram->setValveEauChaude(valvesTemp);
            ram->setValveEauFroide(valvesTemp);
        }
    }
}
