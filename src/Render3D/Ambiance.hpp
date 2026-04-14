#pragma once
#include <glm/glm.hpp>
#include "../Game/Utils.hpp"

enum class EtatAmbiance { CALME, ETRANGE, TENSION, CHAOS };

class AmbianceMarkov {
private:
    EtatAmbiance m_etat_actuel = EtatAmbiance::CALME;

public:
    void transition() 
    {
        double tirage;
        
        switch (m_etat_actuel) {
            case EtatAmbiance::CALME:
                // Loi Uniforme : 70% de rester calme, 30% de passer à Etrange
                tirage = tirageUniforme();
                m_etat_actuel = (tirage < 0.7) ? EtatAmbiance::CALME : EtatAmbiance::ETRANGE;
                break;

            case EtatAmbiance::ETRANGE:
                // Loi de Cauchy : Valeurs souvent proches de 0, mais valeurs extrêmes possibles
                tirage = tirageCauchy(0.0, 1.0);
                // Si la valeur est aberrante (> 2 ou < -2), on monte en tension
                m_etat_actuel = (tirage > 2.0 || tirage < -2.0) ? EtatAmbiance::TENSION : EtatAmbiance::ETRANGE;
                break;

            case EtatAmbiance::TENSION:
                // Loi de Laplace : Très pointue autour de 0, files épaisses
                tirage = tirageLaplace(0.0, 1.0);
                if (tirage > 1.5) m_etat_actuel = EtatAmbiance::CHAOS;     // Dégénération
                else if (tirage < -1.5) m_etat_actuel = EtatAmbiance::ETRANGE; // Apaisement
                // Sinon on reste en Tension
                break;

            case EtatAmbiance::CHAOS:
                // Loi de Poisson : On teste pour voir comment la crise se résout
                int k = tiragePoisson(2.0); // Lambda = 2
                if (k == 0) m_etat_actuel = EtatAmbiance::CALME; // Résolution instantanée !
                else if (k == 1) m_etat_actuel = EtatAmbiance::TENSION; // Redescente douce
                // Si k >= 2, on reste dans le Chaos
                break;
        }
    }

    // Renvoie la couleur de la lumière pour le shader
    glm::vec3 get_light_color() const 
    {
        switch (m_etat_actuel) {
            case EtatAmbiance::CALME:   return glm::vec3(0.7f, 0.8f, 1.0f); // Bleu doux
            case EtatAmbiance::ETRANGE: return glm::vec3(0.6f, 0.3f, 0.8f); // Violet
            case EtatAmbiance::TENSION: return glm::vec3(1.0f, 0.5f, 0.2f); // Orange sombre
            case EtatAmbiance::CHAOS:   return glm::vec3(1.0f, 0.1f, 0.1f); // Rouge vif
            default:                    return glm::vec3(1.0f, 1.0f, 1.0f);
        }
    }
};