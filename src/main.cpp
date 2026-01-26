#include <imgui.h>
#include "quick_imgui/quick_imgui.hpp"
#include "chessboard.hpp"

int main()
{
    float value{0.f};

    quick_imgui::loop(
        "Chess",
        {
            .init = [&]() {},
            .loop = [&]() {
            ImGui::Begin("Chess Board");

            drawchessboard();

            ImGui::End(); },
        }
    );

    return 0;
}