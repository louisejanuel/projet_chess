#include "chessboard.hpp"
#include <string>
#include "pieces/pion.hpp"

ChessState::ChessState()
{
    boardData = {
        'T', 'C', 'F', 'D', 'R', 'F', 'C', 'T',
        'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
        '.', '.', '.', '.', '.', '.', '.', '.',
        '.', '.', '.', '.', '.', '.', '.', '.',
        '.', '.', '.', '.', '.', '.', '.', '.',
        '.', '.', '.', '.', '.', '.', '.', '.',
        'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
        't', 'c', 'f', 'd', 'r', 'f', 'c', 't'
    };
}

// Fonction utilitaire pour la couleur (réduit la complexité de la boucle)
ImVec4 getCellColor(int index, int row, int col, const ChessState& state)
{
    for (int m : state.possibleMoves)
    {
        if (index == m)
            return ImVec4{1.0f, 0.9f, 0.2f, 1.0f}; // Jaune
    }
    bool isLight = (row + col) % 2 == 0;
    return isLight ? ImVec4{0.9f, 0.8f, 0.65f, 1.0f} : ImVec4{0.45f, 0.25f, 0.15f, 1.0f};
}

void handleCellClick(int index, char piece, ChessState& state)
{
    // 1. On vérifie d'abord si on est en train de jouer un mouvement
    bool moveWasMade = false;
    for (int m : state.possibleMoves)
    {
        if (index == m)
        {
            state.boardData[index]               = state.boardData[state.selectedIndex];
            state.boardData[state.selectedIndex] = '.';
            state.selectedIndex                  = -1;
            state.possibleMoves.clear();
            moveWasMade = true;
            break;
        }
    }

    if (!moveWasMade)
    {
        if (piece == 'P' || piece == 'p')
        {
            state.selectedIndex = index;
            state.possibleMoves = getPionMoves(index, state.boardData, (piece == 'P'));
        }
        else
        {
            // Clic sur du vide ou une pièce non gérée : on reset
            state.selectedIndex = -1;
            state.possibleMoves.clear();
        }
    }
}

void drawchessboard(ChessState& state)
{
    const float size = 50.0f;

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            int  index = row * 8 + col;
            char piece = state.boardData[index];

            ImGui::PushStyleColor(ImGuiCol_Button, getCellColor(index, row, col, state));
            ImGui::PushID(index);

            // std::string label = (piece == '.') ? "##" + std::to_string(index) : std::string(1, piece);
            std::string label = std::string(1, piece) + "##" + std::to_string(index);
            if (ImGui::Button(label.c_str(), ImVec2{size, size}))
            {
                handleCellClick(index, piece, state);
            }

            ImGui::PopID();
            ImGui::PopStyleColor();
            if (col < 7)
                ImGui::SameLine(0, 0);
        }
    }
}