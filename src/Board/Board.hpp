#pragma once
#include "../Rendering/OpenGL.hpp"
#include "../Rendering/Model.hpp"
#include "../Rendering/Shader.hpp"
#include "../Rendering/ObjLoader.hpp"
#include "../Chessboard.hpp"

class Board {
    Model caseVolume;
    Model pawnModel;

public:
    Board() {} 
    
    void init() { 
        caseVolume.setupCube(); 
        
        // Chargement de ton pion
        std::vector<float> pawnData = ObjLoader::load("src/Assets/models/pawn.obj");
        if (!pawnData.empty()) {
            pawnModel.setupFromData(pawnData);
        }
    }

    void render(Shader& shader, const Chessboard& logicBoard) {
        // --- 1. DESSIN DU DAMIER ---
        for (int x = 0; x < 8; ++x) {
            for (int z = 0; z < 8; ++z) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x - 3.5f, 0.0f, z - 3.5f));
                shader.setMat4("model", model);
                shader.setInt("state", 0);
                
                if ((x + z) % 2 == 0) {
                    shader.setVec3("baseColor", glm::vec3(0.9f, 0.8f, 0.7f)); 
                } else {
                    shader.setVec3("baseColor", glm::vec3(0.4f, 0.2f, 0.1f)); 
                }

                caseVolume.draw();
            }
        }

        // --- 2. DESSIN DES PIÈCES DEPUIS LA LOGIQUE ---
        // On parcourt les 64 index de ton tableau "m_pieces"
        for (int i = 0; i < 64; ++i) {
            
            if (!logicBoard.is_empty(i)) {
                
                Piece* piece = logicBoard.get_piece(i);
                
                // Astuce mathématique : transformer un index 1D (0 à 63) en (x, z)
                int x = i % 8;
                int z = i / 8;

                // On vérifie la couleur pour appliquer le bon matériau
                if (piece->get_color() == Color::White) {
                    shader.setVec3("baseColor", glm::vec3(0.9f, 0.9f, 0.9f)); // Blanc
                } else {
                    shader.setVec3("baseColor", glm::vec3(0.2f, 0.2f, 0.2f)); // Noir
                }

                glm::mat4 pModel = glm::mat4(1.0f);
                // On utilise les x et z qu'on a calculés juste au-dessus !
                pModel = glm::translate(pModel, glm::vec3(x - 3.5f, 0.1f, z - 3.5f)); 
                
                pModel = glm::scale(pModel, glm::vec3(0.3f, 0.3f, 0.3f)); 
                shader.setMat4("model", pModel);
                
                // On dessine le pion (temporairement pour toutes les pièces)
                pawnModel.draw();
            }
        }
    }
};