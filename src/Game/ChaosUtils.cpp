#include "ChaosUtils.hpp"
#include <cstdlib> // Pour rand() et RAND_MAX

bool tirageBernoulli(double p)
{
    // Génère un nombre aléatoire uniforme entre 0.0 et 1.0
    double u = (double)rand() / RAND_MAX;
    return (u < p); // Retourne true avec une probabilité p
}

int tirageBinomial(int n, double p)
{
    int succes = 0;
    for (int i = 0; i < n; ++i)
    {
        double u = (double)rand() / RAND_MAX;
        if (u < p)
            succes++;
    }
    return succes;
}