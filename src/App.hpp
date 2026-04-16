#pragma once
#include <memory>
#include <optional>
#include "Game/Game.hpp"
#include "Game/Utils.hpp"
#include "Menu/Menu.hpp"
#include "Render2D/GameRender.hpp"
#include "Render3D/Board/Board.hpp"
#include "Render3D/OpenGL.hpp"
#include "Render3D/Shader.hpp"
#include "Render3D/Ambiance.hpp"
#include "Render3D/Skybox.hpp"
#include "Render3D/Camera.hpp"

enum class AppState { 
    MainMenu,
    InGame 
};

class App {
private:
    // --- Logique globale ---
    AppState              m_state = AppState::MainMenu;
    std::unique_ptr<Game> m_current_game;

    // --- Rendu 2D ---
    Menu       m_menu;
    GameRender m_render2D;

    // --- Rendu 3D (Environnement) ---
    Skybox                m_skybox;
    std::optional<Shader> m_skyboxShader;
    AmbianceMarkov        m_ambiance;

    // --- Rendu 3D (Échiquier) ---
    Board                 m_chessBoard3D;
    std::optional<Shader> m_shader;
    glm::vec3             m_mobileLightPos = glm::vec3(0.0f, 3.0f, 0.0f);

    // --- Rendu 3D (Framebuffer) ---
    GLuint fbo = 0, textureColorbuffer = 0, rbo = 0;
    int    viewWidth = 800, viewHeight = 600;

    // --- Caméra et Interactions ---
    Camera m_camera;
    bool   m_isPieceView = false;
    int    m_lastSelectedPiece = -1;
    int    m_hovered_idx_3d = -1;

    // --- Animations ---
    int   m_animFrom = -1;
    int   m_animTo = -1;
    float m_animProgress = 1.0f;

    // --- Mode Chaos ---
    bool m_show_chaos_rules = false;

public:
    void run();
    void start_game(GameMode mode);
    void return_to_menu();
};