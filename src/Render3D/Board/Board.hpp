#pragma once
#include "../../Game/Chessboard.hpp"
#include "../Model.hpp"
#include "../ObjLoader.hpp"
#include "../OpenGL.hpp"
#include "../Shader.hpp"
#include <vector>

class Board {
    Model caseVolume;
    Model pawnModel;
    Model rookModel;
    Model knightModel;
    Model bishopModel;
    Model queenModel;
    Model kingModel;

public:
    Board() {}

    void init()
    {
        caseVolume.setupCube();

        auto loadModel = [](Model& model, const std::string& path) {
            MeshData data = ObjLoader::load(path);
            if (!data.vertices.empty()) {
                model.setupFromData(data);
            }
        };

        loadModel(pawnModel, "../../src/Assets/models/pawn.obj");
        loadModel(rookModel, "../../src/Assets/models/rook.obj");
        loadModel(knightModel, "../../src/Assets/models/knight.obj");
        loadModel(bishopModel, "../../src/Assets/models/bishop.obj");
        loadModel(queenModel, "../../src/Assets/models/queen.obj");
        loadModel(kingModel, "../../src/Assets/models/king.obj");
    }

    // --- NOUVELLE SIGNATURE AVEC L'ANIMATION ---
    void render(Shader& shader, const Chessboard& logicBoard, const std::vector<int>& states, 
                int animFrom = -1, int animTo = -1, float animProgress = 1.0f)
    {
        // --- 1. DESSIN DU DAMIER ---
        for (int x = 0; x < 8; ++x) {
            for (int z = 0; z < 8; ++z) {
                int index = z * 8 + x;
                int state = states[index];

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x - 3.5f, 0.0f, z - 3.5f));
                shader.setMat4("model", model);
                shader.setInt("state", 0);

                glm::vec3 baseColor;
                if ((x + z) % 2 == 0) baseColor = glm::vec3(0.9f, 0.8f, 0.7f);
                else baseColor = glm::vec3(0.4f, 0.2f, 0.1f);

                if (state == 2) baseColor = glm::vec3(0.4f, 0.8f, 0.4f); 
                else if (state == 1) baseColor = glm::vec3(0.8f, 0.8f, 0.4f); 
                else if (state == 3) baseColor += glm::vec3(0.2f, 0.2f, 0.2f); 

                shader.setVec3("baseColor", baseColor);
                caseVolume.draw();
            }
        }

        // --- 2. DESSIN DU CADRE ---
        float thickness = 0.4f; float height = 0.2f;    
        glm::vec3 borderColor = glm::vec3(1.0f, 0.4f, 0.7f); 
        shader.setVec3("baseColor", borderColor);

        for (int i = 0; i < 4; ++i) {
            glm::mat4 borderModel = glm::mat4(1.0f);
            float offset = 4.0f + (thickness / 2.0f);
            
            if (i == 0) borderModel = glm::translate(borderModel, glm::vec3(0.0f, 0.0f, -offset));
            else if (i == 1) borderModel = glm::translate(borderModel, glm::vec3(0.0f, 0.0f, offset));
            else if (i == 2) borderModel = glm::translate(borderModel, glm::vec3(offset, 0.0f, 0.0f));
            else if (i == 3) borderModel = glm::translate(borderModel, glm::vec3(-offset, 0.0f, 0.0f));

            if (i < 2) borderModel = glm::scale(borderModel, glm::vec3(8.0f + 2.0f * thickness, height, thickness));
            else borderModel = glm::scale(borderModel, glm::vec3(thickness, height, 8.0f));

            shader.setMat4("model", borderModel);
            caseVolume.draw();
        }

        // --- 3. DESSIN DES PIÈCES ---
        for (int i = 0; i < 64; ++i)
        {
            if (!logicBoard.is_empty(i))
            {
                Piece* piece = logicBoard.get_piece(i);
                Type   type  = piece->get_type();
                int    state = states[i];

                int x = i % 8;
                int z = i / 8;

                glm::vec3 pieceColor;
                if (piece->get_color() == Color::White) pieceColor = glm::vec3(0.9f, 0.9f, 0.9f);
                else pieceColor = glm::vec3(0.2f, 0.2f, 0.2f);

                if (state == 2) pieceColor += glm::vec3(0.0f, 0.4f, 0.0f); 
                else if (state == 3) pieceColor += glm::vec3(0.2f, 0.2f, 0.2f); 

                shader.setVec3("baseColor", pieceColor);

                float localScale = 0.1f; float rotationX  = -90.0f; float rotationY  = 0.0f;
                glm::vec3 offset(0.0f, 0.0f, 0.0f);
                Model* modelToDraw = nullptr;

                switch (type) {
                    case Type::Pawn:
                    case Type::BerolinaPawn: 
                        localScale = 0.3f; rotationX = 0.0f; modelToDraw = &pawnModel; break;
                    case Type::Rook: offset = glm::vec3(-16.4f, -17.6f, 0.0f); modelToDraw = &rookModel; break;
                    case Type::Bishop: offset = glm::vec3(-6.5f, -17.25f, 0.0f); modelToDraw = &bishopModel; break;
                    case Type::Knight: offset = glm::vec3(-10.7f, -15.15f, 0.0f);
                        if (piece->get_color() == Color::White) rotationY = 180.0f;
                        modelToDraw = &knightModel; break;
                    case Type::Queen: offset = glm::vec3(3.03f, 16.46f, 0.0f); modelToDraw = &queenModel; break;
                    case Type::King: offset = glm::vec3(-2.83f, 16.4f, 0.0f); modelToDraw = &kingModel; break;
                    default: break;
                }

                // ===============================================
                // --- CALCUL DE LA POSITION (AVEC ANIMATION) ---
                // ===============================================
                glm::vec3 piecePos(x - 3.5f, 0.1f, z - 3.5f);

                if (i == animTo && animProgress < 1.0f) {
                    int fromX = animFrom % 8;
                    int fromZ = animFrom / 8;
                    glm::vec3 fromPos(fromX - 3.5f, 0.1f, fromZ - 3.5f);
                    
                    // Smoothstep : accélère au début, ralentit à la fin
                    float t = animProgress * animProgress * (3.0f - 2.0f * animProgress); 
                    piecePos = glm::mix(fromPos, piecePos, t); // Le fameux LERP !
                }

                glm::mat4 pModel = glm::mat4(1.0f);
                pModel = glm::translate(pModel, piecePos); // On utilise la position calculée !
                pModel = glm::rotate(pModel, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
                pModel = glm::rotate(pModel, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
                pModel = glm::scale(pModel, glm::vec3(localScale));
                pModel = glm::translate(pModel, offset);

                shader.setMat4("model", pModel);
                if (modelToDraw != nullptr) modelToDraw->draw();
            }
        }
    }
};