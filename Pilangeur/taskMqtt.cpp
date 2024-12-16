#include "taskMqtt.hpp"
#include "screen.hpp"
#include "temps.hpp"

TTaskMqtt *tMqtt = NULL;

TTaskMqtt::TTaskMqtt(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu) : TThread(name, shared, policy, priority, cpu)
{
    std::string nameMqtt = TMqtt::getMqttConfig();

    screen = (TScreen *)shared;
    partage = TPartage::getInstance();
    mqtt = new TMqtt(screen, "melangeurMqtt", (char *)nameMqtt.c_str());

    screen->dispStr(1, 4, "MQTT : ");
    screen->dispStr(10, 4, nameMqtt.c_str());
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
    TPartage::recetteGo_e lastRecetteStatus = TPartage::RECETTE_STOP;
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
        screen->dispStr(8, 4, strCar);

        if (partage->getChangeTable(0))
        {
            partage->setChangeTable(0, false);
            strMessage = (partage->getMoteurA()) ? "on" : "off";
            mqtt->publish(NULL, "ram/melangeur/etats/motA", strMessage.length(), strMessage.c_str(), 0 , false);
        }

        if (partage->getChangeTable(1))
        {
            partage->setChangeTable(1, false);
            strMessage = (partage->getMoteurB()) ? "on" : "off";
            mqtt->publish(NULL, "ram/melangeur/etats/motB", strMessage.length(), strMessage.c_str(), 0 , false);
        }

        if (partage->getChangeTable(2))
        {
            partage->setChangeTable(2, false);
            strMessage = (partage->getMoteurC()) ? "on" : "off";
            mqtt->publish(NULL, "ram/melangeur/etats/motC", strMessage.length(), strMessage.c_str(), 0 , false);
        }
        

        if(lastRecetteStatus != partage->getRecetteGo())
        {
            lastRecetteStatus = partage->getRecetteGo();
            switch (lastRecetteStatus)
            {
            case TPartage::RECETTE_GO:
                strMessage = "PROCESSING";
                break;
            default:
                strMessage = "FINISHED";
                break;
            }
            mqtt->publish(NULL, "ram/melangeur/etats/recetteStatut", strMessage.length(), strMessage.c_str(), 0 , false);
        }
        
        if (mqtt->loop(200) != 0)
            mqtt->reconnect();
        
        usleep(200000); // 200 ms
    }
}
