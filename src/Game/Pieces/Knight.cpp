#include "Knight.hpp"
#include "../Chessboard.hpp"
#include <array>

struct Offset {
    int dx;
    int dy;
};

std::vector<Move> Knight::get_available_moves(const Chessboard& chessboard, Position currentPos) const {
    std::vector<Move> moves;

    std::array<Offset, 8> offsets = {{
        {-2, -1}, {-2,  1}, // 2 gauche, 1 haut/bas
        {-1, -2}, {-1,  2}, // 1 gauche, 2 haut/bas
        { 1, -2}, { 1,  2}, // 1 droite, 2 haut/bas
        { 2, -1}, { 2,  1}  // 2 droite, 1 haut/bas
    }};

    for (const auto& offset : offsets) {
        
        // On calcule directement la case d'arrivée
        Position nextPos = { currentPos.x + offset.dx, currentPos.y + offset.dy };

        // 1. Est-ce que la case d'arrivée existe sur le plateau ?
        if (nextPos.is_valid()) {
            
            int nextIdx = nextPos.to_index();

            // 2. Si la case est vide, on peut y aller
            if (chessboard.is_empty(nextIdx)) {
                moves.push_back({currentPos, nextPos});
            } 
            // 3. S'il y a une pièce, on vérifie si c'est un ennemi qu'on peut manger
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