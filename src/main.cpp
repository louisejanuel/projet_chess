#include "Chessboard.hpp"
#include "View/GameRender.hpp"
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    quick_imgui::loop(
        "Chess Game",
        {
            .init = [&]() {},
            .loop = [&]() {
                static Chessboard chessboard; 
                
                static GameRender game_render; 
                
                game_render.render(chessboard);
            },
        }
    );
    return 0;
}