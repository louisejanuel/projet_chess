#pragma once
#include "Game.hpp"

class GameChaos : public Game {
private:
    int m_collapsing_square_idx = -1;

public:
    void setup() override;
    bool play_move(int fromIdx, int toIdx, Type promotion = Type::None) override;
    
    int get_collapsing_square_idx() const { return m_collapsing_square_idx; }
};