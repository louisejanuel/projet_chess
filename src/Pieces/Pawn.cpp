#include "Pawn.hpp"
#include "../Chessboard.hpp"

std::vector<int> Pawn::get_available_moves(const Chessboard& chessboard, int currentIdx) const {
    std::vector<int> moves;
    
    int y = currentIdx / 8;
    int x = currentIdx % 8;

    int direction = (m_color == Color::White) ? -1 : 1; 

    //move one square forward
    int nextY = y + direction;
    int nextIdx = nextY * 8 + x;
    
    if (nextY >= 0 && nextY < 8 && chessboard.is_empty(nextIdx)) {
        moves.push_back(nextIdx);

        //if first move, can move two squares
        if (!m_has_moved) {
            int doubleY = y + (2 * direction);
            int doubleIdx = doubleY * 8 + x;
            if (doubleY >= 0 && doubleY < 8 && chessboard.is_empty(doubleIdx)) {
                moves.push_back(doubleIdx);
            }
        }
    }

    //eating moves
    //left diagonal
    int captureLeftX = x - 1;
    if (captureLeftX >= 0) {
        int captureLeftIdx = nextY * 8 + captureLeftX;
        Piece* target = chessboard.get_piece(captureLeftIdx);
    
        if (target != nullptr && target->get_color() != this->m_color) {
            moves.push_back(captureLeftIdx);
        }
    }

    //right diagonal
    int captureRightX = x + 1;
    if (captureRightX <= 7) {
        int captureRightIdx = nextY * 8 + captureRightX;
        Piece* target = chessboard.get_piece(captureRightIdx);

        if (target != nullptr && target->get_color() != this->m_color) {
            moves.push_back(captureRightIdx);
        }
    }

    return moves;
}