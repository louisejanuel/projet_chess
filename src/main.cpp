#include "Rendering/OpenGL.hpp"
#include "Board/Board.hpp"
#include "Rendering/Shader.hpp"
#include "chessboard.hpp"
#include "View/GameRender.hpp"
#include <fstream>
#include <iostream>

int main() {
    // --- OBJETS 3D ---
    Board chessBoard3D;
    Shader* shader = nullptr;
    
    // --- OBJETS LOGIQUE ET 2D ---
    Chessboard logicBoard;    
    GameRender gameRender2D;  
    ImFont* chess_font = nullptr;

    quick_imgui::loop(
        "Échiquier 2D & 3D - IMAC",
        {
            .init = [&]() {
                // Initialisation 3D
                chessBoard3D.init(); 
                shader = new Shader("src/Shaders/board.vs", "src/Shaders/board.fs"); 
                
                // Initialisation de la police
                ImGuiIO& io = ImGui::GetIO();
                io.Fonts->AddFontDefault();
                static const ImWchar ranges[] = { 0x0020, 0x00FF, 0x2650, 0x265F, 0 };
                
                const char *fontPath = "src/Assets/DejaVuSans.ttf";
                std::ifstream fileCheck(fontPath);
                if (fileCheck.good()) {
                    chess_font = io.Fonts->AddFontFromFileTTF(fontPath, 75.0f, nullptr, ranges);
                } else {
                    std::cout << "ERREUR : Police 2D introuvable au chemin " << fontPath << std::endl;
                }
            },
            
            .loop = [&]() {
                // RENDU 3D
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glEnable(GL_DEPTH_TEST);

                if (shader != nullptr) {
                    shader->use();
                    
                    glm::mat4 view = glm::lookAt(
                        glm::vec3(0.0f, 6.0f, 10.0f), 
                        glm::vec3(0.0f, 0.0f, 0.0f),  
                        glm::vec3(0.0f, 1.0f, 0.0f)   
                    );
                    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
                    
                    shader->setMat4("view", view);
                    shader->setMat4("projection", projection);
                    shader->setVec3("lightPos", glm::vec3(0.0f, 10.0f, 0.0f));
                    shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

                    chessBoard3D.render(*shader);
                }

                // RENDU 2D
                gameRender2D.set_font(chess_font);
                gameRender2D.render(logicBoard); 
            }
        }
    );

    delete shader;
    return 0;
}