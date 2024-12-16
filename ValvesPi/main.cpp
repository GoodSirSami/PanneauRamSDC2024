//============================================================================
// Name        : TestValvePurge
// Author      : SG
// Version     :
// Copyright   : Your copyright notice
// Description : Test
//============================================================================

#include <stdio.h>
#include "version.hpp"
#include "clavier.hpp"
#include "screen.hpp"
#include "temps.hpp"
#include "task1.hpp"
#include "task2.hpp"

int main(int argc, char *argv[])
{
  char car, bufStr[50];
  int32_t noCpu;

  // Initialisation task Principal
  noCpu = 0;
  TThread::initTaskMain(SCHED_FIFO, noCpu);

  // Création Temps;
  TTemps *temps = new TTemps();
  // Création Clavier et console
  TClavier *clavier = TClavier::getInstance();
  TScreen *screen = new TScreen();
  screen->start();

  // Création tâches
  TTask1 *task1 = new TTask1("Task 1", screen, SCHED_FIFO, 95, noCpu);
  noCpu = 1;
  TTask2 *task2 = new TTask2("Task 2", screen, SCHED_FIFO, 94, noCpu);

  // Démarrage tâches

  task1->start();
  task2->start();

  // Traitement tâche principale
  screen->dispStr(1, 1, "Test Valve Purge(");
  screen->dispStr(18, 1, AUTEUR);
  screen->dispStr(21, 1, VERSION);
  screen->dispStr(27, 1, DATE);
  screen->dispStr(37, 1, ")");
  sprintf(bufStr, "Nombre de CPU : %ld disponible(s)", sysconf(_SC_NPROCESSORS_ONLN));
  screen->dispStr(1, 2, bufStr);

  screen->dispStr(1, 4, "Task main :");
  screen->dispStr(1, 14, "Alim Cir1 GB (1) : ");
  screen->dispStr(1, 15, "Alim Cir2 PB (2) : ");

  do
  {
    // Traitement
    screen->dispStr(13, 4, temps->now());

    if (clavier->kbhit())
    {
      car = clavier->getch();
      if (car == '1')
        task1->toggleAlimCir1();
      else if (car == '2')
        task1->toggleAlimCir2();
    }

    usleep(200000);
  } while ((car != 'q') && (car != 'Q'));

  // Destruction tâches
  if (task1)
    delete task1;
  if (task2)
    delete task2;

  // Destruction Temps
  if (temps)
    delete temps;
  // Destruction Clavier et console
  if (screen)
    delete screen;

  return 0;
}
