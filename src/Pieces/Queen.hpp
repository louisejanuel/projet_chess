#pragma once
#include "Piece.hpp"

class Queen : public Piece {
public:
    explicit Queen(Color color) : Piece(color, Type::Queen) {}

    std::vector<Move> get_available_moves(const Chessboard& chessboard, Position currentPos) const override;
};