#include "Queen.hpp"
#include "../Chessboard.hpp"
#include <array>

struct Direction {
    int dx;
    int dy;
};

std::vector<Move> Queen::get_available_moves(const Chessboard& chessboard, Position currentPos) const {
    std::vector<Move> moves;

    // Les 8 directions
    std::array<Direction, 8> directions = {{
        // Directions de la Tour (Lignes droites)
        { 0, -1}, { 0,  1}, {-1,  0}, { 1,  0},
        // Directions du Fou (Diagonales)
        {-1, -1}, { 1, -1}, {-1,  1}, { 1,  1}
    }};

    for (const auto& dir : directions) {
        
        // La Reine "glisse" jusqu'à 7 cases, comme la Tour et le Fou
        for (int step = 1; step < 8; ++step) {
            
            Position nextPos = { currentPos.x + (dir.dx * step), currentPos.y + (dir.dy * step) };

            // 1. Si on sort du plateau, on arrête d'explorer cette direction
            if (!nextPos.is_valid()) {
                break; 
            }

            int nextIdx = nextPos.to_index();

            // 2. Si la case est vide, on l'ajoute et on continue d'avancer
            if (chessboard.is_empty(nextIdx)) {
                moves.push_back({currentPos, nextPos});
            } 
            // 3. S'il y a une pièce
            else {
                Piece* target = chessboard.get_piece(nextIdx);
                
                // Si c'est un ennemi, on peut le manger
                if (target->get_color() != this->m_color) {
                    moves.push_back({currentPos, nextPos});
                }
                
                // Quoi qu'il arrive, on est bloqué par la pièce, on arrête cette direction
                break; 
            }
        }
    }

    return moves;
}