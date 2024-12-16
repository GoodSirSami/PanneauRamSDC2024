#ifndef STATESMACHINE_HPP
#define STATESMACHINE_HPP

#include "screen.hpp"
#include "partage.hpp"
#include "Elkor.h"

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
		RXID,
		RXFC,
		RXDATA0,
		RXDATAX,
		RXCRCH,
		RXCRCL,
		END_STATES
	};

	

	// membre de données
	TScreen *screen;
	TPartage *partage;
	states_t state;
	uint8_t trameIndex;
	uint8_t trame[256];
	uint8_t nb_Bytes = 0;



	states_t (TStatesMachine::*tabStates[END_STATES])(uint8_t data);
	states_t stateRxId(uint8_t data);
	states_t stateRxFC(uint8_t data);
	states_t stateRxData0(uint8_t data);
	states_t stateRxDataX(uint8_t data);
	states_t stateRxCrcH(uint8_t data);
	states_t stateRxCrcL(uint8_t data);

	// méthode
	
};

#endif // STATESMACHINE_HPP