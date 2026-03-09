#pragma once
#include "Piece.hpp"

class Pawn : public Piece {
public:
    explicit Pawn(Color color) : Piece(color, Type::Pawn) {}

    std::vector<Move> get_available_moves(const Chessboard& board, Position currentPos) const override;
};