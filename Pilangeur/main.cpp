//============================================================================
// Name        : main.cpp
// Author      : SG (Modifier SDC)
// Version     : Melangeur
// Copyright   : Your copyright notice
// Description : Projet du panneau RAM
//============================================================================

#include "clavier.hpp"
#include "partage.hpp"
#include "screen.hpp"
#include "task1.hpp"
#include "taskAuto.hpp"
#include "taskMqtt.hpp"
#include "com1.hpp"

int main(int argc, char *argv[])
{
  char car;
  std::string strTx;
  //  Initialisation task Principal
  TThread::initTaskMain(SCHED_FIFO, 0);

  // Création Clavier et console
  TClavier *clavier = TClavier::getInstance();

  TPartage *partage = TPartage::getInstance();
  TScreen *screen = new TScreen();
  screen->start();

  // Création tâches
  TCom1 *com1 = new TCom1(TCom1::getCom1Config().c_str(), screen, 95, TCom1::b9600, TCom1::pNONE, TCom1::dS8, 4000);
  TTask1 *task1 = new TTask1("Task1", screen, SCHED_FIFO, 90);
  TTaskAuto *taskAuto = new TTaskAuto("TaskAuto", screen, SCHED_FIFO, 70);
  TTaskMqtt *taskMqtt = new TTaskMqtt("TaskMqtt", screen, SCHED_FIFO, 50);

  // Démarrage tâches
  com1->start();
  com1->setSignalTimeout(1);
  task1->start();
  taskAuto->start();
  taskMqtt->start();

  // Traitement tâche principale
  screen->dispStr(1, 1, "Melangeur (SDC  04/12/2024)");
  partage->setSendTx("<ME>");
  do
  {
    if (partage->getPartageMel()->status.m_EchoGood && partage->getSendTx())
    {
      partage->getPartageMel()->status.m_EchoGood = false;
      strTx = partage->getOldestTx();
      com1->sendTx((void*)strTx.c_str(), strTx.length());
    }
    
    
    //  Traitement
    if (clavier->kbhit())
    {
      car = clavier->getch();
    }
  } while (car != 23);

  // Destruction tâches
  if (task1)
    delete task1;
  if (taskAuto)
    delete taskAuto;
  if (taskMqtt)
    delete taskMqtt;
  // Destruction console
  if (screen)
    delete screen;
  // Destruction Com1
  if (com1)
    delete com1;

  return 0;
}
