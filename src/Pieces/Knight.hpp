#pragma once
#include "Piece.hpp"

class Knight : public Piece {
public:
    explicit Knight(Color color) : Piece(color, Type::Knight) {}

    std::vector<Move> get_available_moves(const Chessboard& chessboard, Position currentPos) const override;
};