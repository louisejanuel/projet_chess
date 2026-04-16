#include "Utils.hpp"
#include <numbers>
#include <cmath>

// pour chaine de markov

double tirageUniforme() {
    return (double)rand() / RAND_MAX;
}

double tirageCauchy(double x0, double gamma) {
    double u = tirageUniforme();
    return x0 + gamma * tan(std::numbers::pi * (u - 0.5));
}

double tirageLaplace(double mu, double b) {
    double u = tirageUniforme();
    // formule d'inversion
    if (u < 0.5) {
        return mu + b * log(2.0 * u);
    } else {
        return mu - b * log(2.0 - 2.0 * u);
    }
}

int tiragePoisson(double lambda) {
    int k = 0;
    double p = 1.0;
    double L = exp(-lambda);
    do {
        k++;
        p *= tirageUniforme();
    } while (p > L);
    return k - 1;
}