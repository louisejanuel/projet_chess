#include "ChaosUtils.hpp"
#include <cstdlib>
#include <cmath>

bool tirageBernoulli(double p)
{
    double u = (double)rand() / RAND_MAX; // génère nombre aléatoire uniforme entre 0 et 1
    return (u < p);
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

int tirageGeometrique(double p)
{
    double u = (double)rand() / RAND_MAX;
    if (u == 0.0) u = 0.00001; // éviter log(0)
    return (int)ceil(log(u) / log(1.0 - p)); // formule de la transformée inverse
}

double tirageExponentiel(double lambda) {
    double u = (double)rand() / RAND_MAX;
    if (u == 0.0) u = 0.0001;
    return -log(u) / lambda;
}