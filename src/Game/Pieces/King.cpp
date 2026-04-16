#include "King.hpp"
#include "../Chessboard.hpp"
#include <array>

struct Offset {
    int dx;
    int dy;
};

std::vector<Move> King::get_available_moves(const Chessboard& chessboard, Position currentPos) const {
    std::vector<Move> moves;

    std::array<Offset, 8> offsets = {{
        { 0, -1}, { 0,  1}, {-1,  0}, { 1,  0},
        {-1, -1}, { 1, -1}, {-1,  1}, { 1,  1}
    }};

    for (const auto& offset : offsets) {
        
        Position nextPos = { currentPos.x + offset.dx, currentPos.y + offset.dy };

        if (nextPos.is_valid()) {
            
            int nextIdx = nextPos.to_index();

            if (chessboard.is_empty(nextIdx)) {
                moves.push_back({currentPos, nextPos});
            } 
            else {
                Piece* target = chessboard.get_piece(nextIdx);
                if (target->get_color() != this->m_color) {
                    moves.push_back({currentPos, nextPos});
                }
            }
        }
    }

    return moves;
}