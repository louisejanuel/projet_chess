#include "Chessboard.hpp"
#include "View/GameRender.hpp"
#include "quick_imgui/quick_imgui.hpp"
#include <imgui.h>
#include <fstream>
#include <iostream>

int main()
{
    ImFont* chess_font = nullptr;

    quick_imgui::loop(
        "Chess Game",
        {
            .init = [&]() {
                ImGuiIO& io = ImGui::GetIO();
                io.Fonts->AddFontDefault();
                
                static const ImWchar ranges[] = {
                    0x0020, 0x00FF, 
                    0x2650, 0x265F,
                    0
                };

                const char *fontPath = "../../src/Assets/DejaVuSans.ttf"; 
                std::ifstream fileCheck(fontPath);

                if (fileCheck.good()) {
                    // On passe 'ranges' à la fin pour charger les symboles
                    chess_font = io.Fonts->AddFontFromFileTTF(fontPath, 75.0f, nullptr, ranges);
                    std::cout << "SUCCES : Police chargee depuis " << fontPath << std::endl;
                } else {
                    std::cout << "ERREUR CRITIQUE : Le fichier " << fontPath << " n'existe pas !" << std::endl;
                }
            },
            .loop = [&]() {
                static Chessboard chessboard; 
                static GameRender game_render; 
                
                game_render.set_font(chess_font);
                game_render.render(chessboard);
            },
        }
    );
    return 0;
}