#ifndef TTASKAUTO_HPP
#define TTASKAUTO_HPP

#include "thread.hpp"
#include "partage.hpp"
#include "screen.hpp"

class TTaskAuto : public TThread
{
public:
    enum etapRec_e
    {
        ETAPE_A = 0,
        ETAPE_B = 1,
        ETAPE_C = 2,
        FINI = 3
    }; 
private:
    TScreen *screen;
    TPartage *partage;
    TPartage::partageMel_t *partageMelangeur;
    float startPoids = 0.0;
    etapRec_e etapRec = ETAPE_A;
    bool recetteGo = false;

public:
    TTaskAuto(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu = -1);
    ~TTaskAuto();
    
    void task(void);
};
#endif // TASKAUTO_HPP
