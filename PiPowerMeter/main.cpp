//============================================================================
// Name        : main.cpp
// Author      : SG (Modifier SDC)
// Version     : Labo 5
// Copyright   : Your copyright notice
// Description : Demonsatration de l'utilisation de MQTT
//============================================================================

#include "clavier.hpp"
#include "partage.hpp"
#include "screen.hpp"
#include "com.hpp"
#include "com1.hpp"
#include "task1.hpp"

int main(int argc, char *argv[])
{
  char car;
  std::string str = "                                        ";
  // Initialisation task Principal
  TThread::initTaskMain(SCHED_FIFO, 0);

  // Création Clavier et console
  TClavier *clavier = TClavier::getInstance();
  TScreen *screen = new TScreen();
  TPartage *partage = TPartage::getInstance();
  
  screen->start();

  // Création tâches
  TCom1 *com1 = new TCom1(TCom1::getCom1Config().c_str(), screen, 91, TCom::b9600, TCom::pNONE, TCom::dS8, 4000);
  TTask1 *task1 = new TTask1("Task1", screen, SCHED_FIFO, 90, 0);
  
  // Démarrage tâches
  com1->start();
  task1->start();

  // Traitement tâche principale
  screen->dispStr(1, 1, "PowerMeter (SDC  02/12/2024)");
  do
  {
    if (partage->getSendCmd())
    {
      partage->resetSendCmd();
      com1->sendTx(partage->getTrameCmd(), partage->getTrameCmdLen());
    }
    
    //car = 0;
    // Traitement
    if (clavier->kbhit())
    {
      car = clavier->getch();
      //str[0] = car;
    }
  } while (car != 23);

  // Destruction tâches
  if (com1)
    delete com1;
  if (task1)
    delete task1;
  

  // Destruction console
  if (screen)
    delete screen;

  return 0;
}
