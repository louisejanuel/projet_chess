#include "chessboard.hpp"

void drawchessboard()
{
    // {R, G, B, A}
    ImVec4 color1 = ImVec4{0.45f, 0.25f, 0.15f, 1.0f};
    ImVec4 color2 = ImVec4{0.9f, 0.8f, 0.65f, 1.0f};
    float  size   = 50.0f;

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            // Couleurs chessboard
            bool   is1          = (row + col) % 2 == 0;
            ImVec4 currentColor = is1 ? color2 : color1;
            ImGui::PushStyleColor(ImGuiCol_Button, currentColor);

            // ID
            std::array<char, 64> chessboard;
            int index = row * 8 + col;
            ImGui::PushID(index);

            const char* label = " ";
            // getPieceAtPos(index)
            
            if (index >= 8 && index <= 15 || index >= 48 && index <= 55)
            {
                label = "P";
            }
            else if (index == 0 || index == 7 || index == 56 || index == 63)
            {
                label = "T";
            }
            else if (index == 1 || index == 6 || index == 57 || index == 62)
            {
                label = "C";
            }
            else if (index == 2 || index == 5 || index == 58 || index == 61)
            {
                label = "F";
            }
            else if (index == 3 || index == 59)
            {
                label = "D";
            }
            else if (index == 4 || index == 60)
            {
                label = "R";
            }            



            if (ImGui::Button(label, ImVec2{size, size}))
            {
                std::cout << "Case cliquee : " << row << ", " << col << "\n";
            }

            ImGui::PopID();
            ImGui::PopStyleColor();

            // Si ce n'est pas la dernière colonne, on reste sur la même ligne
            if (col < 7)
            {
                ImGui::SameLine(0, 0); // 0, 0 pour coller les cases entre elles
            }
        }
        // À la fin de chaque ligne, ImGui passe automatiquement à la ligne suivante
    }
}
