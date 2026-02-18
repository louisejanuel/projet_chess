#pragma once
#include "Piece.hpp"

class Pawn : public Piece {
public:
    explicit Pawn(Color color) : Piece(color, Type::Pawn) {}

    std::vector<int> get_available_moves(const Chessboard& chessboard, int currentIdx) const override;
};