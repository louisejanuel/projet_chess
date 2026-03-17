#include <fstream>
#include <iostream>
#include "Board/Board.hpp"
#include "Chessboard.hpp"
#include "Rendering/OpenGL.hpp"
#include "Rendering/Shader.hpp"
#include "View/GameRender.hpp"

int main()
{
    Board      chessBoard3D;
    Shader*    shader = nullptr;
    Chessboard logicBoard;
    GameRender gameRender2D;
    ImFont*    chess_font = nullptr;

    GLuint fbo = 0, textureColorbuffer = 0, rbo = 0;
    int    viewWidth  = 800;
    int    viewHeight = 600;

    quick_imgui::loop(
        "Échiquier 2D & 3D - IMAC",
        {.init = [&]() {
             chessBoard3D.init();
             shader = new Shader("../../src/Shaders/board.vs", "../../src/Shaders/board.fs");

             ImGuiIO& io = ImGui::GetIO();
             io.Fonts->AddFontDefault();
             static const ImWchar ranges[] = {0x0020, 0x00FF, 0x2650, 0x265F, 0};
             const char*          fontPath = "../../src/Assets/DejaVuSans.ttf";
             std::ifstream        fileCheck(fontPath);
             if (fileCheck.good())
             {
                 chess_font = io.Fonts->AddFontFromFileTTF(fontPath, 40.0f, nullptr, ranges);
             }

             // --- CRÉATION DU FRAMEBUFFER ---
             glGenFramebuffers(1, &fbo);
             glBindFramebuffer(GL_FRAMEBUFFER, fbo);

             // Création de la texture qui contiendra l'image 3D
             glGenTextures(1, &textureColorbuffer);
             glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
             glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewWidth, viewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
             glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

             // Création du Renderbuffer pour la profondeur (la 3D)
             glGenRenderbuffers(1, &rbo);
             glBindRenderbuffer(GL_RENDERBUFFER, rbo);
             glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewWidth, viewHeight);
             glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

             glBindFramebuffer(GL_FRAMEBUFFER, 0); 
         },

         .loop = [&]() {
                // 1. On dit à OpenGL de dessiner DANS notre texture invisible
                glBindFramebuffer(GL_FRAMEBUFFER, fbo);
                glViewport(0, 0, viewWidth, viewHeight);
                glClearColor(0.1f, 0.1f, 0.15f, 1.0f); 
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glEnable(GL_DEPTH_TEST);

                // 2. Le rendu 3D classique
                if (shader != nullptr) {
                    shader->use();
                    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 6.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)viewWidth / (float)viewHeight, 0.1f, 100.0f);
                    shader->setMat4("view", view);
                    shader->setMat4("projection", projection);
                    shader->setVec3("lightPos", glm::vec3(0.0f, 10.0f, 0.0f));
                    shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
                    chessBoard3D.render(*shader);
                }

                // 3. On revient sur l'écran par défaut pour dessiner ImGui
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                
                // Note : on efface l'écran principal (sinon on aura des artefacts)
                glClearColor(0.4f, 0.5f, 0.6f, 1.0f); 
                glClear(GL_COLOR_BUFFER_BIT);

                // 4. FENÊTRE IMGUI POUR LA VUE 3D
                ImGui::Begin("Plateau 3D");
                // On ajoute ImVec2(0, 1) et ImVec2(1, 0) pour retourner l'image !
                ImGui::Image((void*)(intptr_t)textureColorbuffer, ImVec2((float)viewWidth, (float)viewHeight), ImVec2(0, 1), ImVec2(1, 0));
                ImGui::End();

                // 5. FENÊTRE IMGUI POUR LA VUE 2D
                gameRender2D.set_font(chess_font);
                gameRender2D.render(logicBoard); }}
    );

    delete shader;
    return 0;
}