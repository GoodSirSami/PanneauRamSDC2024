#ifndef RAM_HPP
#define RAM_HPP

#include "singletonRam.hpp"
#include "mutex.hpp"
#include "sem.hpp"
#include "filtreFenetreGlissante.hpp"

/*
 * 100% = 20ma
 * 50%  = 12ma
 * 0%   = 4ma
 */
#define CourantToPourcent(val) (double)(((val) - 4.0) * 6.25)
#define PourcentToCourant(val) (double)(((val) / 6.25) + 4.0)
#define AjustPourcent(val) (((val) < 0) ? 0 : (((val) > 100) ? 100 : (val)))

#if 0
/*
 * 100% = 7.5ma
 * 0%   = 4ma
 */
#define CourantToPourcentValveEF(val) (double)(((val) - 4.0) * 28.57)
#define PourcentToCourantValveEF(val) (double)(((val) / 28.57) + 4.0)
#endif

#if 0
/*
 * 100% = 17.5ma
 * 0%   = 1.65ma
 */
#define CourantToPourcentValveEC(val) (double)(((val) - 1.65) * 6.28)
#endif

#define CourantToTempGrosBassin(val) ((7.1151 * ((val) - 4.0)) - 16.0)
#define CourantToTempPetitBassin(val) ((7.1151 * ((val) - 4.0)) - 4.0)
#define AjustCourant(val) (((val) < 4.0) ? 4.0 : (((val) > 20.0) ? 20.0 : (val)))
#define TempToCourantPetitBassin(val) ((((val) + 4.0) / 7.1151) + 4.0)

class TCom1;

class TRam : public TSingletonRam<TRam>
{
	friend class TSingletonRam<TRam>;

public:
	struct ram_t
	{
		bool mode; // Manuel/Auto
		bool pompe;
		bool eauChaude;
		bool eauFroide;
		bool debordementGrosBassin;
		bool debordementPetitBassin;

		double valveGrosBassin;
		double valvePetitBassin;
		double valveEauChaude;
		double valveEauFroide;
		double niveauGrosBassin;
		double niveauPetitBassin;
		double temperatureGrosBassin;
		double temperaturePetitBassin;

		double consigneNiveauGrosBassin;
		double consigneNiveauPetitBassin;
		double consigneTemperaturePetitBassin;

		double valveManuelleGB;
		double valveManuellePB;
	};

	struct alarmesRam_t
	{
		bool debordementGrosBassin;
		bool debordementPetitBassin;
		bool niveauGrosBassin;
		bool niveauPetitBassin;
		bool ackDGB;
		bool ackDPB;
		bool ackNGB;
		bool ackNPB;
		double tempsGenNGB;
		double tempsGenNPB;
		double tempsRepNGB;
		double tempsRepNPB;
		double niveauANGB;
		double niveauANPB;
	};

	struct partageRam_t
	{
		ram_t status;
		alarmesRam_t alarmes;
		double rawDataAI[8];
		TMutex lock;
		TSemaphore synControl;
		TSemaphore synAlarme;
	};

private:
	partageRam_t partageRam;
	TScreen *screen;
	TCom1 *com_1;

	TFiltreFenetreGlissante filtreGrosBassin;
	TFiltreFenetreGlissante filtrePetitBassin;

	TRam(TScreen *s);
	~TRam();

	TRam(const TRam &) = delete;
	TRam &operator=(const TRam &) = delete;

public:
	// acquisition
	void acquisition(void);
	void format(void);

	void init(void);

	// Actionneur
	void setPompe(bool onOff);
	void setEauFroide(bool onOff);
	void setEauChaude(bool onOff);
	bool getPompe(void);
	bool getEauFroide(void);
	bool getEauChaude(void);

	void setValveGrosBassin(double ouverturePourcent);
	void setValvePetitBassin(double ouverturePourcent);
	void setValveEauChaude(double ouverturePourcent);
	void setValveEauFroide(double ouverturePourcent);
	double getValveGrosBassin(void);
	double getValvePetitBassin(void);
	double getValveEauChaude(void);
	double getValveEauFroide(void);

	// Capteur
	bool getDebordementGrosBassin(void);
	bool getDebordementPetitBassin(void);
	double getNiveauGrosBassin(void);
	double getNiveauPetitBassin(void);
	double getTemperatureGrosBassin(void);
	double getTemperaturePetitBassin(void);

	// Consigne
	void setMode(bool manAuto);
	bool getMode(void);
	void setConsigneNiveauGrosBassin(double niveauPourcent);
	void setConsigneNiveauPetitBassin(double niveauPourcent);
	void setConsigneTemperaturePetitBassin(double temperature);

	// Partage
	partageRam_t *getPartageRam(void);
	void setStatusRam(ram_t *status);
	void getStatusRam(ram_t *status);

	// Manuel
	void setManValveGB(double ouverturePourcent);
	void setManValvePB(double ouverturePourcent);
	double getManValveGB(void);
	double getManValvePB(void);
};

#endif // RAM_HPP
