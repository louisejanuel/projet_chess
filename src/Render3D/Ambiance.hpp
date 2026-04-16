#pragma once
#include <glm/glm.hpp>
#include "../Game/Utils.hpp"

enum class EtatAmbiance {
    CALME,
    ETRANGE,
    TENSION,
    CHAOS
};

class AmbianceMarkov {
private:
    EtatAmbiance m_etat_actuel = EtatAmbiance::CALME;

public:
    void transition()
    {
        double tirage;

        switch (m_etat_actuel)
        {
        case EtatAmbiance::CALME:
            // Loi Uniforme : 70% de rester calme, 30% d'étrangeté
            tirage        = tirageUniforme();
            m_etat_actuel = (tirage < 0.7) ? EtatAmbiance::CALME : EtatAmbiance::ETRANGE;
            break;

        case EtatAmbiance::ETRANGE:
            // Loi de Cauchy : Transition en tension sur des valeurs extrêmes aberrantes
            tirage        = tirageCauchy(0.0, 1.0);
            m_etat_actuel = (tirage > 2.0 || tirage < -2.0) ? EtatAmbiance::TENSION : EtatAmbiance::ETRANGE;
            break;

        case EtatAmbiance::TENSION:
            // Loi de Laplace : Bascule vers le chaos ou retour au calme relatif
            tirage = tirageLaplace(0.0, 1.0);
            if (tirage > 1.5)
                m_etat_actuel = EtatAmbiance::CHAOS;
            else if (tirage < -1.5)
                m_etat_actuel = EtatAmbiance::ETRANGE;
            break;

        case EtatAmbiance::CHAOS:
        {
            // Loi de Poisson : Résolution de la crise selon la rareté de l'événement (lambda = 2)
            int k = tiragePoisson(2.0);
            if (k == 0)
                m_etat_actuel = EtatAmbiance::CALME;
            else if (k == 1)
                m_etat_actuel = EtatAmbiance::TENSION;
            break;
        }
        default:
            break;
        }
    }

    // Renvoie la couleur de la lumière pour le shader selon l'état actuel
    glm::vec3 get_light_color() const
    {
        switch (m_etat_actuel)
        {
        case EtatAmbiance::CALME: return glm::vec3(0.7f, 0.8f, 1.0f);   // Bleu 
        case EtatAmbiance::ETRANGE: return glm::vec3(0.6f, 0.3f, 0.8f); // Violet
        case EtatAmbiance::TENSION: return glm::vec3(1.0f, 0.5f, 0.2f); // Orange 
        case EtatAmbiance::CHAOS: return glm::vec3(1.0f, 0.1f, 0.1f);   // Rouge 
        default: return glm::vec3(1.0f, 1.0f, 1.0f);                    // Blanc 
        }
    }
};