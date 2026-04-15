#include "App.hpp"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include "Game/GameChaos.hpp"
#include "Game/GameClassic.hpp"
#include "Render3D/Skybox.hpp"
#include "quick_imgui/quick_imgui.hpp"

void App::start_game(GameMode mode)
{
    if (mode == GameMode::Classic)
    {
        m_current_game = std::make_unique<GameClassic>();
    }
    else
    {
        m_current_game = std::make_unique<GameChaos>();
    }

    m_current_game->setup();
    m_render2D.reset();
    m_state = AppState::InGame;
}

void App::return_to_menu()
{
    m_current_game.reset();
    m_state = AppState::MainMenu;
}

void App::run()
{
    quick_imgui::loop(
        "ECHECS 2D & 3D - IMAC",
        {.init = [&]() {
                srand(time(NULL)); 
                
                ImGuiIO& io = ImGui::GetIO();
                // --- 1. INITIALISATION 3D ---
                m_chessBoard3D.init(); 
                m_shader.emplace("../../src/Shaders/board.vs", "../../src/Shaders/board.fs"); 
                m_skybox.init();
                m_skyboxShader.emplace("../../src/Shaders/skybox.vs", "../../src/Shaders/skybox.fs");
                
                // --- 2. INITIALISATION POLICES 2D ---
                io.Fonts->AddFontDefault();
                static const ImWchar ranges[] = { 0x0020, 0x00FF, 0x2650, 0x265F, 0 };
                const char *fontPath = "../../src/Assets/DejaVuSans.ttf";
                std::ifstream fileCheck(fontPath);
                if (fileCheck.good()) {
                    ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath, 40.0f, nullptr, ranges); 
                    m_render2D.set_font(font);
                }

                // --- 3. CREATION FRAMEBUFFER OPENGL ---
                glGenFramebuffers(1, &fbo);
                glBindFramebuffer(GL_FRAMEBUFFER, fbo);
                glGenTextures(1, &textureColorbuffer);
                glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewWidth, viewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
                glGenRenderbuffers(1, &rbo);
                glBindRenderbuffer(GL_RENDERBUFFER, rbo);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewWidth, viewHeight);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
                glBindFramebuffer(GL_FRAMEBUFFER, 0); },

         .loop = [&]() {
                if (m_state == AppState::MainMenu) 
                {
                    m_menu.draw(this);
                } 
                else if (m_state == AppState::InGame && m_current_game != nullptr) 
                {
                    ImGuiViewport* viewport = ImGui::GetMainViewport();
                    ImVec2 work_pos = viewport->WorkPos;
                    ImVec2 work_size = viewport->WorkSize;
                    
                    float left_panel_width = std::min(700.0f, work_size.x * 0.45f);
                    float top_left_height = left_panel_width;
                    
                    int targetWidth = std::max(1, (int)(work_size.x - left_panel_width));
                    int targetHeight = std::max(1, (int)work_size.y);

                    // REDIMENSIONNEMENT DYNAMIQUE DU FRAMEBUFFER
                    if (targetWidth != viewWidth || targetHeight != viewHeight) {
                        viewWidth = targetWidth; 
                        viewHeight = targetHeight;
                        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewWidth, viewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
                        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
                        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewWidth, viewHeight);
                    }

                    // --- Calcul de la caméra
                    glm::mat4 view = m_camera.getViewMatrix(); 
                    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)viewWidth / (float)viewHeight, 0.1f, 100.0f);
                    
                    // --- RENDU 3D OPENGL ---
                    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
                    glViewport(0, 0, viewWidth, viewHeight);
                    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    glEnable(GL_DEPTH_TEST);

                    // --- RENDU SKYBOX ---
                    if (m_skyboxShader.has_value()) {
                        m_skybox.render(*m_skyboxShader, view, projection);
                    }

                    if (m_shader.has_value()) {
                        m_shader->use();
                        m_shader->setMat4("view", view);
                        m_shader->setMat4("projection", projection);
                        m_shader->setVec3("lightPos", glm::vec3(0.0f, 10.0f, 0.0f));
                        // --- couleur de la chaîne de markov pour la lumière ---
                        m_shader->setVec3("lightColor", m_ambiance.get_light_color());
                        m_chessBoard3D.render(*m_shader, m_current_game->get_board());
                    }

                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glDisable(GL_DEPTH_TEST);
                    glClearColor(0.4f, 0.5f, 0.6f, 1.0f); 
                    glClear(GL_COLOR_BUFFER_BIT);

                    ImGui::SetNextWindowPos(ImVec2(work_pos.x + left_panel_width, work_pos.y), ImGuiCond_Always);
                    ImGui::SetNextWindowSize(ImVec2(targetWidth, targetHeight), ImGuiCond_Always);
                    ImGui::Begin("Plateau 3D", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
                   

                    // --- GESTION DES INPUTS SOURIS POUR LA CAMÉRA ---
                    if (ImGui::IsWindowHovered()) {
                        if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                            ImVec2 mouseDelta = ImGui::GetIO().MouseDelta;
                            // On annule le mouvement si la souris vient de faire un grand saut
                            if (abs(mouseDelta.x) < 50.0f && abs(mouseDelta.y) < 50.0f) {
                                m_camera.processMouseMovement(mouseDelta.x, mouseDelta.y);
                            }
                        }
                        float scrollDelta = ImGui::GetIO().MouseWheel;
                        if (scrollDelta != 0.0f) {
                            m_camera.processMouseScroll(scrollDelta);
                        }
                    }

                    // --- FENETRE 3D IMGUI ---
                     ImGui::Image((void*)(intptr_t)textureColorbuffer, ImVec2((float)viewWidth, (float)viewHeight), ImVec2(0, 1), ImVec2(1, 0));
                    ImGui::End();

                    // --- FENETRE 2D ---
                    ImGui::SetNextWindowPos(ImVec2(work_pos.x, work_pos.y), ImGuiCond_Always);
                    ImGui::SetNextWindowSize(ImVec2(left_panel_width, top_left_height), ImGuiCond_Always);
                    m_render2D.render(*m_current_game, m_ambiance);
                    
                    // --- FENETRE CONTROLES ---
                    ImGui::SetNextWindowPos(ImVec2(work_pos.x, work_pos.y + top_left_height), ImGuiCond_Always);
                    ImGui::SetNextWindowSize(ImVec2(left_panel_width, work_size.y - top_left_height), ImGuiCond_Always);
                    ImGui::Begin("Controles", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
                    
                    if (ImGui::Button("Mode Classique", ImVec2(150, 40))) {
                        start_game(GameMode::Classic);
                        ImGui::End();
                        return;
                    }
                    
                    ImGui::SameLine();
                    
                    if (ImGui::Button("Mode Chaos", ImVec2(150, 40))) {
                        start_game(GameMode::Chaos);
                        ImGui::End();
                        return;
                    }
                    
                    ImGui::SameLine();
                    
                    if (ImGui::Button("Menu Principal", ImVec2(150, 40))) {
                        return_to_menu();
                        ImGui::End();
                        return;
                    }

                    // --- AFFICHAGE MODE CHAOS (PERDRE POUR GAGNER) ---
                    if (m_current_game->is_rules_inverted()) {
                        ImGui::Spacing();
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 1.0f, 1.0f)); // Violet pétant
                        ImGui::TextWrapped("LE CHAOS FRAPPE !!!\nLES REGLES SONT INVERSEES :\nPERDEZ VOTRE ROI POUR GAGNER !");
                        ImGui::PopStyleColor();
                    }

                    // --- AFFICHAGE MODE CHAOS (PROMOTION ALEATOIRE) ---
                    if (m_current_game->get_game_mode() == GameMode::Chaos) {
                        Type last = m_current_game->get_last_promoted();
                        if (last != Type::None) {
                            ImGui::Text("Derniere promotion : ");
                            ImGui::SameLine();
                            if (last == Type::Queen) {
                                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "UNE REINE ! Tu en as de la chance toi");
                            } 
                            else if (last == Type::Rook) {
                                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Une tour");
                            } 
                            else if (last == Type::Bishop) {
                                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Un fou");
                            } 
                            else if (last == Type::Knight) {
                                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Un cavalier");
                            }
                        }
                    }

                    // --- AFFICHAGE VICTOIRE ---                  
                    if (m_current_game->get_state() != GameState::Playing) {
                        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
                        ImGui::SetWindowFontScale(1.5f);
                        if (m_current_game->get_state() == GameState::WhiteWins) {
                            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "VICTOIRE : Les blancs ont gagne !");
                        } else {
                            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "VICTOIRE : Les noirs ont gagne !");
                        }
                        ImGui::SetWindowFontScale(1.0f);
                    }
                    ImGui::End();
                } }
        }
    );
}