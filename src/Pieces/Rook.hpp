#pragma once
#include "Piece.hpp"

class Rook : public Piece {
public:
    explicit Rook(Color color) : Piece(color, Type::Rook) {}

    virtual std::vector<Move> get_available_moves(const Chessboard& board, Position currentPos) const override;
};