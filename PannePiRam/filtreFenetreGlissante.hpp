#ifndef FILTRE_FENETRE_GLISSANTE_HPP
#define FILTRE_FENETRE_GLISSANTE_HPP

#include <queue>

class TFiltreFenetreGlissante
{
private:
    std::queue<double> fenetre;
    int tailleFenetre;
    double currentSum;

public:
    TFiltreFenetreGlissante(int fenetre = 11);
    ~TFiltreFenetreGlissante();

    double filtre(double valAcquisition);
};

#endif // FILTRE_FENETRE_GLISSANTE_HPP
