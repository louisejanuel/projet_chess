#pragma once
#include <vector>
#include "Piece.hpp"


class Chessboard;

class BerolinaPawn : public Piece {
public:
    BerolinaPawn(Color color);
    std::vector<Move> get_available_moves(const Chessboard& chessboard, Position currentPos) const override;
};