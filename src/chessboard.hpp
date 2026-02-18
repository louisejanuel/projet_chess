#pragma once
#include <array>
#include <memory>
#include <vector>
#include "Pieces/Piece.hpp"
#include "Utils.hpp"

class Chessboard {
private:
    std::array<std::unique_ptr<Piece>, 64> m_pieces;
    Color m_current_turn;

public:
    Chessboard();

    Color get_current_turn() const { return m_current_turn; }

    Piece* get_piece(int index) const {
        if (index < 0 || index >= 64) return nullptr;
        return m_pieces[index].get();
    }

    bool move_piece(int fromIdx, int toIdx);
    
    bool is_empty(int index) const {
        return m_pieces[index] == nullptr;
    }
};