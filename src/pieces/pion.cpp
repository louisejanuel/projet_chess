#include "pion.hpp"
#include <array>

std::vector<int> getPionMoves(int currentIndex, const std::vector<char>& board, bool isWhite) {
    std::vector<int> moves;
    
    int row = currentIndex / 8;
    int col = currentIndex % 8;
    
    // Selon ton boardData : 
    // 'P' (Majuscules) sont en haut (lignes 0-1) -> ils descendent (+1)
    // 'p' (minuscules) sont en bas (lignes 6-7) -> ils montent (-1)
    int direction = isWhite ? 1 : -1; 

    // 1. Avancer d'une case
    int nextRow = row + direction;
    if (nextRow >= 0 && nextRow < 8) {
        int nextIndex = nextRow * 8 + col;
        
        // On ne peut avancer que si la case est VIDE ('.')
        if (board[nextIndex] == '.') {
            moves.push_back(nextIndex);

            // 2. Avancer de DEUX cases (seulement si on est sur la ligne de départ)
            // Ligne de départ pour 'P' (White) : 1
            // Ligne de départ pour 'p' (Black) : 6
            int startRow = isWhite ? 1 : 6;
            int doubleRow = row + (2 * direction);
            
            if (row == startRow) {
                int doubleIndex = doubleRow * 8 + col;
                // La deuxième case aussi doit être VIDE
                if (doubleIndex >= 0 && doubleIndex < 64 && board[doubleIndex] == '.') {
                    moves.push_back(doubleIndex);
                }
            }
        }
    }

    // 3. Manger en diagonale
    std::array<int, 2> diagOffsets = {-1, 1}; 
    for (int offset : diagOffsets) {
        int targetCol = col + offset;
        int targetRow = row + direction;

        if (targetCol >= 0 && targetCol < 8 && targetRow >= 0 && targetRow < 8) {
            int targetIndex = targetRow * 8 + targetCol;
            char targetPiece = board[targetIndex];

            // On ne peut aller en diagonale QUE s'il y a un ennemi
            if (targetPiece != '.') {
                bool targetIsWhite = (targetPiece >= 'A' && targetPiece <= 'Z');
                if (isWhite != targetIsWhite) {
                    moves.push_back(targetIndex);
                }
            }
        }
    }

    return moves;
}