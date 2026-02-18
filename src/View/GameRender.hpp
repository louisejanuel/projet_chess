#pragma once
#include <vector>
#include "../Chessboard.hpp"

class GameRender {
private:
    int m_selected_index = -1;
    std::vector<int> m_possible_moves;

    void handle_click(Chessboard& chessboard, int index);
    bool is_highlighted(int index) const;

public:
    void render(Chessboard& chessboard);
};