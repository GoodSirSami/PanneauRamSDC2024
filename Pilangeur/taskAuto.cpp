#include "taskAuto.hpp"

TTaskAuto::TTaskAuto(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu) : TThread(name, shared, policy, priority, cpu)
{
    screen = (TScreen *)shared;
    partage = TPartage::getInstance();
    partageMelangeur = partage->getPartageMel();
}

TTaskAuto::~TTaskAuto()
{

}

void TTaskAuto::task(void)
{
    // variable locale
    char strCar[2] = {'-', '\0'};
    screen->dispStr(1, 6, "TaskAuto : ");
    bool flagMotA = false;
    bool flagMotB = false;
    bool flagMotC = false;
    
    
    // synchronisation démarrage tâche
    signalStart();
    sleep(2);

    while (1)
    {
        partageMelangeur->semAuto.take();

        // traitement
		if (strCar[0] == '|')
			strCar[0] = '-';
		else
			strCar[0] = '|';
		screen->dispStr(12, 6, strCar);

        if (partage->getChangeTable(3))
        {
            partage->setChangeTable(3, false);
            switch (partage->getRecetteGo())        
            {
            case TPartage::RECETTE_GO:
                recetteGo = true;
                flagMotA = false;
                flagMotB = false;
                flagMotC = false;
                etapRec = ETAPE_A;
                startPoids = partage->getPoids();
                break;
            case TPartage::RECETTE_STOP:
                recetteGo = false;
                partage->setSendTx("<MAA>");
                partage->setSendTx("<MAB>");
                partage->setSendTx("<MAC>");
                break;
            case TPartage::RECETTE_FLUSH:
                partage->removeRecette();
                partage->setRecetteGo(TPartage::RECETTE_STOP);
                etapRec = FINI;
                break;
            default:
                break;
            }
        }
        if (recetteGo && partageMelangeur->status.m_nBRecette > 0 && partageMelangeur->status.s_ShopVacFinished)
        {
            screen->dispStr(50,9, "Recette en cours");
            screen->dispStr(50,10, (etapRec == ETAPE_A) ? "res A" : (etapRec == ETAPE_B) ? "res B" : "res C");
            screen->dispStr(50,11, "Poids : " + std::to_string(partage->getPoids() - startPoids));
            switch (etapRec)
            {
            case ETAPE_A:
                if ((partage->getPoids() - startPoids) < (float)partage->getRecette(0).qtyA/1000.0F)
                {
                    if (!flagMotA)
                    {
                        flagMotA = true;
                        partage->setSendTx("<MMA>");
                        partage->setSendTx("<MAB>");
                        partage->setSendTx("<MAC>");
                    }
                }
                else
                {
                    startPoids = partage->getPoids();
                    flagMotA = false;
                    etapRec = ETAPE_B;
                }
                break;
            case ETAPE_B:
                if ((partage->getPoids() - startPoids) < (float)partage->getRecette(0).qtyB/1000.0F)
                {
                    if (!flagMotB)
                    {
                        flagMotB = true;
                        partage->setSendTx("<MAA>");
                        partage->setSendTx("<MMB>");
                        partage->setSendTx("<MAC>");
                    }
                }
                else
                {
                    startPoids = partage->getPoids();
                    flagMotB = false;
                    etapRec = ETAPE_C;
                }
                break;
            case ETAPE_C:
                if ((partage->getPoids() - startPoids) < (float)partage->getRecette(0).qtyC/1000.0F)
                {
                    if (!flagMotC)
                    {
                        flagMotC = true;
                        partage->setSendTx("<MAA>");   
                        partage->setSendTx("<MAB>");
                        partage->setSendTx("<MMC>");

                    }
                }
                else
                {
                    recetteGo = false;
                    flagMotC = false;
                    partage->setSendTx("<MAC>");
                    partage->setRecetteGo(TPartage::RECETTE_STOP);
                    etapRec = FINI;
                }
                break;
            default:

                break;
            }
        }
    }
}