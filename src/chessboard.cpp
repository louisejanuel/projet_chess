#include "chessboard.hpp"

void drawchessboard()
{
    // {R, G, B, A}
    ImVec4 color1 = ImVec4{1.0f, 0.8f, 0.25f, 0.8f};
    ImVec4 color2 = ImVec4{0.25f, 0.8f, 1.0f, 0.8f};
    float  size   = 50.0f; // Taille d'une case

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            // Si (ligne + colonne) pair, couleur A, sinon couleur B
            bool   is1          = (row + col) % 2 == 0;
            ImVec4 currentColor = is1 ? color1 : color2;

            // Couleur du bouton
            ImGui::PushStyleColor(ImGuiCol_Button, currentColor);

            // ID unique pour chaque case 
            ImGui::PushID(row * 8 + col);

            if (ImGui::Button(" ", ImVec2{size, size}))
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
