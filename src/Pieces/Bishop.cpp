#include "Bishop.hpp"
#include "../Chessboard.hpp"


struct Direction { int dx; int dy; };

std::vector<Move> Bishop::get_available_moves(const Chessboard& chessboard, Position currentPos) const {
    std::vector<Move> moves;
    
    std::vector<Direction> directions = {
        {-1, -1}, { 1, -1}, {-1,  1}, { 1,  1}
    };

    for (const auto& dir : directions) {
        for (int step = 1; step < 8; ++step) {
            
            // On calcule la nouvelle Position
            Position nextPos = { currentPos.x + (dir.dx * step), currentPos.y + (dir.dy * step) };

            // 1. On utilise ta méthode is_valid() ! C'est super propre.
            if (!nextPos.is_valid()) {
                break; 
            }

            int nextIdx = nextPos.to_index();

            // 2. Si la case est vide
            if (chessboard.is_empty(nextIdx)) { 
                // On ajoute le Move (départ, arrivée)
                moves.push_back({currentPos, nextPos});
            } 
            // 3. S'il y a une pièce
            else {
                Piece* target = chessboard.get_piece(nextIdx); 
                if (target->get_color() != this->m_color) {
                    moves.push_back({currentPos, nextPos});
                }
                break; // Bloqué
            }
        }
    }

    return moves;
}