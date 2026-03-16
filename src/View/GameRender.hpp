#pragma once
#include <vector>
#include <string>
#include "../Chessboard.hpp"

struct ImFont;

class GameRender {
private:
    int m_selected_index = -1;
    std::vector<Move> m_possible_moves;
    ImFont* m_chess_font = nullptr;

    void handle_click(Chessboard& chessboard, int index);
    bool is_highlighted(int index) const;

    void draw_board(Chessboard& chessboard);
    std::string get_piece_label(Piece* p) const;

public:
    void set_font(ImFont* font) { m_chess_font = font; }
    void render(Chessboard& chessboard);
};