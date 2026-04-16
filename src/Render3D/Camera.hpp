#pragma once
#include <algorithm>
#include <cmath>
#include "OpenGL.hpp"

enum class CameraMode {
    Trackball,
    FirstPerson
};

class Camera {
private:
    // --- Variables Trackball ---
    float     radius         = 12.0f;
    float     trackballYaw   = 90.0f;
    float     trackballPitch = 45.0f;
    glm::vec3 target         = glm::vec3(0.0f, 1.5f, 0.0f);

    // --- Variables First Person (FPS) ---
    float     fpYaw      = -90.0f;
    float     fpPitch    = 0.0f;
    glm::vec3 fpPosition = glm::vec3(0.0f, 2.5f, 0.0f);

    CameraMode mode = CameraMode::Trackball;

public:
    Camera() = default;

    // Trackball
    void setTrackballMode() {
        mode           = CameraMode::Trackball;
        trackballPitch = 45.0f;
        trackballYaw   = 90.0f;
        radius         = 12.0f;
    }

    // FPM
    void setFirstPersonMode(const glm::vec3& piecePosition) {
        mode       = CameraMode::FirstPerson;
        fpPosition = piecePosition + glm::vec3(0.0f, 4.0f, 0.0f);

        // Vue vers l'échiquier changement pièce
        glm::vec3 direction = glm::normalize(glm::vec3(0.0f) - fpPosition);
        fpPitch             = glm::degrees(asin(direction.y));
        fpYaw               = glm::degrees(atan2(direction.z, direction.x));
    }

    CameraMode getMode() const { return mode; }

    // Calcule et retourne position de la caméra
    glm::vec3 getPosition() const {
        if (mode == CameraMode::Trackball) {
            float radYaw   = glm::radians(trackballYaw);
            float radPitch = glm::radians(trackballPitch);
            
            float camX = target.x + radius * cos(radPitch) * cos(radYaw);
            float camY = target.y + radius * sin(radPitch);
            float camZ = target.z + radius * cos(radPitch) * sin(radYaw);
            
            return glm::vec3(camX, camY, camZ);
        }
        return fpPosition;
    }

    // Génère la matrice complète pour Shader
    glm::mat4 getViewMatrix() const {
        if (mode == CameraMode::Trackball) {
            return glm::lookAt(getPosition(), target, glm::vec3(0.0f, 1.0f, 0.0f));
        } else {
            float radYaw   = glm::radians(fpYaw);
            float radPitch = glm::radians(fpPitch);

            glm::vec3 front;
            front.x = cos(radYaw) * cos(radPitch);
            front.y = sin(radPitch);
            front.z = sin(radYaw) * cos(radPitch);
            front   = glm::normalize(front);

            return glm::lookAt(fpPosition, fpPosition + front, glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }

    // Modifie selon souris
    void processMouseMovement(float xoffset, float yoffset) {
        constexpr float sensitivity = 0.2f;

        if (mode == CameraMode::Trackball) {
            trackballYaw   -= xoffset * sensitivity;
            trackballPitch += yoffset * sensitivity;
            // Bloque cam
            trackballPitch = std::clamp(trackballPitch, 5.0f, 89.0f);
        } else {
            fpYaw   += xoffset * sensitivity;
            fpPitch -= yoffset * sensitivity;
            fpPitch = std::clamp(fpPitch, -89.9f, 89.9f);
        }
    }

    // Zoom molette
    void processMouseScroll(float yoffset) {
        if (mode == CameraMode::Trackball) {
            radius -= yoffset * 0.5f;
            radius = std::clamp(radius, 5.0f, 25.0f);
        }
    }
};