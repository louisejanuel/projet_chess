#pragma once
#include "../Rendering/OpenGL.hpp"
#include "../Rendering/Model.hpp"
#include "../Rendering/Shader.hpp"
#include "../Rendering/ObjLoader.hpp"

class Board {
    Model caseVolume;
    Model pawnModel;
public:
    Board() {} 
    
    void init() { 
        caseVolume.setupCube(); 
        std::vector<float> pawnData = ObjLoader::load("../../src/Assets/models/pawn.obj");
        if (!pawnData.empty()) {
            pawnModel.setupFromData(pawnData);
        }
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

        // A. Les 8 Pions Blancs (Ligne z = 1)
        shader.setVec3("baseColor", glm::vec3(0.9f, 0.9f, 0.9f)); 
        for (int x = 0; x < 8; ++x) {
            glm::mat4 pModel = glm::mat4(1.0f);
            
            pModel = glm::translate(pModel, glm::vec3(x - 3.5f, 0.1f, 1.0f - 3.5f)); 
            
            pModel = glm::scale(pModel, glm::vec3(0.3f, 0.3f, 0.3f)); 
            
            shader.setMat4("model", pModel);
            pawnModel.draw();
        }

        // B. Les 8 Pions Noirs (Ligne z = 6)
        shader.setVec3("baseColor", glm::vec3(0.2f, 0.2f, 0.2f)); 
        for (int x = 0; x < 8; ++x) {
            glm::mat4 pModel = glm::mat4(1.0f);

            pModel = glm::translate(pModel, glm::vec3(x - 3.5f, 0.1f, 6.0f - 3.5f)); 
            
            pModel = glm::scale(pModel, glm::vec3(0.3f, 0.3f, 0.3f)); 
            
            shader.setMat4("model", pModel);
            pawnModel.draw();
        }
    }
};