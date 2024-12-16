#ifndef STATESMACHINE_HPP
#define STATESMACHINE_HPP

#include "screen.hpp"
#include "partage.hpp"
#include <string>
#include <string.h>

class TStatesMachine
{
public:
	// méthode
	TStatesMachine(TScreen *pScreen);
	virtual ~TStatesMachine();

	void run(unsigned char data);

private:
	// constante
	enum states_t
	{
		RX_DEBUT,
		RX_DATA,
		RX_FIN,
		END_STATES
	};

	// membre de données
	TScreen *screen;
	TPartage *partage;
	states_t state;

	states_t (TStatesMachine::*tabStates[END_STATES])(uint8_t data);

	// Données
	bool commandePRecue;
	char commande;
	uint8_t cntData;
	char bufferData[100];
	int bufferInt[3];

	// Méthodes
	states_t rxDebut(uint8_t data);
	states_t rxData(uint8_t data);
	states_t rxFin(uint8_t data);
};

#endif // STATESMACHINE_HPP