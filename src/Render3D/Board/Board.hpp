#pragma once
#include "../../Game/Chessboard.hpp"
#include "../Model.hpp"
#include "../ObjLoader.hpp"
#include "../OpenGL.hpp"
#include "../Shader.hpp"

class Board {
    Model caseVolume;
    Model pawnModel;
    Model rookModel;
    Model knightModel;
    Model bishopModel;
    Model queenModel;
    Model kingModel;
    Model berolinaPawnModel;

public:
    Board() {}

    void init()
    {
        caseVolume.setupCube();

        auto loadModel = [](Model& model, const std::string& path) {
            MeshData data = ObjLoader::load(path);
            if (!data.vertices.empty())
            {
                model.setupFromData(data);
            }
        };

        loadModel(pawnModel, "../../src/Assets/models/pawn.obj");
        loadModel(berolinaPawnModel, "../../src/Assets/models/pawn.obj");
        loadModel(rookModel, "../../src/Assets/models/rook.obj");
        loadModel(knightModel, "../../src/Assets/models/knight.obj");
        loadModel(bishopModel, "../../src/Assets/models/bishop.obj");
        loadModel(queenModel, "../../src/Assets/models/queen.obj");
        loadModel(kingModel, "../../src/Assets/models/king.obj");
    }

    void render(Shader& shader, const Chessboard& logicBoard)
    {
        for (int x = 0; x < 8; ++x)
        {
            for (int z = 0; z < 8; ++z)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model           = glm::translate(model, glm::vec3(x - 3.5f, 0.0f, z - 3.5f));
                shader.setMat4("model", model);
                shader.setInt("state", 0);

                if ((x + z) % 2 == 0)
                    shader.setVec3("baseColor", glm::vec3(0.9f, 0.8f, 0.7f));
                else
                    shader.setVec3("baseColor", glm::vec3(0.4f, 0.2f, 0.1f));

                caseVolume.draw();
            }
        }

        for (int i = 0; i < 64; ++i)
        {
            if (!logicBoard.is_empty(i))
            {
                Piece* piece = logicBoard.get_piece(i);
                Type   type  = piece->get_type();

                int x = i % 8;
                int z = i / 8;

                if (piece->get_color() == Color::White)
                {
                    shader.setVec3("baseColor", glm::vec3(0.9f, 0.9f, 0.9f));
                }
                else
                {
                    shader.setVec3("baseColor", glm::vec3(0.2f, 0.2f, 0.2f));
                }

                float localScale = 0.1f;
                float rotationX  = -90.0f;
                float rotationY  = 0.0f;

                // FINI LE BRICOLAGE, YOFFSET EST A 0 POUR TOUT LE MONDE !
                float     yOffset = 0.0f;
                glm::vec3 offset(0.0f, 0.0f, 0.0f);

                if (type == Type::Pawn)
                {
                    localScale = 0.3f;
                    rotationX  = 0.0f;
                    offset     = glm::vec3(0.0f, 0.0f, 0.0f);
                }
                if (type == Type::BerolinaPawn)
                {
                    localScale = 0.3f;
                    rotationX  = 0.0f;
                    offset     = glm::vec3(0.0f, 0.0f, 0.0f);
                }
                else if (type == Type::Rook)
                {
                    localScale = 0.1f;
                    rotationX  = -90.0f;
                    offset     = glm::vec3(-16.4f, -17.6f, 0.0f); // Z est à 0.0f !
                }
                else if (type == Type::Bishop)
                {
                    localScale = 0.1f;
                    rotationX  = -90.0f;
                    offset     = glm::vec3(-6.5f, -17.25f, 0.0f);
                }
                else if (type == Type::Knight)
                {
                    localScale = 0.1f;
                    rotationX  = -90.0f;
                    offset     = glm::vec3(-10.7f, -15.15f, 0.0f);
                    if (piece->get_color() == Color::White)
                    {
                        rotationY = 180.0f;
                    }
                }
                else if (type == Type::Queen)
                {
                    localScale = 0.1f;
                    rotationX  = -90.0f;
                    offset     = glm::vec3(3.03f, 16.46f, 0.0f);
                }
                else if (type == Type::King)
                {
                    localScale = 0.1f;
                    rotationX  = -90.0f;
                    offset     = glm::vec3(-2.83f, 16.4f, 0.0f);
                }

                glm::mat4 pModel = glm::mat4(1.0f);
                pModel           = glm::translate(pModel, glm::vec3(x - 3.5f, 0.1f + yOffset, z - 3.5f));
                pModel           = glm::rotate(pModel, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
                pModel           = glm::rotate(pModel, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
                pModel           = glm::scale(pModel, glm::vec3(localScale));
                pModel           = glm::translate(pModel, offset);

                shader.setMat4("model", pModel);

                if (type == Type::Pawn)
                    pawnModel.draw();
                else if (type == Type::BerolinaPawn)
                    berolinaPawnModel.draw();
                else if (type == Type::Rook)
                    rookModel.draw();
                else if (type == Type::Bishop)
                    bishopModel.draw();
                else if (type == Type::Knight)
                    knightModel.draw();
                else if (type == Type::Queen)
                    queenModel.draw();
                else if (type == Type::King)
                    kingModel.draw();
            }
        }
    }
};