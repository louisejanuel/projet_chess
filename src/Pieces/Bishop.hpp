#pragma once
#include "Piece.hpp"

class Bishop : public Piece {
public:
    explicit Bishop(Color color) : Piece(color, Type::Bishop) {}
    
    virtual std::vector<Move> get_available_moves(const Chessboard& board, Position currentPos) const override;
};