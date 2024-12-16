#include "filtreFenetreGlissante.hpp"
#include <string.h>

TFiltreFenetreGlissante::TFiltreFenetreGlissante(int fenetre)
    : tailleFenetre(fenetre), currentSum(0)
{
}

TFiltreFenetreGlissante::~TFiltreFenetreGlissante()
{
}

double TFiltreFenetreGlissante::filtre(double valAcquisition)
{
    fenetre.push(valAcquisition);
    currentSum += valAcquisition;

    if ((int)fenetre.size() > tailleFenetre)
    {
        currentSum -= fenetre.front();
        fenetre.pop();
    }

    return currentSum / fenetre.size();
}
