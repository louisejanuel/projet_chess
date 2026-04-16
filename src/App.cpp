#include "App.hpp"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
#include "Game/GameChaos.hpp"
#include "Game/GameClassic.hpp"
#include "Render3D/Skybox.hpp"
#include "quick_imgui/quick_imgui.hpp"

// Initialise et lance une nouvelle partie selon le mode choisi
void App::start_game(GameMode mode)
{
    if (mode == GameMode::Classic) {
        m_current_game = std::make_unique<GameClassic>();
    } else {
        m_current_game = std::make_unique<GameChaos>();
    }

    m_current_game->setup();
    m_render2D.reset();
    
    m_isPieceView = false;
    m_camera.setTrackballMode();
    m_lastSelectedPiece = -1;
    m_hovered_idx_3d = -1;
    m_state = AppState::InGame;
}

// Détruit la partie en cours et retourne au menu principal
void App::return_to_menu()
{
    m_current_game.reset();
    m_state = AppState::MainMenu;
}

// Point d'entrée principal contenant la boucle de rendu globale
void App::run()
{
    quick_imgui::loop(
        "ECHECS 2D & 3D - IMAC",
        {.init = [&]() {
            // Initialisation du générateur aléatoire
            srand(time(NULL));

            // Chargement des modèles 3D et des shaders
            m_chessBoard3D.init();
            m_shader.emplace("src/Shaders/board.vs", "src/Shaders/board.fs");
            m_skybox.init();
            m_skyboxShader.emplace("src/Shaders/skybox.vs", "src/Shaders/skybox.fs");

            // Configuration et chargement des polices d'écriture
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontDefault();
            static const ImWchar ranges[] = {0x0020, 0x00FF, 0x2650, 0x265F, 0};
            const char* fontPath = "src/Assets/DejaVuSans.ttf";
            std::ifstream fileCheck(fontPath);
            if (fileCheck.good()) {
                ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath, 40.0f, nullptr, ranges);
                m_render2D.set_font(font);
            }

            // Génération du framebuffer pour afficher la 3D dans ImGui
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
            glBindFramebuffer(GL_FRAMEBUFFER, 0); 
        },

         .loop = [&]() {
            // Affichage du menu principal si la partie n'est pas lancée
            if (m_state == AppState::MainMenu) {
                m_menu.draw(this);
            }
            // Boucle de la partie en cours
            else if (m_state == AppState::InGame && m_current_game != nullptr) {
                
                // Calcul des dimensions dynamiques de l'interface
                ImGuiViewport* viewport  = ImGui::GetMainViewport();
                ImVec2         work_pos  = viewport->WorkPos;
                ImVec2         work_size = viewport->WorkSize;
                float left_panel_width = std::min(700.0f, work_size.x * 0.45f);
                float top_left_height  = left_panel_width;
                int targetWidth  = std::max(1, (int)(work_size.x - left_panel_width));
                int targetHeight = std::max(1, (int)work_size.y);

                // Avancement de l'interpolation temporelle des animations
                if (m_animProgress < 1.0f) {
                    m_animProgress += ImGui::GetIO().DeltaTime * 3.0f; 
                    if (m_animProgress > 1.0f) m_animProgress = 1.0f;
                }

                // Redimensionnement de la texture OpenGL si la fenêtre change de taille
                if (targetWidth != viewWidth || targetHeight != viewHeight) {
                    viewWidth  = targetWidth;
                    viewHeight = targetHeight;
                    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewWidth, viewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
                    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
                    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewWidth, viewHeight);
                }

                // Calcul de la perspective de la caméra
                glm::mat4 view       = m_camera.getViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)viewWidth / (float)viewHeight, 0.1f, 100.0f);

                // Préparation du contexte OpenGL pour le rendu de la frame
                glBindFramebuffer(GL_FRAMEBUFFER, fbo);
                glViewport(0, 0, viewWidth, viewHeight);
                glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glEnable(GL_DEPTH_TEST);

                // Dessin du fond d'environnement
                if (m_skyboxShader.has_value()) {
                    m_skybox.render(*m_skyboxShader, view, projection);
                }

                // Dessin du plateau et des pièces
                if (m_shader.has_value()) {
                    m_shader->use();
                    m_shader->setMat4("view", view);
                    m_shader->setMat4("projection", projection);
                    
                    // Transmission des données lumineuses aux shaders
                    m_shader->setVec3("lightPos", glm::vec3(0.0f, 10.0f, 0.0f));
                    m_shader->setVec3("lightColor", m_ambiance.get_light_color());
                    m_shader->setVec3("mobileLightPos", m_mobileLightPos);
                    int isWhite = (m_current_game->get_current_turn() == Color::White) ? 1 : 0;
                    m_shader->setInt("isWhiteTurn", isWhite);

                    // Génération du tableau des états visuels des 64 cases
                    std::vector<int> boardStates(64, 0); 
                    int currentSelected = m_render2D.get_selected_index();
                    
                    for (int i = 0; i < 64; ++i) {
                        if (i == currentSelected) {
                            boardStates[i] = 2; 
                        } else if (m_render2D.is_highlighted(i)) {
                            boardStates[i] = 1; 
                        }
                    }
                    
                    // Superposition de l'état de survol prioritaire
                    if (m_hovered_idx_3d != -1) {
                        boardStates[m_hovered_idx_3d] = 3; 
                    }

                    // Exécution du rendu complet du modèle 3D
                    m_chessBoard3D.render(*m_shader, m_current_game->get_board(), boardStates, m_animFrom, m_animTo, m_animProgress);            
                }

                // Clôture du rendu OpenGL et retour au framebuffer par défaut
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glDisable(GL_DEPTH_TEST);
                glClearColor(0.4f, 0.5f, 0.6f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // Affichage de la texture 3D dans une fenêtre ImGui
                ImGui::SetNextWindowPos(ImVec2(work_pos.x + left_panel_width, work_pos.y), ImGuiCond_Always);
                ImGui::SetNextWindowSize(ImVec2(targetWidth, targetHeight), ImGuiCond_Always);
                ImGui::Begin("Plateau 3D", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

                ImVec2 imagePos = ImGui::GetCursorScreenPos();
                ImGui::Image((void*)(intptr_t)textureColorbuffer, ImVec2((float)viewWidth, (float)viewHeight), ImVec2(0, 1), ImVec2(1, 0));

                bool is3DViewHovered = ImGui::IsItemHovered();
                m_hovered_idx_3d = -1;

                // Interactions au survol de la vue 3D
                if (is3DViewHovered) {
                    
                    // Raycasting pour la détection du survol de case
                    ImVec2 mousePos = ImGui::GetMousePos();
                    float mouseX = mousePos.x - imagePos.x;
                    float mouseY = mousePos.y - imagePos.y;

                    float ndcX = (2.0f * mouseX) / viewWidth - 1.0f;
                    float ndcY = 1.0f - (2.0f * mouseY) / viewHeight; 

                    glm::vec4 ray_clip(ndcX, ndcY, -1.0f, 1.0f);
                    glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
                    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
                    glm::vec3 ray_wor = glm::normalize(glm::vec3(glm::inverse(view) * ray_eye));

                    glm::vec3 camPos = m_camera.getPosition();

                    // Intersection géométrique avec le plan de l'échiquier
                    if (ray_wor.y < 0.0f) {
                        float t = -camPos.y / ray_wor.y;
                        glm::vec3 hitPoint = camPos + ray_wor * t;

                        int col = (int)floor(hitPoint.x + 4.0f);
                        int row = (int)floor(hitPoint.z + 4.0f);

                        if (col >= 0 && col < 8 && row >= 0 && row < 8) {
                            m_hovered_idx_3d = row * 8 + col; 
                        }
                    }
                    
                    // Transmission du clic gauche 3D vers le moteur logique 2D
                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                        ImGui::SetWindowFocus();
                        if (m_hovered_idx_3d != -1) {
                            Color turnBefore = m_current_game->get_current_turn();
                            int selBefore = m_render2D.get_selected_index();

                            m_render2D.handle_click(*m_current_game, m_hovered_idx_3d, m_ambiance);

                            // Initialisation de l'animation en cas de validation de mouvement
                            if (turnBefore != m_current_game->get_current_turn() && selBefore != -1) {
                                m_animFrom = selBefore;
                                m_animTo = m_hovered_idx_3d;
                                m_animProgress = 0.0f; 
                            }
                        }
                    }

                    // Gestion du zoom avec la molette
                    float scrollDelta = ImGui::GetIO().MouseWheel;
                    if (scrollDelta != 0.0f) m_camera.processMouseScroll(scrollDelta);
                }

                // Prise de focus et gestion de la rotation de la caméra
                if (ImGui::IsWindowFocused() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                    ImVec2 mouseDelta = ImGui::GetIO().MouseDelta;
                    m_camera.processMouseMovement(mouseDelta.x, mouseDelta.y);
                }
                ImGui::End();

                // Affichage du plateau 2D dans le panneau latéral
                ImGui::SetNextWindowPos(ImVec2(work_pos.x, work_pos.y), ImGuiCond_Always);
                ImGui::SetNextWindowSize(ImVec2(left_panel_width, top_left_height), ImGuiCond_Always);
                m_render2D.render(*m_current_game, m_ambiance);

                // Panneau d'options et de contrôles
                ImGui::SetNextWindowPos(ImVec2(work_pos.x, work_pos.y + top_left_height), ImGuiCond_Always);
                ImGui::SetNextWindowSize(ImVec2(left_panel_width, work_size.y - top_left_height), ImGuiCond_Always);
                ImGui::Begin("Controles", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

                // Interrupteur du point de vue subjectif (FPS)
                if (ImGui::Checkbox("Mode 'Vue Piece' (FPS)", &m_isPieceView)) {
                    if (!m_isPieceView) {
                        m_camera.setTrackballMode();
                        m_lastSelectedPiece = -1;
                    }
                }

                // Coordonnées interactives de la lumière mobile
                ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
                ImGui::Text("Deplacer la lumiere mobile (Mode Blanc) :");
                
                if (ImGui::Button("Devant", ImVec2(100, 30))) m_mobileLightPos.z -= 1.0f;
                ImGui::SameLine();
                if (ImGui::Button("Derriere", ImVec2(100, 30))) m_mobileLightPos.z += 1.0f;
                
                if (ImGui::Button("Gauche", ImVec2(100, 30))) m_mobileLightPos.x -= 1.0f;
                ImGui::SameLine();
                if (ImGui::Button("Droite", ImVec2(100, 30))) m_mobileLightPos.x += 1.0f;
                ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

                // Suivi dynamique de la caméra sur la pièce sélectionnée
                if (m_isPieceView) {
                    int currentSelected = m_render2D.get_selected_index();
                    if (currentSelected != -1 && currentSelected != m_lastSelectedPiece) {
                        int x = currentSelected % 8;
                        int z = currentSelected / 8;
                        glm::vec3 piecePos(x - 3.5f, 0.0f, z - 3.5f);

                        m_camera.setFirstPersonMode(piecePos);
                        m_lastSelectedPiece = currentSelected;
                    }
                }

                ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

                // Raccourcis de navigation globale
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

                // Alertes visuelles spécifiques au mode Chaos
                if (m_current_game->is_rules_inverted()) {
                    ImGui::Spacing();
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 1.0f, 1.0f));
                    ImGui::TextWrapped("LE CHAOS FRAPPE !!!\nLES REGLES SONT INVERSEES :\nPERDEZ VOTRE ROI POUR GAGNER !");
                    ImGui::PopStyleColor();
                }

                // Historique d'événement de promotion aléatoire
                if (m_current_game->get_game_mode() == GameMode::Chaos) {
                    Type last = m_current_game->get_last_promoted();
                    if (last != Type::None) {
                        ImGui::Text("Derniere promotion : ");
                        ImGui::SameLine();
                        if (last == Type::Queen) ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "UNE REINE ! Tu en as de la chance toi");
                        else if (last == Type::Rook) ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Une tour");
                        else if (last == Type::Bishop) ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Un fou");
                        else if (last == Type::Knight) ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Un cavalier");
                    }
                }

                // Notification de condition de fin de partie
                if (m_current_game->get_state() != GameState::Playing) {
                    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
                    ImGui::SetWindowFontScale(1.5f);
                    if (m_current_game->get_state() == GameState::WhiteWins) ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "VICTOIRE : Les blancs ont gagne !");
                    else ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "VICTOIRE : Les noirs ont gagne !");
                    ImGui::SetWindowFontScale(1.0f);
                }
                
                ImGui::End();
            }
        }}
    );
}