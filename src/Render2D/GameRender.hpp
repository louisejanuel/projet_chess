#pragma once
#include <string>
#include <vector>
#include "../Game/Game.hpp"
#include "../Game/GameChaos.hpp"
#include "../Render3D/Ambiance.hpp"

struct ImFont;

class GameRender {
private:
    int               m_selected_index = -1;
    std::vector<Move> m_possible_moves;
    ImFont*           m_chess_font = nullptr;

    bool m_awaiting_promotion     = false;
    Move m_pending_promotion_move = {{0, 0}, {0, 0}, Type::None};

    void        draw_board(Game& game, AmbianceMarkov& ambiance);
    std::string get_piece_label(Piece* p) const;

    void draw_promotion_popup(Game& game, AmbianceMarkov& ambiance);

public:
    void set_font(ImFont* font) { m_chess_font = font; }
    void reset();
    void render(Game& game, AmbianceMarkov& ambiance); 
    bool is_highlighted(int index) const;

    void handle_click(Game& game, int index, AmbianceMarkov& ambiance);
    
    int get_selected_index() const { return m_selected_index; }
    const std::vector<Move>& get_possible_moves() const { return m_possible_moves; }
};