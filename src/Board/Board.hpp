#pragma once
#include "../Rendering/OpenGL.hpp"
#include "../Rendering/Model.hpp"
#include "../Rendering/Shader.hpp"

class Board {
    Model caseVolume;
public:
    Board() {} 
    
    void init() { 
        caseVolume.setupCube(); 
    }

    void render(Shader& shader) {
        for (int x = 0; x < 8; ++x) {
            for (int z = 0; z < 8; ++z) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x - 3.5f, 0.0f, z - 3.5f));
                shader.setMat4("model", model);
                shader.setInt("state", 0);
                
                // Si la somme (x+z) est paire, c'est une case claire
                if ((x + z) % 2 == 0) {
                    shader.setVec3("baseColor", glm::vec3(0.9f, 0.8f, 0.7f)); // Beige
                } else {
                    shader.setVec3("baseColor", glm::vec3(0.4f, 0.2f, 0.1f)); // Marron
                }

                caseVolume.draw();
            }
        }
    }
};