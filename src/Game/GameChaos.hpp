#pragma once
#include "Game.hpp"

class GameChaos : public Game {
    private:
    // index of the purple square
    int m_collapsing_square_idx = -1;
    
    // variables for 'lose to win'
    int m_turn_count = 0;
    int m_chaos_turn_trigger = -1;
    bool m_inverted_rules = false;

    // stores the result of the random pawn promotion
    Type m_last_promoted = Type::None;

public:
    void setup() override;
    
    bool play_move(int fromIdx, int toIdx, Type promotion = Type::None) override;
    
    // overridden getters
    int get_collapsing_square_idx() const override { return m_collapsing_square_idx; }
    bool is_rules_inverted() const override { return m_inverted_rules; }
    GameMode get_game_mode() const override { return GameMode::Chaos; }
    Type get_last_promoted() const override { return m_last_promoted; }
};