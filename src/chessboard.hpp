#pragma once
#include <vector>
#include <imgui.h>

struct ChessState {
    std::vector<char> boardData;
    int selectedIndex = -1;
    std::vector<int> possibleMoves;

    // Constructeur pour initialiser le plateau
    ChessState();
};

void drawchessboard(ChessState& state);