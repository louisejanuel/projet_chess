#include "Queen.hpp"
#include "../Chessboard.hpp"
#include <array>

struct Direction {
    int dx;
    int dy;
};

std::vector<Move> Queen::get_available_moves(const Chessboard& chessboard, Position currentPos) const {
    std::vector<Move> moves;

    std::array<Direction, 8> directions = {{
        { 0, -1}, { 0,  1}, {-1,  0}, { 1,  0},
        {-1, -1}, { 1, -1}, {-1,  1}, { 1,  1}
    }};

    for (const auto& dir : directions) {
        
        for (int step = 1; step < 8; ++step) {
            
            Position nextPos = { currentPos.x + (dir.dx * step), currentPos.y + (dir.dy * step) };

            if (!nextPos.is_valid()) {
                break; 
            }

            int nextIdx = nextPos.to_index();

            if (chessboard.is_empty(nextIdx)) {
                moves.push_back({currentPos, nextPos});
            } 
            else {
                Piece* target = chessboard.get_piece(nextIdx);
                
                if (target->get_color() != this->m_color) {
                    moves.push_back({currentPos, nextPos});
                }
                
                break; 
            }
        }
    }

    return moves;
}