#pragma once
#include "Chessboard.hpp"

class Game {
protected:
    Chessboard m_board;
    Color m_current_turn = Color::White;
    GameState m_state = GameState::Playing;

    void setup_classic_pieces(); // Place les 32 pièces normales

public:
    virtual ~Game() = default;
    
    virtual void setup() = 0; 
    virtual bool play_move(int fromIdx, int toIdx, Type promotion = Type::None);
    virtual int get_collapsing_square_idx() const { return -1; }
    
    Chessboard& get_board() { return m_board; }
    Color get_current_turn() const { return m_current_turn; }
    GameState get_state() const { return m_state; }
};