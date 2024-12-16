#include "taskMqtt.hpp"
#include "screen.hpp"
#include "temps.hpp"
#include "alarme.hpp"

TTaskMqtt *tMqtt = NULL;

TTaskMqtt::TTaskMqtt(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu) : TThread(name, shared, policy, priority, cpu)

{
    string nameMqtt;

    screen = (TScreen *)shared;

    ram = TRam::getInstance(screen);
    partageRam = ram->getPartageRam();

    TMqtt::getName(nameMqtt);
    mqtt = new TMqtt(screen, "ramMqtt", (char *)nameMqtt.c_str());

    screen->dispStr(1, 7, "MQTT : ");
    screen->dispStr(10, 7, nameMqtt.c_str());
}

TTaskMqtt::~TTaskMqtt()
{
    if (mqtt)
        delete mqtt;
}

void TTaskMqtt::task(void)
{
    // variable locale
    char strCar[2] = {'-', '\0'};
    std::string strMessage;   

    // synchronisation démarrage tâche
    signalStart();

    sleep(1);

    while (1)
    {
        // traitement

        if (strCar[0] == '|')
            strCar[0] = '-';
        else
            strCar[0] = '|';
        screen->dispStr(8, 7, strCar);

        if (pompeLastValue != ram->getPompe()) 
        {
            pompeLastValue = ram->getPompe();
            strMessage = std::to_string(ram->getPompe());
            mqtt->publish(NULL, "ram/panneau/etats/Pompe", strMessage.length(), strMessage.c_str());
        }
        
        if (eauChaudeLastValue != ram->getEauChaude()) 
        {
            eauChaudeLastValue = ram->getEauChaude();
            strMessage = std::to_string(ram->getEauChaude());
            mqtt->publish(NULL, "ram/panneau/etats/ValveEEC", strMessage.length(), strMessage.c_str());
        }

        if (eauFroideLastValue != ram->getEauFroide()) 
        {
            eauFroideLastValue = ram->getEauFroide();
            strMessage = std::to_string(ram->getEauFroide());
            mqtt->publish(NULL, "ram/panneau/etats/ValveEEF", strMessage.length(), strMessage.c_str());
        }

        if(ram->getValveEauChaude() > valveEauChaudeLastValue + 0.5 || ram->getValveEauChaude() < valveEauChaudeLastValue - 0.5)
        {
            valveEauChaudeLastValue = ram->getValveEauChaude();
            strMessage = std::to_string(ram->getValveEauChaude());
            mqtt->publish(NULL, "ram/panneau/etats/ValveEC", strMessage.length(), strMessage.c_str());
        }

        if(ram->getValveEauFroide() > valveEauFroideLastValue + 0.5 || ram->getValveEauFroide() < valveEauFroideLastValue - 0.5)
        {
            valveEauFroideLastValue = ram->getValveEauFroide();
            strMessage = std::to_string(ram->getValveEauFroide());
            mqtt->publish(NULL, "ram/panneau/etats/ValveEF", strMessage.length(), strMessage.c_str());
        }

        if(ram->getValveGrosBassin() > valveGrosBassinLastValue + 0.5 || ram->getValveGrosBassin() < valveGrosBassinLastValue - 0.5)
        {
            valveGrosBassinLastValue = ram->getValveGrosBassin();
            strMessage = std::to_string(ram->getValveGrosBassin());
            mqtt->publish(NULL, "ram/panneau/etats/ValveGB", strMessage.length(), strMessage.c_str());
        }

        if(ram->getValvePetitBassin() > valvePetitBassinLastValue + 0.5 || ram->getValvePetitBassin() < valvePetitBassinLastValue - 0.5)
        {
            valvePetitBassinLastValue = ram->getValvePetitBassin();
            strMessage = std::to_string(ram->getValvePetitBassin());
            mqtt->publish(NULL, "ram/panneau/etats/ValvePB", strMessage.length(), strMessage.c_str());
        }

        if(ram->getNiveauGrosBassin() > nivGBLastValue + 0.5 || ram->getNiveauGrosBassin() < nivGBLastValue - 0.5)
        {
            nivGBLastValue = ram->getNiveauGrosBassin();
            strMessage = std::to_string(ram->getNiveauGrosBassin());
            mqtt->publish(NULL, "ram/panneau/etats/NivGB", strMessage.length(), strMessage.c_str());
        }

        if(ram->getNiveauPetitBassin() > nivPBLastValue + 0.5 || ram->getNiveauPetitBassin() < nivPBLastValue - 0.5)
        {
            nivPBLastValue = ram->getNiveauPetitBassin();
            strMessage = std::to_string(ram->getNiveauPetitBassin());
            mqtt->publish(NULL, "ram/panneau/etats/NivPB", strMessage.length(), strMessage.c_str());
        }

        if(ram->getTemperaturePetitBassin() > tempPBLastValue + 0.3 || ram->getTemperaturePetitBassin() < tempPBLastValue - 0.3)
        {
            tempPBLastValue = ram->getTemperaturePetitBassin();
            strMessage = std::to_string(ram->getTemperaturePetitBassin());
            mqtt->publish(NULL, "ram/panneau/etats/TmpPB", strMessage.length(), strMessage.c_str());
        }

        if (partageRam->alarmes.debordementGrosBassin && partageRam->alarmes.ackDGB == 0)
        {
            partageRam->alarmes.debordementGrosBassin = 0;
            mqtt->publish(NULL, "ram/alarmes/etats/ALR_GB_OVF", 2, "ON", 0, true);
        }

        if (partageRam->alarmes.debordementPetitBassin && partageRam->alarmes.ackDPB == 0)
        {
            partageRam->alarmes.debordementPetitBassin = 0;
            mqtt->publish(NULL, "ram/alarmes/etats/ALR_PB_OVF", 2, "ON", 0, true);
        }
        
        if (partageRam->alarmes.niveauGrosBassin)
        {
            partageRam->alarmes.niveauGrosBassin = 0;
            mqtt->publish(NULL, "ram/alarmes/etats/ALR_GB_NIV_MAX", 2, "ON", 0, true);
        }

        if (partageRam->alarmes.niveauPetitBassin)
        {
            partageRam->alarmes.niveauPetitBassin = 0;
            mqtt->publish(NULL, "ram/alarmes/etats/ALR_PB_NIV_MAX", 2, "ON", 0, true);
        }
        

        if (mqtt->loop(200) != 0)
            mqtt->reconnect();
        
        usleep(200000); // 200 ms
    }
}
