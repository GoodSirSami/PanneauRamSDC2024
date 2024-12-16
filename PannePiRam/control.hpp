#ifndef CONTROL_HPP
#define CONTROL_HPP

#include "thread.hpp"
#include "screen.hpp"
#include "ram.hpp"
#include "filtreFenetreGlissante.hpp"
#include "pid.hpp"
#include <queue>

class TControl : public TThread
{
private:
	TScreen *screen;
	TRam *ram;
	TRam::partageRam_t *partageRam;
	TFiltreFenetreGlissante filtreGrosBassin;
	TFiltreFenetreGlissante filtrePetitBassin;
	TPid pidGrosBassin = TPid(4.5, 0.2, 0);
	TPid pidPetitBassin = TPid(3, 0.125, 0);
	TPid pidTempPB = TPid(10, 0.5, 20);
	double consTemp = 0;
	double consigneGB = 0;
	double consignePB = 0;

public:
	TControl(const char *name, void *shared, int policy, int priority, int noCpu = -1);
	~TControl();

	// tâche control
	void task(void);
};

#endif // CONTROL_HPP
