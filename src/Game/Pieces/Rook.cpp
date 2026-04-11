#include "Rook.hpp"
#include "../Chessboard.hpp"
#include <array>

struct Direction { 
    int dx; 
    int dy; 
};

std::vector<Move> Rook::get_available_moves(const Chessboard& chessboard, Position currentPos) const {
    std::vector<Move> moves;

    std::array<Direction, 4> directions = {{
        { 0, -1}, 
        { 0,  1}, 
        {-1,  0}, 
        { 1,  0}  
    }};

    // On teste chaque direction
    for (const auto& dir : directions) {
        
        // On avance pas à pas (jusqu'à 7 cases maximum)
        for (int step = 1; step < 8; ++step) {
            
            // Calcul de la nouvelle position grâce à la struct
            Position nextPos = { currentPos.x + (dir.dx * step), currentPos.y + (dir.dy * step) };

            // 1. Si on sort du plateau, on arrête cette direction direct
            if (!nextPos.is_valid()) {
                break; 
            }

            int nextIdx = nextPos.to_index();

            // 2. Si la case est vide, on ajoute le mouvement et on continue
            if (chessboard.is_empty(nextIdx)) {
                moves.push_back({currentPos, nextPos});
            } 
            // 3. S'il y a une pièce sur la case
            else {
                Piece* target = chessboard.get_piece(nextIdx);
                
                // Si c'est un ennemi, on l'ajoute comme mouvement valide (capture)
                if (target->get_color() != this->m_color) {
                    moves.push_back({currentPos, nextPos});
                }
                
                // Quoi qu'il arrive (allié ou ennemi), la tour est bloquée et ne peut pas sauter
                break; 
            }
        }
    }

    return moves;
}