#pragma once
#include "Piece.hpp"

class King : public Piece {
public:
    explicit King(Color color) : Piece(color, Type::King) {}

    std::vector<Move> get_available_moves(const Chessboard& chessboard, Position currentPos) const override;
};