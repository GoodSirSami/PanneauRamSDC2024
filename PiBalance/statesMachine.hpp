#ifndef STATESMACHINE_HPP
#define STATESMACHINE_HPP

#include "screen.hpp"
#include "partage.hpp"
#include <string>

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
		RX_STX,
		RX_MOT_A,
		RX_MOT_B,
		RX_MOT_C,
		RX_POIDS,
		RX_TARE,
		RX_CR,
		RX_CHECKSUM,
		END_STATES
	};

	// membre de données
	TScreen *screen;
	TPartage *partage;
	states_t state;

	states_t (TStatesMachine::*tabStates[END_STATES])(uint8_t data);

	//std::string poids;
	//std::string tare;
	char poids[8] = {'0'};
	char tare[8] = {'0'};
	uint8_t pointIndex;
	uint8_t brut; // 0 = brut, 1 = net
	uint8_t unite; // 0 = lb, 1 = kg
	uint8_t checksum;
	uint8_t poidsIndex;
	uint8_t tareIndex;
	uint8_t checksumOk;

	// méthode
	states_t rxStx(uint8_t data);
	states_t rxMotA(uint8_t data);
	states_t rxMotB(uint8_t data);
	states_t rxMotC(uint8_t data);
	states_t rxPoids(uint8_t data);
	states_t rxTare(uint8_t data);
	states_t rxCr(uint8_t data);
	states_t rxChecksum(uint8_t data);
};

#endif // STATESMACHINE_HPP