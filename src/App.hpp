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


enum class AppState { MainMenu,
                      InGame };

class App {
private:
    AppState              m_state = AppState::MainMenu;
    std::unique_ptr<Game> m_current_game;

    Menu       m_menu;
    GameRender m_render2D;

    Skybox                m_skybox;
    std::optional<Shader> m_skyboxShader;

    Board                 m_chessBoard3D;
    std::optional<Shader> m_shader;
    GLuint fbo = 0, textureColorbuffer = 0, rbo = 0;
    int viewWidth = 800, viewHeight = 600;
    AmbianceMarkov m_ambiance;

public:
    void run();
    void start_game(GameMode mode);
    void return_to_menu();
};