#ifndef STATESMACHINE_HPP
#define STATESMACHINE_HPP

#include "screen.hpp"
#include "partage.hpp"
#include <string>
#include <string.h>

enum states_t
{
	RX_DEBUT,
	RX_M,
	RX_CMD,
	RX_DATA,
	RX_FIN,
	END_STATES
};

class TStatesMachine
{
public:
	// méthode
	TStatesMachine(TScreen *pScreen);
	virtual ~TStatesMachine();

	void run(unsigned char data);

private:
	// constante

	// membre de données
	TScreen *screen;
	states_t state;
	TPartage *partage;
	TPartage::partageMel_t *partageMelangeur;

	states_t (TStatesMachine::*tabStates[END_STATES])(uint8_t data);

	// Données
	unsigned char commande;
	unsigned char etats[3] = {0};
	uint8_t etatsIndex = 0;
	bool echo = false;
	bool valMoteur = false;
	int moteur = 0;

	// Méthodes
	states_t rxDebut(uint8_t data);
	states_t rxM(uint8_t data);
	states_t rxCmd(uint8_t data);
	states_t rxData(uint8_t data);
	states_t rxFin(uint8_t data);
};

#endif // STATESMACHINE_HPP