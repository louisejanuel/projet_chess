#include "King.hpp"
#include "../Chessboard.hpp"
#include <array>

struct Offset {
    int dx;
    int dy;
};

std::vector<Move> King::get_available_moves(const Chessboard& chessboard, Position currentPos) const {
    std::vector<Move> moves;

    // Les 8 cases autour du Roi
    std::array<Offset, 8> offsets = {{
        { 0, -1}, { 0,  1}, {-1,  0}, { 1,  0}, // Haut, Bas, Gauche, Droite
        {-1, -1}, { 1, -1}, {-1,  1}, { 1,  1}  // Les 4 diagonales
    }};

    for (const auto& offset : offsets) {
        
        // On calcule la case d'arrivée directement
        Position nextPos = { currentPos.x + offset.dx, currentPos.y + offset.dy };

        // 1. Si la case est bien sur le plateau
        if (nextPos.is_valid()) {
            
            int nextIdx = nextPos.to_index();

            // 2. Si la case est vide
            if (chessboard.is_empty(nextIdx)) {
                moves.push_back({currentPos, nextPos});
            } 
            // 3. S'il y a une pièce, on vérifie si c'est un ennemi
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