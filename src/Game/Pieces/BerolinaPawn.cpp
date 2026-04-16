#include "BerolinaPawn.hpp"
#include "../Chessboard.hpp"

BerolinaPawn::BerolinaPawn(Color color) : Piece(color, Type::BerolinaPawn) {}

std::vector<Move> BerolinaPawn::get_available_moves(const Chessboard& chessboard, Position currentPos) const
{
    std::vector<Move> moves;
    int               dy = (this->m_color == Color::White) ? -1 : 1;

    // Déplacement en diagonale (sans capture)
    Position diagLeft = {.x=currentPos.x - 1, .y=currentPos.y + dy};
    if (diagLeft.is_valid() && chessboard.is_empty(diagLeft.to_index()))
    {
        moves.push_back({.start=currentPos, .end=diagLeft});
    }

    Position diagRight = {.x=currentPos.x + 1, .y=currentPos.y + dy};
    if (diagRight.is_valid() && chessboard.is_empty(diagRight.to_index()))
    {
        moves.push_back({.start=currentPos, .end=diagRight});
    }

    // Capture tout droit
    Position straight = {.x=currentPos.x, .y=currentPos.y + dy};
    if (straight.is_valid() && !chessboard.is_empty(straight.to_index()))
    {
        Piece* target = chessboard.get_piece(straight.to_index());
        if (target->get_color() != this->m_color)
        {
            moves.push_back({.start=currentPos, .end=straight});
        }
    }

    return moves;
}