#pragma once
#include "OpenGL.hpp"
#include <algorithm> // Pour std::clamp

enum class CameraMode {
    Trackball,
    FirstPerson // On s'en servira pour le deuxième mode !
};

class Camera {
private:
    float radius = 12.0f;
    float yaw = 90.0f;   
    float pitch = 45.0f; 
    // CONSIGNE : "regarde au centre de l’échiquier (voire un peu au dessus)"
    glm::vec3 target = glm::vec3(0.0f, 1.5f, 0.0f); 

    CameraMode mode = CameraMode::Trackball;

public:
    Camera() = default;

    glm::mat4 getViewMatrix() const {
        if (mode == CameraMode::Trackball) {
            float radYaw = glm::radians(yaw);
            float radPitch = glm::radians(pitch);

            float camX = target.x + radius * cos(radPitch) * cos(radYaw);
            float camY = target.y + radius * sin(radPitch);
            float camZ = target.z + radius * cos(radPitch) * sin(radYaw);
            
            return glm::lookAt(glm::vec3(camX, camY, camZ), target, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        return glm::mat4(1.0f); 
    }

    void processMouseMovement(float xoffset, float yoffset) {
        if (mode == CameraMode::Trackball) {
            float sensitivity = 0.2f; // On baisse la sensibilité pour que ce soit contrôlable
            yaw -= xoffset * sensitivity; 
            pitch += yoffset * sensitivity; 

            // CONSIGNE : "ne devra pas passer et regarder en dessous de l’échiquier"
            // 5.0f = bloqué juste au-dessus du plateau, 89.0f = bloqué juste au-dessus (vue de haut)
            pitch = std::clamp(pitch, 5.0f, 89.0f);
        }
    }

    void processMouseScroll(float yoffset) {
        if (mode == CameraMode::Trackball) {
            radius -= yoffset * 0.5f;
            radius = std::clamp(radius, 5.0f, 25.0f); 
        }
    }
};