#include "Pawn.hpp"
#include "../Chessboard.hpp"

std::vector<Move> Pawn::get_available_moves(const Chessboard& chessboard, Position currentPos) const {
    std::vector<Move> moves;
    
    int direction = (m_color == Color::White) ? -1 : 1;

    Position nextPos = { currentPos.x, currentPos.y + direction };
    
    if (nextPos.is_valid() && chessboard.is_empty(nextPos.to_index())) {
        moves.push_back({currentPos, nextPos});

        if (!m_has_moved) { //
            Position doublePos = { currentPos.x, currentPos.y + (2 * direction) };
            if (doublePos.is_valid() && chessboard.is_empty(doublePos.to_index())) {
                moves.push_back({currentPos, doublePos});
            }
        }
    }

    Position captureLeft = { currentPos.x - 1, currentPos.y + direction };
    if (captureLeft.is_valid()) {
        Piece* target = chessboard.get_piece(captureLeft.to_index());
        if (target != nullptr && target->get_color() != this->m_color) { //
            moves.push_back({currentPos, captureLeft});
        }
    }

    Position captureRight = { currentPos.x + 1, currentPos.y + direction };
    if (captureRight.is_valid()) {
        Piece* target = chessboard.get_piece(captureRight.to_index());
        if (target != nullptr && target->get_color() != this->m_color) { //
            moves.push_back({currentPos, captureRight});
        }
    }

    return moves;
}