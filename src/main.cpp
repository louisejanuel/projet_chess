#include <imgui.h>
#include "chessboard.hpp"
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    float value{0.f};

    quick_imgui::loop(
        "Chess",
        {
            .init = [&]() {},
            .loop = [&]() {
            ImGui::Begin("Chess Board");

            static ChessState state; 
            drawchessboard(state);

            ImGui::End(); },
        }
    );

    return 0;
}