#pragma once
#include "Chessboard.hpp"

// class managing game rules, turns, and victory states
class Game {
protected:
    Chessboard m_board;
    Color m_current_turn = Color::White;
    GameState m_state = GameState::Playing;

    // place the standard pieces
    void setup_classic_pieces();

public:
    virtual ~Game() = default;
    
    // initialize
    virtual void setup() = 0; 
    
    // execute a move on the board and handle basic rules (turns, victory)
    virtual bool play_move(int fromIdx, int toIdx, Type promotion = Type::None);
    
    // chaos mode-specific getters
    virtual int get_collapsing_square_idx() const { return -1; }
    virtual bool is_rules_inverted() const { return false; }
    virtual GameMode get_game_mode() const { return GameMode::Classic; }
    virtual Type get_last_promoted() const { return Type::None; }
    
    // general getters
    Chessboard& get_board() { return m_board; }
    Color get_current_turn() const { return m_current_turn; }
    GameState get_state() const { return m_state; }
};