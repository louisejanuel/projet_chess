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

    bool m_awaiting_promotion = false;
    Move m_pending_promotion_move = { {0,0}, {0,0}, Type::None };

    void handle_click(Chessboard& chessboard, int index);
    bool is_highlighted(int index) const;
    void draw_board(Chessboard& chessboard);
    std::string get_piece_label(Piece* p) const;
    
    void draw_promotion_popup(Chessboard& chessboard); 

public:
    void set_font(ImFont* font) { m_chess_font = font; }
    void render(Chessboard& chessboard);
};