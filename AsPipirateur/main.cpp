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
#include "task1.hpp"
#include "taskCom.hpp"

int main(int argc, char *argv[])
{
  char car;
  // std::string str = "                                        ";
  
  // Initialisation task Principal
  TThread::initTaskMain(SCHED_FIFO, 0);

  // Création Clavier et console
  TClavier *clavier = TClavier::getInstance();
  TScreen *screen = new TScreen();
  screen->start();

  // Création tâches
  TTaskCom *taskCom = new TTaskCom("TaskCom", screen, SCHED_FIFO, 92);
  TTask1 *task1 = new TTask1("Task1", screen, SCHED_FIFO, 90);

  // Démarrage tâches
  task1->start();
  taskCom->start();

  // Traitement tâche principale
  screen->dispStr(1, 1, "Aspirateur (SDC  04/12/2024)");

  do
  {
    // car = 0;
    //  Traitement
    if (clavier->kbhit())
    {
      car = clavier->getch();
        
    }
  } while (car != 23);

  // Destruction tâches
  if (task1)
    delete task1;
  if (taskCom)
    delete taskCom;
  // Destruction console
  if (screen)
    delete screen;

  return 0;
}
